/*****************************************************************************************/
/* Toppler - globals.h                                                                   */
/* structrues LevelInfo, Fly, Aphid, Lily and HighScoreEntry,                            */
/* constants, macros, global variables, item coordinates and global functions decl.      */
/*                                                                                       */
/*****************************************************************************************/

#ifndef GLOBALS_H
#define GLOBALS_H

#define DIRECTINPUT_VERSION 0x0700		/* use DirectInput version 7 */
#include <fstream>
#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include <dinput.h>
#include <mmreg.h>
#include <dsound.h>
#include <dmksctrl.h>
#include <dmusici.h>
#include <dmusicc.h>
#include <dmusicf.h>
#include <cmath>
#include <ctime>


#define WNDCLASSNAME "TOPPLERCLASS"	/* window class name */
#define APPNAME "Toppler"						/* application name */

#define ST_QUIT 1										/* user wish to leave the game */
#define ST_PLAYING 2								/* user plays the game */
#define ST_DEATH 3									/* frog just died *sigh* */
#define ST_LEVEL_COMPLETE 4					/* level completed */
#define ST_MENU 5										/* user is in the menu */
#define ST_VIEW_HS 6								/* viewing high score */
#define ST_RECORD 7									/* record to high score */
#define ST_WIN 8										/* the game is won */

#define CFG_FN "toppler.cfg"				/* high score file name */
#define PAL_FN "toppler.pal"				/* global palette file name */
#define LVL_FN "toppler.lvl"				/* level info file name */
#define WATER_FN "water%i.dat"			/* water background file names */
#define ITEMS_FN "items.dat"				/* items file name */
#define SOUND_FN "toppler%i.snd"		/* sound file names */
#define MUSIC_FN "toppler%i.mus"		/* music file names */

#define KEY_ENTRIES 20			/* number of entries in the keyboard data buffer */

/* following defines are used for error handling */
#define ERR_OK 0						/* no error */
#define ERR_GENERIC 1				/* general error */
#define ERR_MEMORY 2				/* not enough memory */
#define ERR_MISSINGFILE 3		/* some file corrupted or not present */
#define ERR_DDRAWINIT	4			/* DirectDraw initialization error */
#define ERR_DDRAWGENERIC 5	/* general DirectDraw error */
#define ERR_GRMODE 6				/* graphic mode can't be set */
#define ERR_DINPUT 7				/* DirectInput failure */

#define SCREENX 800				/* screen X resolution */
#define SCREENY 600				/* screen Y resolution */
#define WATERX 800				/* water X resolution */
#define WATERY 560				/* water Y resolution */
#define ITEMSX 881				/* items surface X resolution */
#define ITEMSY 1274				/* items surface Y resolution */
#define MENU_BEGINX	151		/* position of the menu */
#define MENU_BEGINY 80
#define HS_BEGINX 210			/* position of high score bar */
#define HS_BEGINY 80
#define RECORD_BEGINX 190	/* position of the record-to-high-score bar */
#define RECORD_BEGINY 200
#define WIN_BEGINX 177		/* position of the win message */
#define WIN_BEGINY 160
#define MENU_NUMITEMS 5		/* number of items in the menu */
#define STATUSY 40				/* y size of the status bar */
#define BPP 8							/* color depth, 8 => 256 colors, 16 => 65536 colors, etc. */
#define COLORS (1 << BPP)	/* number of colors */
#define FPS 40						/* frame rate */

/* measure of water slowness - the higher the value, the slower animation */
#define WATER_SLOWNESS 30
#define WATER_FRAMES 3			/* we use 3 succesive frames for water animation */

#define LAST_MOVEX 1			/* last fly movement was in x direction (used for better path search) */
#define LAST_MOVEY 2			/* last fly movement was in y direction */

#define APHID_FRAMES 9		/* number of frames for aphid animation */
#define FLY_FRAMES 10			/* number of frames for big fly animation */
#define ANIM 70						/* used for fly and aphid animation - see Pond::render() */

#define TRANSCOL 0xff			/* transparent color in the surface of items */

#define FIELDCOLS	19				/* number of columns in playing field */
#define FIELDROWS	18				/* number of rows */
#define NUMLEVELS 25				/* number of levels */
#define NUMLILIES 20				/* number of water lilies */
#define NUMRIPPLES 2				/* number of ending ripples */
/* how fast to increase the score (which should be a multiple of this constant) */
#define SCORE_SPEED 50
/* the maximal status a lily can have, if it is exactly this value, the lily is not visible */
#define MAXSTATUS 1000000		
#define STAT_DENSITY (MAXSTATUS/(NUMLILIES+NUMRIPPLES))
#define MAXANIMAL_STATUS 20000	/* used for fly & aphid speed manipulation */
#define MAXFLIES 10							/* maximal number of flies in one level */
#define MAXAPHIDS 10						/* maximal number of aphids in one level */
#define MINANIMAL_SPEED 70			/* minimal fly & aphid speed */
#define MAXANIMAL_SPEED 500			/* maximal fly & aphid speed */
#define MAXATTEMPTS 8						/* maximal number of attempts a frog can possibly have */
#define CHANGE_END_PERIOD	1400	/* how often possibly change the level ending lily */
#define CHANGE_END_MINLEV 9			/* minimal level where the ending can be possibly changed */
#define LILY_SIZEX 173					/* x size of a lily (with box) */
#define LILY_SIZEY 110					/* y size of a lily */
#define FROG_SIZEX 20						/* frog size */
#define FROG_SIZEY 24
#define NUMBER_SIZEX 13					/* size of a number literal */
#define BONUS_RAD	6							/* radius of each bonus circle */
#define STAR_SIZEX 30						/* size of ending level star */
#define STAR_SIZEY 27
#define FLY_SIZEX 33
#define FLY_SIZEY 29
#define APHID_SIZEX 20
#define APHID_SIZEY 21
#define CROSS_SIZEX 17
#define CROSS_SIZEY 30
#define CHAR_SIZEX 15
#define MAX_NAME_LEN 11					/* maximal player name length (to high score) */
#define NUM_NAMES 10						/* number of names in high score */
#define BONUS_NEWLEVEL 1000
#define BONUS_SMALL 300				/* small bonus */
#define BONUS_BIG	2500				/* big bonus */
#define BONUS_NEG (-1000)			/* negative "bonus" */
#define BONUS_ADV 5500				/* adventure bonus */
#define BONUS_ADVNEG (-5000)	/* unhappy adventure bonus */
#define BONUS_APHID 7000			/* aphid bonus gain */
#define ATTEMPTS_ADDUP 40000	/* add one attempt after reaching score of multiple of this value */
/* following are the frog's possible directions */
#define DIR_DOWN 0
#define DIR_LEFT 1
#define DIR_RIGHT 2
#define DIR_UP 3
/* minimal status a lily must have in order to move fly or aphid to it */
#define TRESHOLD (NUMLILIES-2)*STAT_DENSITY

#define NUMSOUNDS 17
#define SND_FLYMOVE 0
#define SND_FLYEATFROG 1
#define SND_DEATH 2
#define SND_NEWLIFE 3
#define SND_ADVBONUS 4
#define SND_APHIDMOVE 5
#define SND_MENUCLICK 6
#define SND_SMALLBONUS 7
#define SND_NEGATIVE 8
#define SND_FROGMOVE 9
#define SND_RECORD 10
#define SND_BIGBONUS 11
#define SND_EXIT 12
#define SND_WIN 13
#define SND_FLYDEATH 14
#define SND_APHIDEATEN 15
#define SND_ENDLEVEL 16

#define NUMMIDIS 6

/*****************************************************************************************/

/* release DirectX interface */
#define DXRELEASE(x) if(x) x->Release();

/* folowing macro gets buffered keyboard input */
#define GET_KEYBOARD_DATA DIDEVICEOBJECTDATA data_array[KEY_ENTRIES]; \
	DWORD entries = KEY_ENTRIES; \
	while (lpKeyboard->Acquire() == DIERR_INPUTLOST);	\
	lpKeyboard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), data_array, &entries, 0); 

/* flip backbuffer to primary surface */
#define FLIP check_surf(lpSBack); \
	check_surf(lpSPrimary); \
	lpSPrimary->Flip(NULL, DDFLIP_WAIT);

/* begin measuring ticks */
#define LOCK_FPS_PROLOGUE DWORD start_time = GetTickCount();
/* lock frame rate to # FPS */
#define LOCK_FPS_EPILOGUE while ((GetTickCount() - start_time) < (1/FPS)*1000) Sleep(1);
/* is a key with scan code (DIK_XXX) pressed? */
#define KEY_DOWN(i) ((data_array[i].dwData & 0x80) != 0)	

/*****************************************************************************************/

/* info about each level, to be read from file */
struct LevelInfo {
	BYTE cols;						/* number of used columns */
	BYTE rows;						/* number of used rows */
	BYTE start_lily;			/* index of the starting lily */
	BYTE xspacing;				/* number of pixels as a distance between each two lilies */
	BYTE yspacing;
	BYTE xtrim;						/* number of pixels to trim from left and right side of the lily */
	BYTE ytrim;						/* number of pixels to trim from upper and down side of the lily */
	BYTE xoffset;					/* offset of the left-topmost lily on the screen */
	BYTE yoffset;
	WORD min_decay;				/* minimal decay of lilies */
	WORD decay_spread;		/* maximal value to add to decay */
	WORD start_lily_decay;/* decay of the starting lily */
	WORD delay_start_min;	/* minimal value of the delay before lily is visible */
	WORD delay_start_max;	/* maximal value of the delay before lily is visible */
	BYTE bonus_small;			/* probability (in %) of small bonus */
	BYTE bonus_neg;				/* etc. */
	BYTE bonus_adv;
	BYTE bonus_big;
	BYTE fly;							/* probability (in %) of fly creation */
	BYTE maxflies;				/* maximal number of flies */
	BYTE aphid;						/* probability (in %) of aphid creation */
	BYTE maxaphids;				/* maximal number of aphids */
};

/* represents an opponent fly */
struct Fly {
	bool alive;						/* true if the fly is alive */
	WORD speed;						/* speed of movement */
	WORD cnt;							/* counter of iterations */
	int posx;							/* position in the pond */
	int posy;
	int last_move;				/* used for smarter path finding, see Pond::move_fly() */
};

/* represents an aphid which can be eaten to increase score */
struct Aphid {
	bool alive;
	WORD speed;
	WORD cnt;
	int posx;
	int posy;
};

/* water lily with bonuses etc. */
struct Lily {
	/* how big or small the lily is, smaller values indicate more recently created lily
	 and conversely, larger values (up to MAXSTATUS) represent smaller lily */
	DWORD status;
	WORD decay;							/* how fast the lily decays */
	int bonus;							/* optional bonus or adventure (not including aphid) */
	WORD delay_start;				/* how many cycles to delay before the lily is created */
	WORD delay_cnt;					/* counter of cycles for delay_start */
};

/* entry in high score */
struct HighScoreEntry {
	char name[MAX_NAME_LEN+1];		/* including the ending zero */
	DWORD score;
};

/*****************************************************************************************/
/* following variables describe each object (ie. lily, cross, bonus) in terms */
/* of their coordinates on the lpSItems surface - see globals.cpp */

extern RECT LILIES_COORD[NUMLILIES+NUMRIPPLES];			/* lilies (incl. ending ripples) */
extern RECT STAR_COORD;															/* red star (level end) */
extern RECT BONUS_SMALL_COORD;											/* small bonus (blue) */
extern RECT BONUS_BIG_COORD;												/* big bonus (orange) */
extern RECT BONUS_ADV_COORD;												/* adventure (grey) */
extern RECT BONUS_NEG_COORD;												/* negative 'bonus' (brown square) */
extern RECT CROSS_COORD;														/* cross (death) */
extern RECT FROG_COORD[];														/* frog - up to down, right to left, etc. */
extern RECT FLY_COORD[];														/* big fly */
extern RECT APHID_COORD[];													/* small aphid */
extern RECT STATUS_COORD;														/* status bar */
extern RECT NUMBERS[10];														/* numbers 0 - 9 */
extern RECT FIRST_CHAR;															/* coords of the first character literal */
extern RECT MENU_COORD;															/* main menu */
extern RECT HS_COORD;																/* high score */
extern RECT RECORD_COORD;														/* record to high score */
extern RECT NOTICE_COORD;														/* notice how to close the menu */
extern RECT WIN_COORD;															/* game-won notice */
extern RECT BALL_COORD;															/* ball used in menu */
extern RECT VOLUME_COORD;														/* sound/music volume indicator */

/*****************************************************************************************/

extern int error;						/* variable for tracking errors */
extern char* error_msg[];		/* error messages */

/* the application handle, initialised by Game::Game() */
extern HWND hwnd;
extern LPDIRECTDRAW7 lpDD;											/* DirectDraw object */
extern LPDIRECTDRAWPALETTE lpPal;								/* the one and only palette */
extern LPDIRECTDRAWSURFACE7 lpSPrimary;					/* primary surface */
extern LPDIRECTDRAWSURFACE7 lpSBack;						/* back buffer surface */
extern LPDIRECTDRAWSURFACE7 lpSItems;						/* all items - water lilies, frog, etc. */
extern LPDIRECTDRAWSURFACE7 lpSScreen[WATER_FRAMES];	/* main screen (water) */
extern LPDIRECTINPUT lpDI;											/* DirectInput object */
extern LPDIRECTINPUTDEVICE lpKeyboard;
extern LPDIRECTSOUND lpDS;											/* DirectSound object */
extern LPDIRECTSOUNDBUFFER lpSound[NUMSOUNDS];	/* buffers for all the sounds */
extern IDirectMusicPerformance* lpDM;						/* DirectMusic performance */
extern IDirectMusicLoader* lpLoader;						/* DirectMusic loader (for loading MIDI files) */
extern IDirectMusicSegment* lpMusic[NUMMIDIS];		/* Segments representing all the midis */
extern IDirectMusicSegmentState* lpStates[NUMMIDIS];	/* Segment states */

extern PALETTEENTRY pal[COLORS];							/* palette structure, to be filled from file */
extern bool play_sound;												/* true if sound supported */
extern int sound_volume;											/* can be 0..3. now set to highest */
extern bool play_music;												/* true if music supported */
extern int music_volume;											/* can be 0..3. now set to highest */

/*****************************************************************************************/

/* Window Procedure */
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
/* get file size, called before file opened */
DWORD file_size(const char* fn);
/* blit item specified by its coords from lpSItems to [x,y] on lpSBack */
void blit_item(DWORD x, DWORD y, RECT& src_coords);
/* restore lost surface */
void check_surf(LPDIRECTDRAWSURFACE7 surf);
/* load LevelInfo from fn */
int loadlvl(const char* fn, LevelInfo* levinfo);
/* load palette from fn, return error code */
int loadpal(const char* fn);
/* load picture to surface, return err.code */
int loadpic(const char* fn, LPDIRECTDRAWSURFACE7 surf, int szx, int szy);	
/* loads the config file */
int loadcfg(const char* fn, HighScoreEntry* hs);
/* save the config file */
void savecfg(const char* fn, HighScoreEntry* hs);
/* load a midi to a segment */
int loadmus(const char* fn, IDirectMusicSegment** seg); 
/* load sound to a buffer */
int loadsnd(const char* fn, LPDIRECTSOUNDBUFFER buf);
/* play music according to MUS_XXX */
void play_mus(int mus_number);
/* stop music playing */
void stop_mus(int mus_number);
/* determines whether music is playing */
bool is_mus_playing(int mus_number);
/* same as above except it checks all music segments */
bool is_mus_playing();
/* play sound according to a number of type SND_XXX */
void play_snd(int snd_number);
/* stop sound playing */
void stop_snd(int snd_number);
/* determines whether sound is playing */
bool is_snd_playing(int snd_number);
/* decrease music volume */
void decrease_music();
/* increase music volume */
void increase_music();
/* decrease sound volume */
void decrease_sound();
/* increase sound volume */
void increase_sound();
/* apply the music volume specified by music_volume */
void apply_music_vol();
/* apply the sound volume specified by sound_volume */
void apply_sound_vol();
/* show music volume indicator */
void show_musicvol();
/* show sound volume indicator on the menu */
void show_soundvol();
/* write a number to backbuffer */
void show_number(int x, int y, DWORD number);
/* write a text to backbuffer */
void show_text(int x, int y, char* text);
/* get text input, return true if pressed ENTER */
bool get_text(char* text, int max_len);
/* flush keyboard buffer */
void flush_keyboard();
/* return the sum of all characters in a file */
unsigned __int64 checksum(const char* fn);
/* check if all files are correct, except for CFG_FN, which varies */
int check_files();
/* fade the screen in (when starting the game) */
void fade_in();
/* fade the screen out (used when exiting) */
void fade_out();

/*****************************************************************************************/

#endif