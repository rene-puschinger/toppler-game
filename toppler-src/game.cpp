/*****************************************************************************************/
/* Toppler - game.cpp                                                                    */
/* definitions of classes HighScoreEntry and Game                                        */
/*                                                                                       */
/*****************************************************************************************/

#include "game.h"

Game::Game(HINSTANCE hInstance):
	state(ST_MENU), level(0), score(0), added_attempts(0), attempts(2), show_notice(false)
{
	
	/* check if the constructor of inner class Pond succeeded */
	if (error != ERR_OK) return;

	srand((unsigned)time(NULL));

	CoInitialize(NULL);		/* we must do it because of DirectMusic */

	/* load palette from file */
	if (loadpal(PAL_FN) != ERR_OK) 
		return;

	/* load configuration (ie. high score + sound volumes) from file */
	if (loadcfg(CFG_FN, hs) != ERR_OK)
		return;

	/* create and register window class */
	WNDCLASSEX wnd;	/* the window class */
	wnd.cbSize = sizeof(wnd);
	wnd.style = 0;
	wnd.lpfnWndProc = (WNDPROC) WndProc;
	wnd.cbWndExtra = 0;
	wnd.cbClsExtra = 0;
	wnd.hInstance = hInstance;
	wnd.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wnd.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	wnd.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
	wnd.lpszMenuName = NULL;
	wnd.lpszClassName = WNDCLASSNAME;
	if (!RegisterClassEx(&wnd)) {
		error = ERR_GENERIC;
		return;
	}
	
	/* create the topmost window */
	if (!(hwnd = CreateWindowEx(0, WNDCLASSNAME, APPNAME,	WS_VISIBLE | WS_POPUP,
		0, 0, SCREENX, SCREENY,
		NULL,	NULL,	hInstance, NULL))) {
			error = ERR_GENERIC;
			return;
		}

	/* hide the mouse cursor */
	ShowCursor(FALSE);

	/* create DirectInput object */
	if (FAILED(DirectInputCreate(hInstance, DIRECTINPUT_VERSION, &lpDI, NULL))) {
		error = ERR_DINPUT;
		return;
	}

	/* create keyboard device */
	if (FAILED(lpDI->CreateDevice(GUID_SysKeyboard, &lpKeyboard, NULL))) {
		error = ERR_DINPUT;
		return;
	}

	/* set the keyboard cooperative level */
	if (FAILED(lpKeyboard->SetCooperativeLevel(hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE))) {
		error = ERR_DINPUT;
		return;
	}

	/* set the keyboard data format to the default one */
	if (FAILED(lpKeyboard->SetDataFormat(&c_dfDIKeyboard))) {
		error = ERR_DINPUT;
		return;
	}

	/* set keyboard buffered mode */
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(DIPROPDWORD);
	diprop.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = KEY_ENTRIES;
	if (FAILED(lpKeyboard->SetProperty(DIPROP_BUFFERSIZE, &diprop.diph))) {
		error = ERR_DINPUT;
		return;
	}

	/* acquire the keyboard */
	if (FAILED(lpKeyboard->Acquire())) {
		error = ERR_DINPUT;
		return;
	}

	/* create the DirectSound object */
	play_sound = SUCCEEDED(DirectSoundCreate(NULL, &lpDS, NULL));

	/* set the DirectSound cooperative level */
	play_sound = play_sound && SUCCEEDED(lpDS->SetCooperativeLevel(hwnd, DSSCL_NORMAL));

	/* create sound buffers and load a sound to them */
	DSBUFFERDESC dsbd;
	WAVEFORMATEX wavefmt;
	for (int i = 0; i < NUMSOUNDS; i++)
		lpSound[i] = NULL;
	for (int i = 0; i < NUMSOUNDS; i++) {
		ZeroMemory(&wavefmt, sizeof(wavefmt));
		wavefmt.wFormatTag = WAVE_FORMAT_PCM;		/* we will work only with PCM */
		wavefmt.nChannels = 1;									/* mono */
		wavefmt.nSamplesPerSec = 22050;
		wavefmt.wBitsPerSample = 8;
		wavefmt.nBlockAlign = 1;								/* # of channels * bytes per sample */
		wavefmt.nAvgBytesPerSec = wavefmt.nSamplesPerSec * wavefmt.nBlockAlign;
		wavefmt.cbSize = 0;
		ZeroMemory(&dsbd, sizeof(dsbd));
		dsbd.dwSize = sizeof(dsbd);
		dsbd.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_STATIC;
		char fn[20];
		sprintf(fn, SOUND_FN, i+1);
		dsbd.dwBufferBytes = file_size(fn);
		dsbd.lpwfxFormat = &wavefmt;
		play_sound = play_sound && SUCCEEDED(lpDS->CreateSoundBuffer(&dsbd, &lpSound[i], NULL));
		if (loadsnd(fn, lpSound[i]) != ERR_OK) return;
	}

	/* after having DirectSound created, let's create DirectMusicPerformance object,
	   which also creates DirectMusic object */
	play_music = SUCCEEDED(CoCreateInstance(CLSID_DirectMusicPerformance, NULL, CLSCTX_INPROC,
		IID_IDirectMusicPerformance, (LPVOID*) &lpDM));
	if (play_sound)
		play_music = play_music && SUCCEEDED(lpDM->Init(NULL, lpDS, hwnd));
	else
		play_music = play_music && SUCCEEDED(lpDM->Init(NULL, NULL, hwnd));
	/* add a Microsoft Software Wavetable Synthesizer to the performance */
	play_music = play_music && SUCCEEDED(lpDM->AddPort(NULL));
	/* create the DirectMusic loader to be able to load midi files */
	play_music = play_music && SUCCEEDED(CoCreateInstance(CLSID_DirectMusicLoader, NULL,
		CLSCTX_INPROC, IID_IDirectMusicLoader, (LPVOID*) &lpLoader));

	for (int i = 0; i < NUMMIDIS; i++) {
		lpMusic[i] = NULL;
		lpStates[i] = NULL;
	}
	/* load all midis */
	for (int i = 0; i < NUMMIDIS; i++) {
		char fn[20];
		sprintf(fn, MUSIC_FN, i+1);
		if (loadmus(fn, &lpMusic[i]) != ERR_OK) return;
	}

	/* create the DirectDraw object */
	if (FAILED(DirectDrawCreateEx(NULL, (LPVOID*)&lpDD, IID_IDirectDraw7, NULL))) {
		error = ERR_DDRAWINIT;
		return;
	}

	/* set the DirectDraw cooperative level & display mode */
	if (FAILED(lpDD->SetCooperativeLevel(hwnd, DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE))) {
		error = ERR_DDRAWINIT;
		return;
	}
	if (FAILED(lpDD->SetDisplayMode(SCREENX, SCREENY, BPP, 0, 0))) {
		error = ERR_GRMODE;
		return;
	}

	/* create primary surface and one backbuffer for double buffering */
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
	ddsd.dwBackBufferCount = 1;
	if (FAILED(lpDD->CreateSurface(&ddsd, &lpSPrimary, NULL))) {
		error = ERR_DDRAWGENERIC;
		return;
	}
	DDSCAPS2 ddcaps;
	ZeroMemory(&ddcaps, sizeof(ddcaps));
	ddcaps.dwCaps = DDSCAPS_BACKBUFFER;
	if (FAILED(lpSPrimary->GetAttachedSurface(&ddcaps, &lpSBack))) {
		error = ERR_DDRAWGENERIC;
		return;
	}

	/* create palette and attach it to the primary surface */
	if (FAILED(lpDD->CreatePalette(DDPCAPS_8BIT | DDPCAPS_INITIALIZE, pal, &lpPal, NULL))) {
		error = ERR_DDRAWGENERIC;
		return;
	}
	if (FAILED(lpSPrimary->SetPalette(lpPal))) {
		error = ERR_DDRAWGENERIC;
		return;
	} 

	/* create three screen surfaces from image (water) */
	for (int i = 0; i < WATER_FRAMES; i++)		/* just to be pedant */
		lpSScreen[i] = NULL;
	for (int i = 0; i < WATER_FRAMES; i++) {
		ZeroMemory(&ddsd, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
		ddsd.dwWidth = WATERX;
		ddsd.dwHeight = WATERY;
		if (FAILED(lpDD->CreateSurface(&ddsd, &lpSScreen[i], NULL))) {
			error = ERR_DDRAWGENERIC;
			return;
		}
		char fn[20];
		sprintf(fn, WATER_FN, i+1);
		if (loadpic(fn, lpSScreen[i], WATERX, WATERY) != ERR_OK)
			return;
	}

	/* create items surface from image */
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_CKSRCBLT;
	ddsd.ddckCKSrcBlt.dwColorSpaceLowValue = TRANSCOL;
	ddsd.ddckCKSrcBlt.dwColorSpaceHighValue = TRANSCOL;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwWidth = ITEMSX;
	ddsd.dwHeight = ITEMSY;
	if (FAILED(lpDD->CreateSurface(&ddsd, &lpSItems, NULL))) {
		error = ERR_DDRAWGENERIC;	
		return;
	}
	if (loadpic(ITEMS_FN, lpSItems, ITEMSX, ITEMSY) != ERR_OK)
		return;

	apply_sound_vol();
	apply_music_vol();
}

/*****************************************************************************************/

void Game::inc_attempts() {
	if (attempts < MAXATTEMPTS) {
		stop_snd(SND_NEWLIFE);
		play_snd(SND_NEWLIFE);
		attempts++;
	}
}

/*****************************************************************************************/

void Game::advance() {
	pond.advance(level);	/* first advance the lilies, flies and aphids */
	DWORD old_score = score;
	if (pond.aphid_eaten())
		score += BONUS_APHID;
	int bonus = pond.get_frog_bonus();	/* get the bonus and clear it */
	/* check if the frog takes some bonus */
	if (bonus < 0 && score <= -BONUS_NEG) {
		score = 0;	/* we don't want the score to be negative */
		stop_snd(SND_NEGATIVE);
		play_snd(SND_NEGATIVE);
	}
	else if (bonus != BONUS_ADV) {	/* not an adventure? */
		score += bonus;
		if (bonus < 0) {
			stop_snd(SND_NEGATIVE);		
			play_snd(SND_NEGATIVE);
		}
		else if (bonus == BONUS_SMALL) {
			stop_snd(SND_SMALLBONUS);
			play_snd(SND_SMALLBONUS);
		}
		else if (bonus == BONUS_BIG) {
			stop_snd(SND_BIGBONUS);
			play_snd(SND_BIGBONUS);
		}
	}
	else {	/* randomly determine the type of adventure */
		int adv = rand()%7;
		switch(adv) {
			case 0:		/* positive bonus */
				play_snd(SND_ADVBONUS);
				score += BONUS_ADV;
				break;
			case 1:		/* death */
				pond.make_death(level);
				state = ST_DEATH;
				break;			
			case 2:		/* negative bonus */
				play_snd(SND_NEGATIVE);
				if (score <= -BONUS_ADVNEG) score = 0;
				else score += BONUS_ADVNEG;
				break;
			case 3:		/* add one attempt */
				inc_attempts();
				break;
			case 4:
				play_snd(SND_FLYMOVE);
				pond.create_fly(level);
				break;
			case 5:
				play_snd(SND_APHIDMOVE);
				pond.create_aphid(level);
				break;
			case 6:
				if (pond.exist_fly())
					pond.destroy_fly();	/* get rid of one fly if there is one */
				else
					inc_attempts();			/* if there's fly, add an attempt instead */
				break;
		}
	}
	/* add attempts if gained appropriate score */
	if ((__int64)score/ATTEMPTS_ADDUP - (__int64)old_score/ATTEMPTS_ADDUP >= 1 &&
			added_attempts < score/ATTEMPTS_ADDUP)
	{
		added_attempts++;	
		inc_attempts();
	}
	static int music_check_cnt = 0;
	/* for performance reasons, test if music if playing only every 100th iterations */
	if (music_check_cnt++ == 100) {
		music_check_cnt = 0;
		if (!is_mus_playing())					/* play random song */
			play_mus(rand()%NUMMIDIS);
	}
}

/*****************************************************************************************/

void Game::respond_to_player() {
	GET_KEYBOARD_DATA;
	for (DWORD i = 0; i < entries; i++) {
		if (pond.is_death(level)) {
			state = ST_DEATH;
			return;
		}
		if (pond.is_level_complete()) {
			state = ST_LEVEL_COMPLETE;
			return;
		}
		if (data_array[i].dwOfs == DIK_ESCAPE && KEY_DOWN(i)) {
			flush_keyboard();
			state = ST_MENU;
			return;
		}
		switch(data_array[i].dwOfs) {
			case DIK_DOWN:
				if (KEY_DOWN(i) && pond.possible(DIK_DOWN, level))
					pond.move_frog(DIK_DOWN);
				break;
			case DIK_LEFT:
				if (KEY_DOWN(i) && pond.possible(DIK_LEFT, level))
					pond.move_frog(DIK_LEFT);
				break;
			case DIK_RIGHT:
				if (KEY_DOWN(i) && pond.possible(DIK_RIGHT, level))
					pond.move_frog(DIK_RIGHT);
				break;
			case DIK_UP:
				if (KEY_DOWN(i) && pond.possible(DIK_UP, level))
					pond.move_frog(DIK_UP);
				break;
		}
	}
	/* is the frog staying long time on one lily? */
	state = pond.is_death(level) ? ST_DEATH : ST_PLAYING;
}

/*****************************************************************************************/

void Game::show_status() { 
	static DWORD score_cnt = 0;	/* used for slow increasing/decreasing of score */
	if (score_cnt  < score)
		score_cnt += SCORE_SPEED;	/* in case of increasing score */
	if (score_cnt > score)
		score_cnt -= SCORE_SPEED;	/* in case of decreasing score */
	if (state != ST_PLAYING)		/* when not playing, display score immediately */
		score_cnt = score;
	blit_item(0, SCREENY - STATUSY, STATUS_COORD);
	show_number(100, SCREENY - 27, level+1);
	show_number(637, SCREENY - 27, score_cnt);
	for (int i = 0; i < attempts; i++) {
		blit_item(305 + i*(FROG_SIZEX+9), SCREENY-29, FROG_COORD[DIR_DOWN]);
	}
}

/*****************************************************************************************/

void Game::new_level() {
	srand((unsigned)rand());		/* initialize random number generator */
	play_snd(SND_ENDLEVEL);
	Sleep(1000);
	flush_keyboard();
	score += BONUS_NEWLEVEL;
	if (++level == NUMLEVELS) {
		level--;	
		state = ST_WIN;
	}
	else {
		pond.reset(level);
		state = ST_PLAYING;
	}
}

/*****************************************************************************************/

void Game::new_game() {
	srand((unsigned)rand());		/* initialize random number generator */
	level = 0;
	score = 0;
	added_attempts = 0;
	attempts = 2;
	pond.reset(level);
	show_status();
	state = ST_PLAYING;
	flush_keyboard();
}

/*****************************************************************************************/

bool Game::new_attempt() {
	flush_keyboard();
	if (attempts-- == 0)
		return false;
	pond.reset(level);
	state = ST_PLAYING;
	return true;
}

/*****************************************************************************************/

void Game::render() {
	/* now blit the pond, how it should appear on the screen */
	pond.render(level);

	/* show level, lives, score etc. */
	show_status();

	/* flip everything to the primary surface */
	FLIP;
}

/*****************************************************************************************/

void Game::show_hs() {
#define DIGITS(n) (((int) floor(log10((double) n))) + 1)	/* compute number of digits */
	state = ST_VIEW_HS;
	pond.blit_bg();
	blit_item(HS_BEGINX, HS_BEGINY, HS_COORD);
	for (DWORD i = 0; i < NUM_NAMES; i++) {
		show_number(HS_BEGINX + 17, HS_BEGINY + 50 + 21*(i+1), i+1);
		show_text(HS_BEGINX + 62, HS_BEGINY + 50 + 21*(i+1), hs[i].name);
		/* delta is simply used for right side align of scores - it looks better */
		int delta = (NUMBER_SIZEX)*(9 - DIGITS(hs[i].score));
		show_number(HS_BEGINX + 240 + delta, HS_BEGINY + 50 + 21*(i+1), hs[i].score);
	}
	GET_KEYBOARD_DATA;
	if ((data_array[0].dwOfs == DIK_ESCAPE || data_array[0].dwOfs == DIK_RETURN) && KEY_DOWN(0)) {
		flush_keyboard();
		state = ST_MENU;
	}
	show_status();
	FLIP;
	if (!is_mus_playing())					/* play random song */
		play_mus(rand()%NUMMIDIS);
}

/*****************************************************************************************/

void Game::show_win() {
	/* play the sound only once */
	static bool snd_playing = false;
	if (!snd_playing) {
		play_snd(SND_WIN);
		snd_playing = true;
	}
	state = ST_WIN;
	pond.blit_bg();
	blit_item(WIN_BEGINX, WIN_BEGINY, WIN_COORD);
	show_status();
	/* show a stupid animation */
	static int fly_frame = 0;
	static int fly_frame_cnt = 0;
	if ((fly_frame_cnt+=2) >= ANIM) {
		fly_frame_cnt = 0;
		fly_frame++;
		fly_frame %= FLY_FRAMES;
	}
	blit_item(WIN_BEGINX+15, WIN_BEGINY+90, FLY_COORD[fly_frame]);
	blit_item(WIN_BEGINX+65, WIN_BEGINY+90, FLY_COORD[fly_frame]);
	blit_item(WIN_BEGINX+340, WIN_BEGINY+90, FLY_COORD[fly_frame]);
	blit_item(WIN_BEGINX+390, WIN_BEGINY+90, FLY_COORD[fly_frame]);
	FLIP;
	GET_KEYBOARD_DATA;
	if ((data_array[0].dwOfs == DIK_ESCAPE || data_array[0].dwOfs == DIK_RETURN) && KEY_DOWN(0)) {
		flush_keyboard();	
		snd_playing = false;
		state = ST_RECORD;
	}
	if (!is_mus_playing())					/* play random song */
		play_mus(rand()%NUMMIDIS);
}

/*****************************************************************************************/

void Game::record_hs() { 
	static bool computed = false;
	static bool entered = false;
	static int i;
	state = ST_RECORD;
	if (!computed && score <= hs[NUM_NAMES-1].score) {
		state = ST_VIEW_HS;
		return;	/* nothing to record */
	}
	static bool snd_playing = false;
	if (!snd_playing) {
		play_snd(SND_RECORD);
		snd_playing = true;
	}
	if (!computed) {		/* compute the index i to hs[] only first time */
		for (i = 0; i < NUM_NAMES && hs[i].score >= score; i++);
		for (int j = NUM_NAMES-1; j > i; j--) {
			hs[j].score = hs[j-1].score;
			strcpy(hs[j].name, hs[j-1].name);
		}
		strcpy(hs[i].name, "");
		hs[i].score = score;
		computed = true;
	}
	pond.blit_bg();
	blit_item(RECORD_BEGINX, RECORD_BEGINY, RECORD_COORD);
	show_status();
	entered = get_text(hs[i].name, MAX_NAME_LEN);
	show_text(RECORD_BEGINX + 202, RECORD_BEGINY + 27, hs[i].name);
	FLIP;
	if (entered) {
		computed = false;
		Sleep(200);
		savecfg(CFG_FN, hs);
		snd_playing = false;
		state = ST_VIEW_HS;
	}
	if (!is_mus_playing())					/* play random song */
		play_mus(rand()%NUMMIDIS);
}

/*****************************************************************************************/

void Game::show_menu() {
#define MENU_ITEM_NEW 0			/* new game */
#define MENU_ITEM_HS 1			/* high score */
#define MENU_ITEM_MUSIC 2		/* music volume */
#define MENU_ITEM_SOUND 3		/* sound volume */
#define MENU_ITEM_EXIT 4		/* exit game */
	static int active_item = 0;		/* active menu item */
	static bool fading = true;	/* true initially when performing screen fade in */
	if (!is_mus_playing())					/* play random song */
		play_mus(rand()%NUMMIDIS);
	state = ST_MENU;
	pond.blit_bg();
	blit_item(MENU_BEGINX, MENU_BEGINY, MENU_COORD);
	if (show_notice)
		blit_item(0, 0, NOTICE_COORD);
	DWORD ballx1;	DWORD bally1;	DWORD ballx2;	DWORD bally2;
	switch (active_item) {
		case MENU_ITEM_NEW: ballx1 = 155; bally1 = 104; ballx2 = 332; break;
		case MENU_ITEM_HS: ballx1 = 145; bally1 = 132; ballx2 = 338; break;
		case MENU_ITEM_MUSIC: ballx1 = 130; bally1 = 162; ballx2 = 355; break;
		case MENU_ITEM_SOUND: ballx1 = 126; bally1 = 215; ballx2 = 357; break;
		case MENU_ITEM_EXIT: ballx1 = 191; bally1 = 269; ballx2 = 290; break;
	}
	bally2 = bally1;
	blit_item(MENU_BEGINX + ballx1, MENU_BEGINY + bally1, BALL_COORD);		/* blit balls */
	blit_item(MENU_BEGINX + ballx2, MENU_BEGINY + bally2, BALL_COORD);
	show_status();
	show_musicvol();
	show_soundvol();
	FLIP;
	if (fading) {
		fade_in();
		fading = false;
	}
	GET_KEYBOARD_DATA;
	if (KEY_DOWN(0)) {		/* just read the first keypress */
		switch(data_array[0].dwOfs) {
			case DIK_DOWN:
				play_snd(SND_MENUCLICK);
				active_item++;
				active_item %= MENU_NUMITEMS;
				break;
			case DIK_LEFT: 
				if (active_item == MENU_ITEM_MUSIC)
					decrease_music();
				if (active_item == MENU_ITEM_SOUND) {
					decrease_sound();
					play_snd(SND_MENUCLICK);
				}
				break;
			case DIK_RIGHT:
				if (active_item == MENU_ITEM_MUSIC)
					increase_music();
				if (active_item == MENU_ITEM_SOUND) {
					increase_sound();
					play_snd(SND_MENUCLICK);
				}
				break;
			case DIK_UP:
				play_snd(SND_MENUCLICK);
				active_item--;
				if (active_item < 0) active_item = MENU_NUMITEMS-1;
				break;
			case DIK_ESCAPE:
				flush_keyboard();
				if (show_notice) state = ST_PLAYING;	/* close the menu (only when playing game) */
				break;
			case DIK_RETURN:
				if (active_item == MENU_ITEM_NEW) new_game();
				else if (active_item == MENU_ITEM_EXIT) state = ST_QUIT;
				else if (active_item == MENU_ITEM_HS) state = ST_VIEW_HS;
				break;
		}
	}
}

/*****************************************************************************************/

void Game::run() {
	Sleep(500);	/* wait until the resolution is set */
	/* start with the game menu */
	show_menu();
	/* message to be continuously updated */
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT) {			/* main program loop */
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		LOCK_FPS_PROLOGUE;
		Sleep(5);
		switch (state) {
			case ST_PLAYING:
				show_notice = true;		/* now when user displays menu, show also the notice about closing */
				respond_to_player();	/* check user input if any and make reaction */
				if (state == ST_PLAYING) {
					advance();
					render();
				}
				break;
			case ST_DEATH:
				if (!new_attempt()) {	/* there is no attempt left */
					show_notice = false;
					flush_keyboard();
					record_hs();
				}
				break;
			case ST_LEVEL_COMPLETE:
				show_status();
				new_level();
				break;
			case ST_MENU:
				show_menu();
				break;
			case ST_VIEW_HS:
				show_hs();
				break;
			case ST_RECORD:
				record_hs();
				break;
			case ST_WIN:
				show_notice = false;
				show_win();
				break;
			case ST_QUIT:
				savecfg(CFG_FN, hs);
				play_snd(SND_EXIT);
				decrease_music();
				fade_out();
				if (play_sound) Sleep(600);
				return;
		}
		LOCK_FPS_EPILOGUE;	/* lock the frame rate */
	}
}

/*****************************************************************************************/

Game::~Game() {
	if (lpDD)
		lpDD->SetCooperativeLevel(hwnd, DDSCL_NORMAL);
	DXRELEASE(lpSItems);
	for (int i = WATER_FRAMES-1; i >= 0; i--) 
		DXRELEASE(lpSScreen[i]);
	DXRELEASE(lpSBack);
	DXRELEASE(lpSPrimary);
	DXRELEASE(lpDD);
	for (int i = NUMMIDIS-1; i >= 0; i--) {
		if (lpMusic[i]) lpMusic[i]->SetParam(GUID_Unload, -1, 0, 0, (LPVOID) lpDM);
		DXRELEASE(lpMusic[i]);
	}
	DXRELEASE(lpLoader);
	if (lpDM) {
		lpDM->CloseDown();
		DXRELEASE(lpDM);
	}
	for (int i = NUMSOUNDS-1; i >= 0; i--) 	
		DXRELEASE(lpSound[i]);
	DXRELEASE(lpDS);
	if (lpKeyboard)
		lpKeyboard->Unacquire();
	DXRELEASE(lpKeyboard);
	DXRELEASE(lpDI);
	CoUninitialize();
}

/*****************************************************************************************/