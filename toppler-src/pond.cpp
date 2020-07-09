/*****************************************************************************************/
/* Toppler - pond.cpp                                                                    */
/* definition of class Pond                                                              */
/*                                                                                       */
/*****************************************************************************************/

#include "pond.h"

/*****************************************************************************************/

Pond::Pond():
	frog_posx(0), frog_posy(0), start_posx(0), start_posy(0), end_posx(0), end_posy(0),
	frog_direction(0), fly_cnt(0), adv_posx(-1), adv_posy(-1)
{
	if (check_files() != ERR_OK)
		return;
	
	/* load level info from file */
	if (loadlvl(LVL_FN, levels) != ERR_OK)
		return;
}

/*****************************************************************************************/

void Pond::create_aphid(int level) {
	if (aphid_cnt == MAXAPHIDS)
		return;
	aphids[aphid_cnt].alive = true;
	aphids[aphid_cnt].speed = rand()%(MAXANIMAL_SPEED-MINANIMAL_SPEED) + MINANIMAL_SPEED;
	aphids[aphid_cnt].cnt = 0;
	/* generate aphid's coordinates, but not too close to the frog */
	aphids[aphid_cnt].posx = rand()%(levels[level].cols - 4) + 4;
	aphids[aphid_cnt].posy = rand()%(levels[level].rows - 4);
	field[aphids[aphid_cnt].posx][aphids[aphid_cnt].posy].status = levels[level].start_lily * STAT_DENSITY;
	aphid_cnt++;
}

/*****************************************************************************************/

void Pond::create_fly(int level) {
	if (fly_cnt == MAXFLIES)
		return;
	flies[fly_cnt].alive = true;
	flies[fly_cnt].speed = rand()%(MAXANIMAL_SPEED-MINANIMAL_SPEED) + MINANIMAL_SPEED;
	flies[fly_cnt].cnt = 0;
	flies[fly_cnt].last_move = LAST_MOVEY;
	if (rand()%100 < 80)	/* 80 % chance that the fly will start in right corner */
		flies[fly_cnt].posx = levels[level].cols-1;
	else									/* 20 % chance that the fly will start in left corner */
		flies[fly_cnt].posx = 0;
	/* similarly set the y-position */
	if (flies[fly_cnt].posx == levels[level].cols-1 && rand()%100 < 50) {
		flies[fly_cnt].posy = levels[level].rows-1;
		flies[fly_cnt].last_move = LAST_MOVEX;
	}
	else flies[fly_cnt].posy = 0;
	/* now set properly the attributes of the lily where last fly was created */
	field[flies[fly_cnt].posx][flies[fly_cnt].posy].status = levels[level].start_lily * STAT_DENSITY;
	field[flies[fly_cnt].posx][flies[fly_cnt].posy].decay = levels[level].start_lily_decay;
	fly_cnt++;
}

/*****************************************************************************************/

void Pond::check_fly_death(int index, int level) {
	const int x = flies[index].posx;
	const int y = flies[index].posy;
	if (field[x][y].status >= NUMLILIES*STAT_DENSITY) {
    play_snd(SND_FLYDEATH);
		flies[index].alive = false;
		if (adv_posx != -1)
			field[adv_posx][adv_posy].bonus = 0;	/* clear adventure bonus, if any */
		field[x][y].bonus = BONUS_ADV;	/* set new adventure on former fly's position */
		field[x][y].status = levels[level].start_lily * STAT_DENSITY;
		adv_posx = x;
		adv_posy = y;
	}
}

/*****************************************************************************************/

int Pond::dist(int x, int y) {
	return (frog_posx - x)*(frog_posx - x) + (frog_posy - y)*(frog_posy - y);
}

/*****************************************************************************************/

void Pond::move_fly(int index, int level) {
	const int x = flies[index].posx;
	const int y = flies[index].posy;
	/* first try to find a move with a lame method */
	int newx = x;
	int newy = y;
	int bestdist = FIELDROWS*FIELDROWS+FIELDCOLS*FIELDCOLS;
	if (x > 0 && dist(x-1, y) < bestdist && field[x-1][y].status < TRESHOLD) {
		newx = x-1; newy = y;
		bestdist = dist(newx, newy);
	}
	if (x < levels[level].cols-1 && dist(x+1, y) < bestdist && field[x+1][y].status < TRESHOLD) {
		newx = x+1; newy = y;
		bestdist = dist(newx, newy);
	}
	if (y > 0 && dist(x, y-1) < bestdist && field[x][y-1].status < TRESHOLD) {
		newx = x; newy = y-1;
		bestdist = dist(newx, newy);
	}
	if (y < levels[level].rows-1 && dist(x, y+1) < bestdist && field[x][y+1].status < TRESHOLD) {
		newx = x; newy = y+1;
		bestdist = dist(newx, newy);
	}
	/* if the fly not moved yet (ie. the lame method failed), check if it can find better path */
	if (newx == x && newy == y && flies[index].last_move == LAST_MOVEX) {
		if (y > 0 && field[x][y-1].status < TRESHOLD) newy--;
		else if (y < levels[level].rows-1 && field[x][y+1].status < TRESHOLD) newy++;
	}
	else if (newx == x && newy == y && flies[index].last_move == LAST_MOVEY) {
		if (x > 0 && field[x-1][y].status < TRESHOLD) newx--;
		else if (x < levels[level].cols-1 && field[x+1][y].status < TRESHOLD) newx++;
	}
	/* must go to a safer place, if such exists */
	if (newx == x && newy == y && field[x][y].status >= TRESHOLD) {
		if (x > 0 && field[x-1][y].status < TRESHOLD) newx--;
		else if (x < levels[level].cols-1 && field[x+1][y].status < TRESHOLD) newx++;
		else if (y > 0 && field[x][y-1].status < TRESHOLD) newy--;
		else if (y < levels[level].rows-1 && field[x][y+1].status < TRESHOLD) newy++;
	}
	if (newx != x)					/* fly moved in the x direction */
		flies[index].last_move = LAST_MOVEX;
	else if (newy != y)
		flies[index].last_move = LAST_MOVEY;	/* fly moved in the y direction */
	flies[index].posx = newx;
	flies[index].posy = newy;
	/* 15% chance that the fly turns positive bonus to negative */
	if (field[newx][newy].bonus == BONUS_SMALL && rand()%100 < 15) {
		field[newx][newy].bonus = BONUS_NEG;
		field[newx][newy].status = levels[level].start_lily * STAT_DENSITY;
	}
	/* if the fly moved, there is 40% chance to play a sound */
	if ((newx != x || newy != y) && rand()%100 < 40)
		play_snd(SND_FLYMOVE);
}

/*****************************************************************************************/

void Pond::check_aphid_death(int index, int level) {
	const int x = aphids[index].posx;
	const int y = aphids[index].posy;
	if (field[x][y].status >= NUMLILIES*STAT_DENSITY) {
    play_snd(SND_APHIDMOVE);
		aphids[index].alive = false;
		field[x][y].bonus = BONUS_BIG;	/* set big bonus on former aphid location */
		field[x][y].status = levels[level].start_lily * STAT_DENSITY;
	}
}

/*****************************************************************************************/

void Pond::move_aphid(int index, int level) {
	const int x = aphids[index].posx;
	const int y = aphids[index].posy;
	int newx = x;
	int newy = y;
	int bestdist = 0;
	if (x > 0 && dist(x-1, y) > bestdist && field[x-1][y].status < TRESHOLD) {
		newx = x-1; newy = y;
		bestdist = dist(newx, newy);
		/* try to get away from the level ending, where the frog will probably go */
		if (end_posx == levels[level].cols - 1) bestdist += dist(newx, newy)/3;			
	}
	if (x < levels[level].cols-1 && dist(x+1, y) > bestdist && field[x+1][y].status < TRESHOLD) {
		newx = x+1; newy = y;
		bestdist = dist(newx, newy);
		if (end_posx == 0) bestdist += dist(newx, newy)/3;
	}
	if (y > 0 && dist(x, y-1) > bestdist && field[x][y-1].status < TRESHOLD) {
		newx = x; newy = y-1;
		bestdist = dist(newx, newy);
		if (end_posy == levels[level].rows - 1) bestdist += dist(newx, newy)/3;
	}
	if (y < levels[level].rows-1 && dist(x, y+1) > bestdist && field[x][y+1].status < TRESHOLD) {
		newx = x; newy = y+1;
		bestdist = dist(newx, newy);
		if (end_posy == 0) bestdist += dist(newx, newy)/3;
	}
	/* must go to a safer place, if such exists */
	if (newx == x && newy == y && field[x][y].status >= TRESHOLD) {
		if (x > 0 && field[x-1][y].status < TRESHOLD) newx--;
		else if (x < levels[level].cols-1 && field[x+1][y].status < TRESHOLD) newx++;
		else if (y > 0 && field[x][y-1].status < TRESHOLD) newy--;
		else if (y < levels[level].rows-1 && field[x][y+1].status < TRESHOLD) newy++;
	} 
	aphids[index].posx = newx;
	aphids[index].posy = newy;
	/* if the aphid moved, there is 40% chance to play a sound */
	if ((newx != x || newy != y) && rand()%100 < 40)
		play_snd(SND_APHIDMOVE);
}

/*****************************************************************************************/

bool Pond::aphid_eaten() {
	for (int i = 0; i < aphid_cnt; i++) 
		if (aphids[i].alive && frog_posx == aphids[i].posx && frog_posy == aphids[i].posy) {
			aphids[i].alive = false;
			play_snd(SND_APHIDEATEN);
			return true;
		}
	return false;
}

/*****************************************************************************************/

bool Pond::exist_fly() {
	if (fly_cnt == 0)
		return false;
	int num_flies = 0;
	for (int i = 0; i < fly_cnt; i++)
		if (flies[i].alive) num_flies++;
  return (num_flies > 0);
}

/*****************************************************************************************/

void Pond::destroy_fly() {
	for (int i = 0; i < fly_cnt; i++) 
		if (flies[i].alive) {
			play_snd(SND_FLYDEATH);
			flies[i].alive = false;
			return;
		}
}

/*****************************************************************************************/

void Pond::blit_bg() {
	static int water_counter = 0;	/* used for water animation */
	static int water_index = 0;		/* index of currently viewed water screen */
	if (water_counter == 0) {			/* proceed with new frame ? */
		water_index++;
		water_index %= WATER_FRAMES;
	}
	water_counter++;
	water_counter %= WATER_SLOWNESS;
	
	/* blit the game background */
	check_surf(lpSBack);
	check_surf(lpSScreen[water_index]);
	lpSBack->BltFast(0, 0, lpSScreen[water_index], NULL, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT);
}

/*****************************************************************************************/

bool Pond::possible(BYTE vk_code, int level) {
	switch(vk_code) {
		case DIK_DOWN: return (frog_posy < levels[level].rows-1);
		case DIK_LEFT: return (frog_posx > 0);
		case DIK_RIGHT: return (frog_posx < levels[level].cols-1);
		case DIK_UP: return (frog_posy > 0);
	}
	return false;
}

/*****************************************************************************************/

void Pond::move_frog(BYTE vk_code) {
	play_snd(SND_FROGMOVE);
	switch(vk_code) {
		case DIK_DOWN: frog_posy++; frog_direction = DIR_DOWN; break;
		case DIK_LEFT: frog_posx--; frog_direction = DIR_LEFT; break;
		case DIK_RIGHT: frog_posx++; frog_direction = DIR_RIGHT;  break;
		case DIK_UP: frog_posy--; frog_direction = DIR_UP;
	}
}

/*****************************************************************************************/

void Pond::make_death(int level) {
	play_snd(SND_DEATH);
	field[frog_posx][frog_posy].status = NUMLILIES*STAT_DENSITY;
	is_death(level);
}

/*****************************************************************************************/

bool Pond::is_death(int level) {
	/* first check if the frog was eaten by a fly */
	for (int i = 0; i < fly_cnt; i++) {
		if (flies[i].alive && frog_posx == flies[i].posx && frog_posy == flies[i].posy) {
			play_snd(SND_FLYEATFROG);
			render(level, false);		/* hide the frog */
			FLIP;
			Sleep(800);
			return true;
		}
	}
	/* now check if frog is out of a lily */
	if (field[frog_posx][frog_posy].status >= NUMLILIES*STAT_DENSITY) {
		/* draw ripples and a cross */
		play_snd(SND_DEATH);
		render(level, false);		/* hide the frog */
		blit_lily(frog_posx, frog_posy, NUMLILIES, level);
		FLIP;
		Sleep(200);
		render(level, false);
		blit_lily(frog_posx, frog_posy, NUMLILIES+1, level);
		FLIP;
		Sleep(200);
		int x = levels[level].xoffset + frog_posx*(LILY_SIZEX + levels[level].xspacing - 2*levels[level].xtrim);
		int y = levels[level].yoffset + frog_posy*(LILY_SIZEY + levels[level].yspacing - 2*levels[level].ytrim);
		x += (LILY_SIZEX - 2*levels[level].xtrim - CROSS_SIZEX)/2;
		y += (LILY_SIZEY - 2*levels[level].ytrim - CROSS_SIZEY)/2;
		render(level, false);
		blit_item(x, y, CROSS_COORD);
		FLIP;
		Sleep(800);
		return true;
	}
	return false;
}

/*****************************************************************************************/

bool Pond::is_level_complete() {
	return (frog_posx == end_posx && frog_posy == end_posy);
}

/*****************************************************************************************/

void Pond::reset(int level) {
	fly_cnt = 0;
	aphid_cnt = 0;
	start_posx = 0;
	start_posy = levels[level].rows-1;
	frog_posx = start_posx;
	frog_posy = start_posy;
	frog_direction = DIR_DOWN;
	end_posx = levels[level].cols-1;
	end_posy = 0;
	adv_posx = -1;
	adv_posy = -1;
	WORD delay_max = levels[level].delay_start_max;
	WORD delay_min = levels[level].delay_start_min;
	/* set all the lilies */
	for (int i = 0; i < levels[level].cols; i++) 
		for (int j = 0; j < levels[level].rows; j++) {
			field[i][j].status = (rand()%100 < 80)? MAXSTATUS : levels[level].start_lily * STAT_DENSITY;
			field[i][j].bonus = 0;		/* clear bonus of a lily */
			/* set possible bonus */
			if (rand()%100 < levels[level].bonus_small)
				field[i][j].bonus = BONUS_SMALL;
			else if (rand()%100 < levels[level].bonus_neg)
				field[i][j].bonus = BONUS_NEG;
			/* adventure can be set only once */
			else if (adv_posx == -1 && rand()%100 < levels[level].bonus_adv) {
				field[i][j].bonus = BONUS_ADV;
				adv_posx = i;
				adv_posy = j;
			} else if (rand()%100 < levels[level].bonus_big)
				field[i][j].bonus = BONUS_BIG;
			field[i][j].delay_start = rand()%(delay_max - delay_min) + delay_min;
			field[i][j].delay_cnt = 0;
			field[i][j].decay = levels[level].min_decay + (rand()%levels[level].decay_spread);
			if (field[i][j].bonus == BONUS_ADV || field[i][j].bonus == BONUS_NEG)
				field[i][j].status = levels[level].start_lily * STAT_DENSITY;
		}
	field[start_posx][start_posy].status = levels[level].start_lily * STAT_DENSITY;
	field[start_posx][start_posy].bonus = 0;
	field[start_posx][start_posy].decay = levels[level].start_lily_decay; 
	field[end_posx][end_posy].status = levels[level].start_lily * STAT_DENSITY;
	field[end_posx][end_posy].bonus = 0;
	/* create flies */
	for (int i = 0; i < levels[level].maxflies; i++) 
		if (rand()%100 < levels[level].fly) 
			create_fly(level);
	/* create aphids */
	for (int i = 0; i < levels[level].maxaphids; i++)
		if (rand()%100 < levels[level].aphid) 
			create_aphid(level);
}

/*****************************************************************************************/

void Pond::blit_lily(int i, int j, int index, int level) {
	int x = levels[level].xoffset + i*(LILY_SIZEX + levels[level].xspacing - 2*levels[level].xtrim);
	int y = levels[level].yoffset + j*(LILY_SIZEY + levels[level].yspacing - 2*levels[level].ytrim);
	RECT lily_coords = LILIES_COORD[index];
	lily_coords.left += levels[level].xtrim;	/* trim the lily boundaries to appropriate level */
	lily_coords.right -= levels[level].xtrim;
	lily_coords.top += levels[level].ytrim;
	lily_coords.bottom -= levels[level].ytrim;
	blit_item(x, y, lily_coords);			/* blit the lily */
}

/*****************************************************************************************/

void Pond::render(int level, bool display_frog) {
	int x, y;
	/* first blit the water background */
	blit_bg();
	/* now blit each water lily in its actual state */
	for (int i = 0; i < levels[level].cols; i++) 
		for (int j = 0; j < levels[level].rows; j++) {
			if (i == frog_posx && j == frog_posy && field[i][j].status >= NUMLILIES*STAT_DENSITY)
				continue;	/* don't show the two ripples if the frog is obviously dead */
			if (field[i][j].status < MAXSTATUS-20) {	/* 20 is a tolerance for computing index below */
				int index = (int) floor((double) field[i][j].status / STAT_DENSITY);
				blit_lily(i, j, index, level);
			}
			/* blit a bonus, if any */
			if (field[i][j].status < NUMLILIES*STAT_DENSITY && field[i][j].bonus != 0) {
				RECT bonus_coords;
				switch(field[i][j].bonus) {		/* is there any bonus? */
					case BONUS_BIG: bonus_coords = BONUS_BIG_COORD; break;
					case BONUS_ADV: bonus_coords = BONUS_ADV_COORD; break;
					case BONUS_NEG: bonus_coords = BONUS_NEG_COORD; break;
					default: bonus_coords = BONUS_SMALL_COORD; break;
				}
				x = levels[level].xoffset + i*(LILY_SIZEX + levels[level].xspacing - 2*levels[level].xtrim);
				y = levels[level].yoffset + j*(LILY_SIZEY + levels[level].yspacing - 2*levels[level].ytrim);
				x += (LILY_SIZEX - 2*levels[level].xtrim)/2 - BONUS_RAD;
				y += (LILY_SIZEY - 2*levels[level].ytrim)/2 - BONUS_RAD;
				blit_item(x, y, bonus_coords);
			}
		}
	/* now blit the end of level marker, if the frog isn't there yet */
	if (!is_level_complete()) {
		x = levels[level].xoffset + end_posx*(LILY_SIZEX + levels[level].xspacing - 2*levels[level].xtrim);
		y = levels[level].yoffset + end_posy*(LILY_SIZEY + levels[level].yspacing - 2*levels[level].ytrim);
		x += (LILY_SIZEX - 2*levels[level].xtrim - STAR_SIZEX)/2 + 1;
		y += (LILY_SIZEY - 2*levels[level].ytrim - STAR_SIZEY)/2;
		blit_item(x, y, STAR_COORD);
	}
	/* blit frog to its location */
	if (display_frog) {
		x = levels[level].xoffset + frog_posx*(LILY_SIZEX + levels[level].xspacing - 2*levels[level].xtrim);
		y = levels[level].yoffset + frog_posy*(LILY_SIZEY + levels[level].yspacing - 2*levels[level].ytrim);
		x += (LILY_SIZEX - 2*levels[level].xtrim - FROG_SIZEX)/2 + 2;
		y += (LILY_SIZEY - 2*levels[level].ytrim - FROG_SIZEY)/2 - 3;
		blit_item(x, y, FROG_COORD[frog_direction]);
	}
	static int fly_frame = 0;		/* frame of fly animation */
	static int fly_frame_cnt = 0;
	static int aphid_frame = 0;	/* frame of aphid animation */
	static int aphid_frame_cnt = 0;
	/* now blit all flies */
	if (fly_frame_cnt++ >= ANIM) {
		fly_frame_cnt = 0;
		fly_frame++;
		fly_frame %= FLY_FRAMES;
		aphid_frame_cnt = 0;
		aphid_frame++;
		aphid_frame %= APHID_FRAMES;
	}
	for (int i = 0; i < fly_cnt; i++) {
		if (!flies[i].alive)		/* skip dead flies */
			continue;
		x = levels[level].xoffset + flies[i].posx*(LILY_SIZEX + levels[level].xspacing - 2*levels[level].xtrim);
		y = levels[level].yoffset + flies[i].posy*(LILY_SIZEY + levels[level].yspacing - 2*levels[level].ytrim);
		x += (LILY_SIZEX - 2*levels[level].xtrim - FLY_SIZEX)/2 + 1;
		y += (LILY_SIZEY - 2*levels[level].ytrim - FLY_SIZEY)/2 - 6;
		blit_item(x, y, FLY_COORD[fly_frame]);
	}
	/* at last blit all aphids */
	for (int i = 0; i < aphid_cnt; i++) {
		if (!aphids[i].alive)		/* skip dead aphids */
			continue;
		x = levels[level].xoffset + aphids[i].posx*(LILY_SIZEX + levels[level].xspacing - 2*levels[level].xtrim);
		y = levels[level].yoffset + aphids[i].posy*(LILY_SIZEY + levels[level].yspacing - 2*levels[level].ytrim);
		x += (LILY_SIZEX - 2*levels[level].xtrim - APHID_SIZEX)/2;
		y += (LILY_SIZEY - 2*levels[level].ytrim - APHID_SIZEY)/2 - 2;
		blit_item(x, y, APHID_COORD[aphid_frame]);
	}
}

/*****************************************************************************************/

void Pond::advance(int level) {
	/* first reduce the sizes of all lilies according to Lily::decay */
	for (int i = 0; i < levels[level].cols; i++) 
		for (int j = 0; j < levels[level].rows; j++) {
			if (i == end_posx && j == end_posy)
				continue;		/* skip ending lily */
			if (field[i][j].bonus == BONUS_ADV || field[i][j].bonus == BONUS_NEG)
				continue;		/* also skip adventure and negative bonuses */ 
			if (field[i][j].status + field[i][j].decay < MAXSTATUS) /* add to size if possible */
				field[i][j].status += field[i][j].decay;
			else {	/* if not possible, it means the lily is dead */
				field[i][j].status = MAXSTATUS; /* now render() won't display the lily */
				if (field[i][j].delay_cnt++ == field[i][j].delay_start) {
					field[i][j].delay_cnt = 0;
					WORD delay_max = levels[level].delay_start_max;
					WORD delay_min = levels[level].delay_start_min;
					field[i][j].delay_start = rand()%(delay_max - delay_min) + delay_min;
					field[i][j].status = levels[level].start_lily * STAT_DENSITY;
					if (i != start_posx || j != start_posy) 	/* change decay only for non-start lily */
						field[i][j].decay = levels[level].min_decay + (rand()%levels[level].decay_spread);
				}
			}
		}
	/* advance all flies */
	for (int i = 0; i < fly_cnt; i++) {
		if (!flies[i].alive) continue;
		if ((flies[i].cnt += flies[i].speed) >= MAXANIMAL_STATUS) {
			flies[i].cnt = 0;
			move_fly(i, level);
		}
		check_fly_death(i, level);
	}
	/* advance all aphids */
	for (int i = 0; i < aphid_cnt; i++) {
		if (!aphids[i].alive) continue;
		if ((aphids[i].cnt += aphids[i].speed) >= MAXANIMAL_STATUS) {
			aphids[i].cnt = 0;
			move_aphid(i, level);
		}
		check_aphid_death(i, level);
	}
	/* each CHANGE_END_PERIOD iterations there is a chance to change the level ending */
	static int change_cnt = 0;
	if (level >= CHANGE_END_MINLEV && change_cnt++ == CHANGE_END_PERIOD) {
		change_cnt = 0;
		int pond_diag = levels[level].cols*levels[level].cols + levels[level].rows*levels[level].rows;
		if (dist(end_posx, end_posy) < pond_diag/4) {
			if (rand()%100 < 20) end_posx = (end_posx == 0) ? levels[level].cols - 1 : 0;
			if (rand()%100 < 20) end_posy = (end_posy == 0) ? levels[level].rows - 1 : 0;
			field[end_posx][end_posy].status = levels[level].start_lily * STAT_DENSITY;
			field[end_posx][end_posy].bonus = 0;
		}
	}
}

/*****************************************************************************************/

int Pond::get_frog_bonus() {
	int res = field[frog_posx][frog_posy].bonus;
	field[frog_posx][frog_posy].bonus = 0;
	return res;
}

/*****************************************************************************************/
