/*****************************************************************************************/
/* Toppler - pond.h                                                                      */
/* declaration of class Pond                                                             */
/*                                                                                       */
/*****************************************************************************************/

#ifndef POND_H
#define POND_H

#include "globals.h"

/*****************************************************************************************/

/* the playing field - pond with frog, flies and aphids */
class Pond {
	Lily field[FIELDCOLS][FIELDROWS];	/* playing field,in lover levels only a subset is used */
	LevelInfo levels[NUMLEVELS];			/* detailed info about each level */
	Fly flies[MAXFLIES];							/* the opponents */
	Aphid aphids[MAXAPHIDS];					/* the aphids (moving bonus) */
	int fly_cnt;											/* counter of the flies */
	int aphid_cnt;
	int frog_posx;										/* position of the frog */
	int frog_posy;
	int adv_posx;											/* position of the adventure bonus (-1 if not present) */
	int adv_posy;
	int frog_direction;								/* direction of frog, set to DIR_XXX */
	int start_posx;										/* starting point of the frog */
	int start_posy;
	int end_posx;											/* position of the end-level star (may vary) */
	int end_posy;
	/* check if fly at an index died and take appropriate action */
	void check_fly_death(int index, int level);
	int dist(int x, int y);						/* return distance of frog from lily at x,y  */
	/* move fly to the next lily according to its intelligence */
	void move_fly(int index, int level);
	void check_aphid_death(int index, int level);	/* check if aphid is dead, not including eating */
	void move_aphid(int index, int level);
public:
	Pond();
	void blit_bg();										/* blit the background - moving water */
	/* test if the movement to a new location, given by the virtual key code is possible */	
	bool possible(BYTE vk_code, int level);
	void move_frog(BYTE vk_code);			/* move frog to new location according to vk_code */
	void create_aphid(int level);			/* create one aphid */
	void create_fly(int level);				/* create one fly */
	bool aphid_eaten();								/* true if the frog has eaten some aphid */
	bool is_death(int level);					/* check if the frog is dead */
	bool is_level_complete();					/* check if level finished */
	void make_death(int level);				/* make the frog dead when taking death adventure */
	bool exist_fly();									/* check if there is some fly */
	void destroy_fly();								/* destroy one fly (in case of appropriate adventure) */
	void reset(int level);						/* resets the pond for given level */
	/* blit a lily from field[i][j] to the backbuffer surface */
	void blit_lily(int i, int j, int index, int level);
	/* blit the lilies to backbuffer, if display_frog == false, don't show the frog */
	void render(int level, bool display_frog = true);
	/* reduce the size of all lilies according to Lily::decay, create new lilies
	according to the probability PROB_CREATE, move the opponent, etc. return true
	if the frog is alive, false if dead */
	void advance(int level);
	int get_frog_bonus();							/* return the bonus at frog position and clear it */
};

#endif