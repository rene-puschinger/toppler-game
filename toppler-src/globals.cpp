/*****************************************************************************************/
/* Toppler - globals.cpp                                                                 */
/* global variables, item coordinates and global functions definition                    */
/*                                                                                       */
/*****************************************************************************************/

#include "globals.h"

/*****************************************************************************************/

/* definitions of the item coordinates */
RECT LILIES_COORD[NUMLILIES+NUMRIPPLES] = {			/* lilies (incl. ending ripples) */
	{1, 1, 174, 111}, {175, 1, 348, 111}, {349, 1, 522, 111}, {523, 1, 696, 111},
	{1, 112, 174, 222}, {175, 112, 348, 222}, {349, 112, 522, 222}, {523, 112, 696, 222},
	{1, 223, 174, 333}, {175, 223, 348, 333}, {349, 223, 522, 333}, {523, 223, 696, 333},
	{1, 334, 174, 444}, {175, 334, 348, 444}, {349, 334, 522, 444}, {523, 334, 696, 444},
	{52, 476, 225, 586}, {226, 484, 399, 593}, {417, 445, 590, 555}, {591, 445, 764, 555},
	{417, 556, 590, 666}, {591, 556, 764, 666}
};
RECT STAR_COORD = {13, 458, 40, 482};						/* red star (level end) */
RECT BONUS_SMALL_COORD = {61, 455, 73, 466};		/* small bonus (blue) */
RECT BONUS_BIG_COORD = {92, 455, 104, 466};			/* big bonus (orange) */
RECT BONUS_ADV_COORD = {123, 455, 135, 466};		/* adventure (grey) */
RECT BONUS_NEG_COORD = {154, 455, 166, 466};		/* negative 'bonus' (brown square) */
RECT CROSS_COORD = {1, 523, 18, 553};						/* cross (death) */
RECT FROG_COORD[] = {
	{176, 445, 193, 466},													/* frog - up to down */
	{194, 445, 212, 466},													/* frog - right to left */
	{232, 445, 251, 466},													/* frog - left to right */
	{213, 445, 231, 467}													/* frog - down to up */
};
RECT FLY_COORD[] = {														/* big fly */
	{280, 445, 307, 474}, {280, 445, 307, 474},
	{252, 445, 279, 474}, {280, 445, 307, 474},
	{308, 445, 335, 474}, {280, 445, 307, 474},
	{336, 445, 369, 474}, {280, 445, 307, 474},
	{280, 445, 307, 474}, {370, 445, 403, 474}
};
RECT APHID_COORD[] = {													/* aphid */
	{1, 593, 21, 614}, {22, 593, 42, 614},
	{1, 593, 21, 614}, {22, 593, 42, 614},
	{43, 593, 65, 614},	{1, 593, 21, 614},
	{22, 593, 42, 614}, {1, 593, 21, 614},
	{66, 593, 88, 614}
};
RECT STATUS_COORD = {0, 667, 800, 707};					/* status bar */
RECT NUMBERS[10] = {														/* numbers 0 - 9 */
	{1, 710, 14, 726}, {15, 710, 28, 726},
	{29, 710, 42, 726}, {43, 710, 56, 726},
	{57, 710, 70, 726}, {71, 710, 84, 726},
	{85, 710, 98, 726}, {99, 710, 112, 726},
	{113, 710, 126, 726}, {127, 710, 140, 726}
};
RECT FIRST_CHAR = {1, 1100, 16, 1118};					/* coords of the first character literal */
RECT MENU_COORD = {1, 727, 505, 1031};					/* main menu */
RECT HS_COORD = {506, 727, 880, 1031};					/* high score */
RECT RECORD_COORD = {1, 1119, 405, 1183};				/* record to high score */
RECT NOTICE_COORD = {0, 1059, 800, 1099};				/* notice how to close the menu */
RECT WIN_COORD = {407, 1119, 841, 1273};				/* game-won notice */
RECT BALL_COORD = {1, 571, 21, 591};						/* ball used in menu */
RECT VOLUME_COORD = {29, 523, 31, 539};					/* sound/music volume indicator */

/*****************************************************************************************/

/* definition of other global variables */
int error = ERR_OK;

char* error_msg[] = {"", "Unknown error!", "Not enough memory!",
	"Some file(s) corrupted. Please reinstall the game!",
	"DirectDraw can't initialize. Make sure you have DirectX 7 or above installed!",
	"Unknown DirectX error!",
	"Can't set graphic mode to 800x600 with 256 colors!", "Can't initialize DirectInput!"};

HWND hwnd;

LPDIRECTDRAW7 lpDD = NULL;
LPDIRECTDRAWPALETTE lpPal = NULL;
LPDIRECTDRAWSURFACE7 lpSPrimary = NULL;
LPDIRECTDRAWSURFACE7 lpSBack = NULL;
LPDIRECTDRAWSURFACE7 lpSItems = NULL;
LPDIRECTDRAWSURFACE7 lpSScreen[WATER_FRAMES];
LPDIRECTINPUT lpDI = NULL;
LPDIRECTINPUTDEVICE lpKeyboard = NULL;
LPDIRECTSOUND lpDS = NULL;
LPDIRECTSOUNDBUFFER lpSound[NUMSOUNDS];
IDirectMusicPerformance* lpDM = NULL;
IDirectMusicLoader* lpLoader = NULL;
IDirectMusicSegment* lpMusic[NUMMIDIS];
IDirectMusicSegmentState* lpStates[NUMMIDIS];

PALETTEENTRY pal[COLORS];
bool play_sound;
int sound_volume = 3;
bool play_music;
int music_volume = 3;

/*****************************************************************************************/

/* definition of global functions */

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch(msg) {
		case WM_KILLFOCUS:	/* close the program if lost focus, quite lame but effective :-) */
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_SYSKEYDOWN:	/* ignore certain system keys such as ALT-F4 */
		case WM_SYSKEYUP:
		case WM_SYSCOMMAND:
			return 1;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

/*****************************************************************************************/

DWORD file_size(const char* fn) {
	std::ifstream fin(fn, std::ios::binary);
	if (!fin)
		return 0;
	fin.seekg(0, std::ios::end);
	DWORD result = fin.tellg();
	fin.close();
	return result;
}

/*****************************************************************************************/

void blit_item(DWORD x, DWORD y, RECT& src_coords) {
	check_surf(lpSBack);
	lpSBack->BltFast(x, y, lpSItems, &src_coords, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);
}

/*****************************************************************************************/

void check_surf(LPDIRECTDRAWSURFACE7 surf) {
	if (surf->IsLost() == DDERR_SURFACELOST)
		surf->Restore();
}

/*****************************************************************************************/

int loadlvl(const char* fn, LevelInfo* levinfo) {
	std::ifstream fin(fn, std::ios::binary);
	if (!fin)
		return (error = ERR_MISSINGFILE);
	fin.read((char*) levinfo, sizeof(LevelInfo)*NUMLEVELS);
	fin.close();
	return (error = ERR_OK);
}

/*****************************************************************************************/

int loadpal(const char* fn) {
	std::ifstream fin(fn, std::ios::binary);
	if (!fin)
		return (error = ERR_MISSINGFILE);
	for (int i = 0; i < COLORS; i++) {
		fin.get((char&) pal[i].peRed);
		fin.get((char&) pal[i].peGreen);
		fin.get((char&) pal[i].peBlue);
		pal[i].peFlags = PC_NOCOLLAPSE;
	}
	fin.close();
	return (error = ERR_OK);
}

/*****************************************************************************************/

int loadpic(const char* fn, LPDIRECTDRAWSURFACE7 surf, int szx, int szy) {
	std::ifstream fin(fn, std::ios::binary);
	if (!fin) 
		return (error = ERR_MISSINGFILE);
	char* pic = new char[szx*szy];
	if (!pic)
		return (error = ERR_MEMORY);
	fin.read(pic, szx*szy);
	check_surf(surf);
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	surf->Lock(NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL);
	char* surf_mem = (char*) ddsd.lpSurface;
	int pitch = ddsd.lPitch;
	int c = 0;
	for (int j = 0; j < szy; j++)
		for (int i = 0; i < szx; i++)	
			surf_mem[i+pitch*j] = pic[c++];
	surf->Unlock(NULL);
  fin.close();
	delete [] pic;
	return (error = ERR_OK);
}

/*****************************************************************************************/

int loadcfg(const char *fn, HighScoreEntry* hs) {
	std::ifstream fin(fn, std::ios::binary);
	if (!fin) return (error = ERR_MISSINGFILE);
	/* read checksum */
	DWORD file_check;
	fin.read((char*) &file_check, sizeof(DWORD));
	/* read crypted high score */
	HighScoreEntry crypt_hs[NUM_NAMES];
	fin.read((char*) crypt_hs, sizeof(HighScoreEntry)*NUM_NAMES);
	/* compute actual checksum */
	DWORD check = 0;
	for (int i = 0; i < sizeof(HighScoreEntry)*NUM_NAMES; i++)
		check += (DWORD) ((char*) crypt_hs)[i];
	/* test checksum */
	if (file_check != check)
		return (error = ERR_MISSINGFILE);	/* some lamer corrupted the HighScore */
	/* decrypt high score */
	for (int i = 0; i < sizeof(HighScoreEntry)*NUM_NAMES; i++)
		((char*) crypt_hs)[i] ^= (char) i;
	memcpy((void*) hs, (void*) crypt_hs, sizeof(HighScoreEntry)*NUM_NAMES);
	fin.get((char&) music_volume);
	fin.get((char&) sound_volume);
	fin.close();
	return (error = ERR_OK);
}

/*****************************************************************************************/

void savecfg(const char* fn, HighScoreEntry* hs) {
	std::ofstream fout(fn, std::ios::binary);
	if (!fout) return;	/* this could mean denied access - a serious problem */
	/* first encrypt the high score */
	HighScoreEntry crypt_hs[NUM_NAMES];
	memcpy((void*) crypt_hs, (void*) hs, sizeof(HighScoreEntry)*NUM_NAMES);
	for (int i = 0; i < sizeof(HighScoreEntry)*NUM_NAMES; i++)
		((char*) crypt_hs)[i] ^= (char) i;
	/* compute checksum for high score */
	DWORD check = 0;
	for (int i = 0; i < sizeof(HighScoreEntry)*NUM_NAMES; i++)
		check += (DWORD) ((char*) crypt_hs)[i];
	fout.write((char*) &check, sizeof(DWORD));
	fout.write((char*) crypt_hs, sizeof(HighScoreEntry)*NUM_NAMES);
	fout.put((char) music_volume);
	fout.put((char) sound_volume);
	fout.close();
}

/*****************************************************************************************/

int loadmus(const char* fn, IDirectMusicSegment** seg) {
	/* if unable to play music, return immediately with no error */
	if (!play_music) return (error = ERR_OK);
	/* set the search path to working directory */
	WCHAR search_path[_MAX_PATH];
	_wgetcwd(search_path, _MAX_PATH);
	lpLoader->SetSearchDirectory(GUID_DirectMusicAllTypes, search_path, false);
	/* convert the file name fn to wide characters */
	WCHAR fn_wide[20];
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, fn, -1, fn_wide, 20);
	DMUS_OBJECTDESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.dwSize = sizeof(desc);
	desc.dwValidData = DMUS_OBJ_CLASS | DMUS_OBJ_FILENAME;
	desc.guidClass = CLSID_DirectMusicSegment;
	wcscpy(desc.wszFileName, fn_wide);
	/* create the music segment */
	play_music = play_music && SUCCEEDED(lpLoader->GetObject(&desc, IID_IDirectMusicSegment, (void**) seg));
	if (!play_music) return (error = ERR_OK);
	(*seg)->SetParam(GUID_StandardMIDIFile, (DWORD) -1, 0, 0, (LPVOID) lpDM);
	(*seg)->SetParam(GUID_Download, -1, 0, 0, (LPVOID) lpDM);
	(*seg)->SetParam(GUID_EnableTempo, -1, 0, 0, NULL);
	return (error = ERR_OK);
}

/*****************************************************************************************/

int loadsnd(const char* fn, LPDIRECTSOUNDBUFFER buf) {
	DWORD fsize = file_size(fn);
	std::ifstream fin(fn, std::ios::binary);
	if (!fin)
		return (error = ERR_MISSINGFILE);
	if (!play_sound) return (error = ERR_OK);
	char* snd = new char[fsize];
	if (!snd)
		return (error = ERR_MEMORY);
	fin.read(snd, fsize);
	char* ptr1 = NULL;		/* the first buffer */
	DWORD bytes1 = 0;			/* bytes in the first buffer */
	char* ptr2 = NULL;
	DWORD bytes2 = 0;
	/* lock the sound buffer and get the two pointers to write to */
	if (FAILED(buf->Lock(0, fsize, (LPVOID*) &ptr1, &bytes1, (LPVOID*) &ptr2, &bytes2, DSBLOCK_ENTIREBUFFER)))
		return (error = ERR_GENERIC);
	memcpy(ptr1, snd, bytes1);
	memcpy(ptr2, snd + bytes1, bytes2);
	buf->Unlock(ptr1, bytes1, ptr2, bytes2);
	buf->SetCurrentPosition(0);
	fin.close();
	delete [] snd;
	return (error = ERR_OK);
}

/*****************************************************************************************/

void play_mus(int mus_number) {
	if (play_music) 
		lpDM->PlaySegment(lpMusic[mus_number], 0, 0, &lpStates[mus_number]);
}

/*****************************************************************************************/

void stop_mus(int mus_number) {
	if (play_music)
    lpDM->Stop(lpMusic[mus_number], lpStates[mus_number], 0, 0);
}

/*****************************************************************************************/

bool is_mus_playing(int mus_number) {
	return (lpDM->IsPlaying(lpMusic[mus_number], NULL) == S_OK);
}

/*****************************************************************************************/

bool is_mus_playing() {
	for (int i = 0; i < NUMMIDIS; i++) {
		if (is_mus_playing(i)) return true;
	}
	return false;
}

/*****************************************************************************************/

void play_snd(int snd_number) {
	if (play_sound)
    lpSound[snd_number]->Play(0, 0, 0);
}

/*****************************************************************************************/

void stop_snd(int snd_number) {
	if (play_sound) {
		lpSound[snd_number]->Stop();
		lpSound[snd_number]->SetCurrentPosition(0);
	}
}

/*****************************************************************************************/

bool is_snd_playing(int snd_number) {
	if (!play_sound) return false;
	DWORD state;
	lpSound[snd_number]->GetStatus(&state);
	return (state == DSBSTATUS_PLAYING);
}

/*****************************************************************************************/

void decrease_music() {
	if (!play_music) return;
	if (--music_volume < 0) music_volume = 0;	/* ensure that volume stays nonnegative */
	apply_music_vol();
}

/*****************************************************************************************/

void increase_music() {
	if (!play_music) return;
	if (++music_volume > 3) music_volume = 3;	/* ensure that volume stays nonnegative */
	apply_music_vol();
}

/*****************************************************************************************/

void decrease_sound() {
	if (!play_sound) return;
	if (--sound_volume < 0) sound_volume = 0;
	apply_sound_vol();
}

/*****************************************************************************************/

void increase_sound() {
	if (!play_sound) return;
	if (++sound_volume > 3) sound_volume = 3;
	apply_sound_vol();
}

/*****************************************************************************************/

void apply_music_vol() {
	if (!play_music) return;
	if (music_volume < 0 || music_volume > 3)
		music_volume = 2;
	long volume;
	switch(music_volume) {
		case 0: volume = -10000; break;
		case 1: volume = -1400; break;
		case 2: volume = -500; break;
		case 3: volume = 0; break;
	}
	lpDM->SetGlobalParam(GUID_PerfMasterVolume, (LPVOID) &volume, sizeof(long));
}

/*****************************************************************************************/

void apply_sound_vol() {
	if (!play_sound) return;
	if (sound_volume < 0 || sound_volume > 3)
		sound_volume = 2;
	for (int i = 0; i < NUMSOUNDS; i++) {
		switch(sound_volume) {
			case 0: lpSound[i]->SetVolume(-10000); break;
			case 1: lpSound[i]->SetVolume(-1900); break;
			case 2: lpSound[i]->SetVolume(-1000); break;
			case 3: lpSound[i]->SetVolume(0); break;
		}		
	}
}

/*****************************************************************************************/

void show_musicvol() {
	if (!play_music) {
		blit_item(MENU_BEGINX + 119, MENU_BEGINY + 189, VOLUME_COORD);
		return;
	}
	switch (music_volume) {
		case 0: blit_item(MENU_BEGINX + 119, MENU_BEGINY + 189, VOLUME_COORD); break;
		case 1: blit_item(MENU_BEGINX + 203, MENU_BEGINY + 189, VOLUME_COORD); break;
		case 2: blit_item(MENU_BEGINX + 287, MENU_BEGINY + 189, VOLUME_COORD); break;
		case 3: blit_item(MENU_BEGINX + 381, MENU_BEGINY + 189, VOLUME_COORD); break;
	}
}

/*****************************************************************************************/

void show_soundvol() {
	if (!play_sound) {
		blit_item(MENU_BEGINX + 119, MENU_BEGINY + 244, VOLUME_COORD);
		return;
	}
	switch (sound_volume) {
		case 0: blit_item(MENU_BEGINX + 119, MENU_BEGINY + 244, VOLUME_COORD); break;
		case 1: blit_item(MENU_BEGINX + 203, MENU_BEGINY + 244, VOLUME_COORD); break;
		case 2: blit_item(MENU_BEGINX + 287, MENU_BEGINY + 244, VOLUME_COORD); break;
		case 3: blit_item(MENU_BEGINX + 381, MENU_BEGINY + 244, VOLUME_COORD); break;
	}
}

/*****************************************************************************************/

void show_number(int x, int y, DWORD number) {
	static BYTE stack[20];
	int cnt = 0;
	if (number > 0) {
		while (number > 0) {
			stack[cnt++] = (BYTE)(number % 10);
			number /= 10;
		}
	}	else {
		stack[0] = 0;
		cnt = 1;
	}
	for (int i = cnt-1; i >= 0; i--)
		blit_item(x + (cnt - 1 - i) * NUMBER_SIZEX, y, NUMBERS[stack[i]]);
}

/*****************************************************************************************/

void show_text(int x, int y, char* text) {
	for (WORD i = 0; i < strlen(text); i++) {
		if (text[i] < 'A' || text[i] > 'Z')
			continue;		/* show only characters A - Z */
		int lit_ofs = (int) text[i] - 'A';		/* offset of the literal (0 for A, 1 for B....) */
		RECT rect = FIRST_CHAR;
		rect.left += lit_ofs*(CHAR_SIZEX+1);
		rect.right += lit_ofs*(CHAR_SIZEX+1);
		blit_item(x + i*CHAR_SIZEX, y, rect);
	}
}

/*****************************************************************************************/

bool get_text(char* text, int max_len) {
	/* The function is called from Game::record_hs() and is called over and over again
	 until the user enters a text; only a single character per call is registered.
	 It returns true if some text was entered following with the [enter] key, false
	 otherwise. */
	static int cnt = 0;
	GET_KEYBOARD_DATA;
	if (!KEY_DOWN(0)) return false;		/* return if nothing pressed */
	if (data_array[0].dwOfs == DIK_RETURN && cnt == 0) /* nothing actually entered yet */
		return false;
	if (data_array[0].dwOfs == DIK_RETURN) {	/* the only possibility to return true */
		text[cnt] = 0;
		cnt = 0;
		return true;
	}
	else if (data_array[0].dwOfs == DIK_BACK && cnt > 0) {	/* remove one char on backspace */
		text[--cnt] = 0;
		return false;
	}
	if (cnt == max_len) return false;
	switch (data_array[0].dwOfs) {
		case DIK_A: text[cnt++] = 'A'; break; 
		case DIK_B: text[cnt++] = 'B'; break;
		case DIK_C: text[cnt++] = 'C'; break;
		case DIK_D: text[cnt++] = 'D'; break;
		case DIK_E: text[cnt++] = 'E'; break;
		case DIK_F: text[cnt++] = 'F'; break;
		case DIK_G: text[cnt++] = 'G'; break;
		case DIK_H: text[cnt++] = 'H'; break;
		case DIK_I: text[cnt++] = 'I'; break;
		case DIK_J: text[cnt++] = 'J'; break;
		case DIK_K: text[cnt++] = 'K'; break;
		case DIK_L: text[cnt++] = 'L'; break;
		case DIK_M: text[cnt++] = 'M'; break;
		case DIK_N: text[cnt++] = 'N'; break;
		case DIK_O: text[cnt++] = 'O'; break;
		case DIK_P: text[cnt++] = 'P'; break;
		case DIK_Q: text[cnt++] = 'Q'; break;
		case DIK_R: text[cnt++] = 'R'; break;
		case DIK_S: text[cnt++] = 'S'; break;
		case DIK_T: text[cnt++] = 'T'; break;
		case DIK_U: text[cnt++] = 'U'; break;
		case DIK_V: text[cnt++] = 'V'; break;
		case DIK_W: text[cnt++] = 'W'; break;
		case DIK_X: text[cnt++] = 'X'; break;
		case DIK_Y: text[cnt++] = 'Y'; break;
		case DIK_Z: text[cnt++] = 'Z'; break;
		case DIK_SPACE: text[cnt++] = ' '; break;
	}
	text[cnt] = 0;
	return false;
}

/*****************************************************************************************/

void flush_keyboard() {
	GET_KEYBOARD_DATA;
}

/*****************************************************************************************/

unsigned __int64 checksum(const char* fn) {
	DWORD fsize = file_size(fn);
	std::ifstream fin(fn, std::ios::binary);
	if (!fin) {
		error = ERR_MISSINGFILE;
		return 0;
	}
	unsigned char* data = new unsigned char[fsize];
	if (!data) {
		error = ERR_MEMORY;	
		return 0;
	}
	fin.read((char*) data, fsize);
	unsigned __int64 result = 0;
	DWORD j;
	for (DWORD i = 0; i < fsize; i++) {
		j = (i >= 1) ? i - 1: i;
		result += (data[i] + data[j]);
	}
	delete [] data;
	fin.close();
	error = ERR_OK;
	return result;
}

/*****************************************************************************************/

int check_files() {
#define CORRECT 948265450		/* correct checksum */	
	unsigned __int64 sum = 0;
	sum += checksum(PAL_FN);
	if (error != ERR_OK) return error;
	sum += checksum(LVL_FN);
	if (error != ERR_OK) return error;
	sum += checksum(ITEMS_FN);
	if (error != ERR_OK) return error;
	for (int i = 0; i < WATER_FRAMES; i++) {
		char fn[20];
		sprintf(fn, WATER_FN, i+1);
		sum += checksum(fn);
		if (error != ERR_OK) return error;
	}
	for (int i = 0; i < NUMSOUNDS; i++) {
		char fn[20];
		sprintf(fn, SOUND_FN, i+1);
		sum += checksum(fn);
		if (error != ERR_OK) return error;
	}
	for (int i = 0; i < NUMMIDIS; i++) {
		char fn[20];
		sprintf(fn, MUSIC_FN, i+1);
		sum += checksum(fn);
		if (error != ERR_OK) return error;
	}
	if (sum != CORRECT) return (error = ERR_MISSINGFILE);
	return ERR_OK;
}

/*****************************************************************************************/

void fade_in() {
	static PALETTEENTRY tmp[COLORS];
	for (int i = 0; i < COLORS; i++) {
		tmp[i].peRed = 0;
		tmp[i].peGreen = 0;
		tmp[i].peBlue = 0;
		tmp[i].peFlags = PC_NOCOLLAPSE;
	}
	for (int j = 0; j < COLORS; j++) {	
		for (int i = 0; i < COLORS; i++) {
			if (tmp[i].peRed < pal[i].peRed) tmp[i].peRed++;
			if (tmp[i].peGreen < pal[i].peGreen) tmp[i].peGreen++;
			if (tmp[i].peBlue < pal[i].peBlue) tmp[i].peBlue++;
		}
		lpPal->SetEntries(0, 0, COLORS, tmp);
		Sleep(5);
	}
	lpPal->SetEntries(0, 0, COLORS, pal);
}

/*****************************************************************************************/

void fade_out() {
	for (int j = 0; j < COLORS; j++) {	
		for (int i = 0; i < COLORS; i++) {
			if (pal[i].peRed >= 1) pal[i].peRed -= 1;
			if (pal[i].peGreen >= 1) pal[i].peGreen -= 1;
			if (pal[i].peBlue >= 1) pal[i].peBlue -= 1;
		}
		lpPal->SetEntries(0, 0, COLORS, pal);
		Sleep(5);
	}
	check_surf(lpSPrimary);
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	lpSPrimary->Lock(NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL);
	char* surf_mem = (char*) ddsd.lpSurface;
	int pitch = ddsd.lPitch;
	for (int j = 0; j < SCREENY; j++)
		for (int i = 0; i < SCREENX; i++)	
			surf_mem[i+pitch*j] = 0;
	lpSPrimary->Unlock(NULL);
}
