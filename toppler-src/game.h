/*****************************************************************************************/
/* Toppler - game.h                                                                      */
/* declaration of class Game                                                             */
/*                                                                                       */
/*****************************************************************************************/

#ifndef GAME_H
#define GAME_H

#include "globals.h"
#include "pond.h"

/* the game class - water, lilies, score, etc. */
class Game {
	int state;												/* actual state of the game, set to ST_XXX */
	HighScoreEntry hs[NUM_NAMES];			/* high score */
	Pond pond;
	int level;												/* game level (0..NUMLEVELS-1) */
	int attempts;											/* number of lives or attempts */
	DWORD score;											/* game score */
	DWORD added_attempts;								/* attempts added for score, not including adventure */
	bool show_notice;									/* see show_menu() decl. */
	void show_status();								/* display level, score etc. */
	void respond_to_player();					/* check player action and respond to it */
	void new_game();									/* begin new game from the first level */
	void new_level();									/* begin new level */
	/* attempt to play actual level again, if there's no attempt left, return false */
	bool new_attempt();
	void render();										/* convert the contents of game to the screen */
	void inc_attempts();							/* increase number of attempts by 1 */
	void advance();										/* move objects */
	void show_hs();										/* show high score */
	void show_win();									/* show the win message */
	void record_hs();									/* record to high score if possible */
	/* show main menu, if show_notice true, display also a notice how to close the menu */
	void show_menu();
public:
	Game(HINSTANCE hInstance);				/* init everything */
	void run();												/* the main loop, return false when exiting */
	~Game();													/* destroy everything */
};

#endif