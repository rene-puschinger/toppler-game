#include <windows.h>
#include <fstream>

using namespace std;

#define NUMLEVELS 25

/* info about each level, to be read from file */
struct LevelInfo {
	BYTE cols;					/* number of used columns */
	BYTE rows;					/* number of used rows */
	BYTE start_lily;		/* index of the starting lily */
	BYTE xspacing;			/* number of pixels as a distance between each two lilies */
	BYTE yspacing;
	BYTE xtrim;				/* number of pixels to trim from left and right side of the lily */
	BYTE ytrim;				/* number of pixels to trim from upper and down side of the lily */
	BYTE xoffset;			/* offset of the left-topmost lily on the screen */
	BYTE yoffset;
	WORD min_decay;		/* minimal decay of lilies */
	WORD decay_spread;	/* maximal value to add to decay */
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
} levels[NUMLEVELS];

void save(char* fn);
void load(char* fn);

int main() {
	
	levels[0].cols = 4;
	levels[0].rows = 5;
	levels[0].start_lily = 0;
	levels[0].xspacing = 20;
	levels[0].yspacing = 1;
	levels[0].xtrim = 0;
	levels[0].ytrim = 0;
	levels[0].xoffset = 22;
	levels[0].yoffset = 3;
	levels[0].min_decay = 850;
	levels[0].decay_spread = 1800;
	levels[0].start_lily_decay = 400;
	levels[0].delay_start_min = 200;
	levels[0].delay_start_max = 650;
	levels[0].bonus_small = 0;
	levels[0].bonus_neg = 0;
	levels[0].bonus_adv = 0;
	levels[0].bonus_big = 0;
	levels[0].fly = 0;
	levels[0].maxflies = 0;
	levels[0].aphid = 0;
	levels[0].maxaphids = 0;

	levels[1].cols = 5;
	levels[1].rows = 5;
	levels[1].start_lily = 1;
	levels[1].xspacing = 10;
	levels[1].yspacing = 14;
	levels[1].xtrim = 12;
	levels[1].ytrim = 6;
	levels[1].xoffset = 9;
	levels[1].yoffset = 9;
	levels[1].min_decay = 750;
	levels[1].decay_spread = 1800;
	levels[1].start_lily_decay = 400;
	levels[1].delay_start_min = 160;
	levels[1].delay_start_max = 650;
	levels[1].bonus_small = 5;
	levels[1].bonus_neg = 0;
	levels[1].bonus_adv = 0;
	levels[1].bonus_big = 0;
	levels[1].fly = 0;
	levels[1].maxflies = 0;
	levels[1].aphid = 0;
	levels[1].maxaphids = 0;

	levels[2].cols = 6;
	levels[2].rows = 6;
	levels[2].start_lily = 2;
	levels[2].xspacing = 4;
	levels[2].yspacing = 3;
	levels[2].xtrim = 23;
	levels[2].ytrim = 10;
	levels[2].xoffset = 10;
	levels[2].yoffset = 2;
	levels[2].min_decay = 700;
	levels[2].decay_spread = 1700;
	levels[2].start_lily_decay = 370;
	levels[2].delay_start_min = 100;
	levels[2].delay_start_max = 600;
	levels[2].bonus_small = 10;
	levels[2].bonus_neg = 0;
	levels[2].bonus_adv = 1;
	levels[2].bonus_big = 0;
	levels[2].fly = 0;
	levels[2].maxflies = 0;
	levels[2].aphid = 0;
	levels[2].maxaphids = 0;

	levels[3].cols = 6;
	levels[3].rows = 6;
	levels[3].start_lily = 3;
	levels[3].xspacing = 19;
	levels[3].yspacing = 15;
	levels[3].xtrim = 30;
	levels[3].ytrim = 16;
	levels[3].xoffset = 12;
	levels[3].yoffset = 9;
	levels[3].min_decay = 670;
	levels[3].decay_spread = 1650;
	levels[3].start_lily_decay = 350;
	levels[3].delay_start_min = 100;
	levels[3].delay_start_max = 600;
	levels[3].bonus_small = 15;
	levels[3].bonus_neg = 0;
	levels[3].bonus_adv = 1;
	levels[3].bonus_big = 0;
	levels[3].fly = 10;
	levels[3].maxflies = 1;
	levels[3].aphid = 0;
	levels[3].maxaphids = 0;

	levels[4].cols = 7;
	levels[4].rows = 7;
	levels[4].start_lily = 4;
	levels[4].xspacing = 16;
	levels[4].yspacing = 5;
	levels[4].xtrim = 38;
	levels[4].ytrim = 19;
	levels[4].xoffset = 11;
	levels[4].yoffset = 12;
	levels[4].min_decay = 650;
	levels[4].decay_spread = 1600;
	levels[4].start_lily_decay = 320;
	levels[4].delay_start_min = 100;
	levels[4].delay_start_max = 600;
	levels[4].bonus_small = 17;
	levels[4].bonus_neg = 3;
	levels[4].bonus_adv = 2;
	levels[4].bonus_big = 1;
	levels[4].fly = 45;
	levels[4].maxflies = 1;
	levels[4].aphid = 0;
	levels[4].maxaphids = 0;

	levels[5].cols = 8;
	levels[5].rows = 8;
	levels[5].start_lily = 5;
	levels[5].xspacing = 11;
	levels[5].yspacing = 5;
	levels[5].xtrim = 43;
	levels[5].ytrim = 23;
	levels[5].xoffset = 12;
	levels[5].yoffset = 7;
	levels[5].min_decay = 640;
	levels[5].decay_spread = 1550;
	levels[5].start_lily_decay = 320;
	levels[5].delay_start_min = 100;
	levels[5].delay_start_max = 560;
	levels[5].bonus_small = 20;
	levels[5].bonus_neg = 4;
	levels[5].bonus_adv = 4;
	levels[5].bonus_big = 1;
	levels[5].fly = 75;
	levels[5].maxflies = 1;
	levels[5].aphid = 10;
	levels[5].maxaphids = 1;

	levels[6].cols = 9;
	levels[6].rows = 9;
	levels[6].start_lily = 6;
	levels[6].xspacing = 10;
	levels[6].yspacing = 5;
	levels[6].xtrim = 48;
	levels[6].ytrim = 27;
	levels[6].xoffset = 12;
	levels[6].yoffset = 9;
	levels[6].min_decay = 610;
	levels[6].decay_spread = 1450;
	levels[6].start_lily_decay = 320;
	levels[6].delay_start_min = 100;
	levels[6].delay_start_max = 550;
	levels[6].bonus_small = 20;
	levels[6].bonus_neg = 4;
	levels[6].bonus_adv = 5;
	levels[6].bonus_big = 2;
	levels[6].fly = 85;
	levels[6].maxflies = 1;
	levels[6].aphid = 20;
	levels[6].maxaphids = 1;

	levels[7].cols = 10;
	levels[7].rows = 10;
	levels[7].start_lily = 7;
	levels[7].xspacing = 5;
	levels[7].yspacing = 3;
	levels[7].xtrim = 50;
	levels[7].ytrim = 29;
	levels[7].xoffset = 11;
	levels[7].yoffset = 5;
	levels[7].min_decay = 600;
	levels[7].decay_spread = 1400;
	levels[7].start_lily_decay = 320;
	levels[7].delay_start_min = 100;
	levels[7].delay_start_max = 540;
	levels[7].bonus_small = 23;
	levels[7].bonus_neg = 4;
	levels[7].bonus_adv = 5;
	levels[7].bonus_big = 3;
	levels[7].fly = 95;
	levels[7].maxflies = 1;
	levels[7].aphid = 25;
	levels[7].maxaphids = 1;

	levels[8].cols = 10;
	levels[8].rows = 10;
	levels[8].start_lily = 7;
	levels[8].xspacing = 5;
	levels[8].yspacing = 3;
	levels[8].xtrim = 50;
	levels[8].ytrim = 29;
	levels[8].xoffset = 11;
	levels[8].yoffset = 5;
	levels[8].min_decay = 580;
	levels[8].decay_spread = 1250;
	levels[8].start_lily_decay = 300;
	levels[8].delay_start_min = 100;
	levels[8].delay_start_max = 520;
	levels[8].bonus_small = 25;
	levels[8].bonus_neg = 5;
	levels[8].bonus_adv = 5;
	levels[8].bonus_big = 3;
	levels[8].fly = 95;
	levels[8].maxflies = 1;
	levels[8].aphid = 30;
	levels[8].maxaphids = 1;

	levels[9].cols = 11;
	levels[9].rows = 10;
	levels[9].start_lily = 8;
	levels[9].xspacing = 5;
	levels[9].yspacing = 5;
	levels[9].xtrim = 53;
	levels[9].ytrim = 30;
	levels[9].xoffset = 7;
	levels[9].yoffset = 7;
	levels[9].min_decay = 570;
	levels[9].decay_spread = 1200;
	levels[9].start_lily_decay = 300;
	levels[9].delay_start_min = 100;
	levels[9].delay_start_max = 520;
	levels[9].bonus_small = 27;
	levels[9].bonus_neg = 8;
	levels[9].bonus_adv = 5;
	levels[9].bonus_big = 4;
	levels[9].fly = 50;
	levels[9].maxflies = 2;
	levels[9].aphid = 37;
	levels[9].maxaphids = 1;

	levels[10].cols = 11;
	levels[10].rows = 10;
	levels[10].start_lily = 8;
	levels[10].xspacing = 5;
	levels[10].yspacing = 5;
	levels[10].xtrim = 53;
	levels[10].ytrim = 30;
	levels[10].xoffset = 7;
	levels[10].yoffset = 7;
	levels[10].min_decay = 560;
	levels[10].decay_spread = 1150;
	levels[10].start_lily_decay = 300;
	levels[10].delay_start_min = 100;
	levels[10].delay_start_max = 520;
	levels[10].bonus_small = 30;
	levels[10].bonus_neg = 10;
	levels[10].bonus_adv = 5;
	levels[10].bonus_big = 5;
	levels[10].fly = 60;
	levels[10].maxflies = 2;
	levels[10].aphid = 45;
	levels[10].maxaphids = 1;

	levels[11].cols = 12;
	levels[11].rows = 11;
	levels[11].start_lily = 9;
	levels[11].xspacing = 3;
	levels[11].yspacing = 2;
	levels[11].xtrim = 55;
	levels[11].ytrim = 31;
	levels[11].xoffset = 5;
	levels[11].yoffset = 5;
	levels[11].min_decay = 550;
	levels[11].decay_spread = 1000;
	levels[11].start_lily_decay = 280;
	levels[11].delay_start_min = 100;
	levels[11].delay_start_max = 510;
	levels[11].bonus_small = 33;
	levels[11].bonus_neg = 10;
	levels[11].bonus_adv = 5;
	levels[11].bonus_big = 6;
	levels[11].fly = 70;
	levels[11].maxflies = 2;
	levels[11].aphid = 52;
	levels[11].maxaphids = 1;

	levels[12].cols = 12;
	levels[12].rows = 11;
	levels[12].start_lily = 9;
	levels[12].xspacing = 3;
	levels[12].yspacing = 2;
	levels[12].xtrim = 55;
	levels[12].ytrim = 31;
	levels[12].xoffset = 5;
	levels[12].yoffset = 5;
	levels[12].min_decay = 530;
	levels[12].decay_spread = 950;
	levels[12].start_lily_decay = 260;
	levels[12].delay_start_min = 100;
	levels[12].delay_start_max = 500;
	levels[12].bonus_small = 35;
	levels[12].bonus_neg = 13;
	levels[12].bonus_adv = 5;
	levels[12].bonus_big = 7;
	levels[12].fly = 73;
	levels[12].maxflies = 2;
	levels[12].aphid = 40;
	levels[12].maxaphids = 2;

	levels[13].cols = 13;
	levels[13].rows = 12;
	levels[13].start_lily = 10;
	levels[13].xspacing = 0;
	levels[13].yspacing = 3;
	levels[13].xtrim = 56;
	levels[13].ytrim = 34;
	levels[13].xoffset = 3;
	levels[13].yoffset = 10;
	levels[13].min_decay = 510;
	levels[13].decay_spread = 900;
	levels[13].start_lily_decay = 250;
	levels[13].delay_start_min = 100;
	levels[13].delay_start_max = 500;
	levels[13].bonus_small = 39;
	levels[13].bonus_neg = 14;
	levels[13].bonus_adv = 7;
	levels[13].bonus_big = 7;
	levels[13].fly = 65;
	levels[13].maxflies = 2;
	levels[13].aphid = 42;
	levels[13].maxaphids = 2;

	levels[14].cols = 13;
	levels[14].rows = 12;
	levels[14].start_lily = 10;
	levels[14].xspacing = 0;
	levels[14].yspacing = 3;
	levels[14].xtrim = 56;
	levels[14].ytrim = 34;
	levels[14].xoffset = 3;
	levels[14].yoffset = 10;
	levels[14].min_decay = 500;
	levels[14].decay_spread = 870;
	levels[14].start_lily_decay = 240;
	levels[14].delay_start_min = 100;
	levels[14].delay_start_max = 500;
	levels[14].bonus_small = 40;
	levels[14].bonus_neg = 15;
	levels[14].bonus_adv = 7;
	levels[14].bonus_big = 10;
	levels[14].fly = 70;
	levels[14].maxflies = 2;
	levels[14].aphid = 42;
	levels[14].maxaphids = 2;

	levels[15].cols = 13;
	levels[15].rows = 13;
	levels[15].start_lily = 11;
	levels[15].xspacing = 4;
	levels[15].yspacing = 2;
	levels[15].xtrim = 58;
	levels[15].ytrim = 35;
	levels[15].xoffset = 5;
	levels[15].yoffset = 8;
	levels[15].min_decay = 500;
	levels[15].decay_spread = 850;
	levels[15].start_lily_decay = 200;
	levels[15].delay_start_min = 100;
	levels[15].delay_start_max = 500;
	levels[15].bonus_small = 42;
	levels[15].bonus_neg = 16;
	levels[15].bonus_adv = 7;
	levels[15].bonus_big = 12;
	levels[15].fly = 75;
	levels[15].maxflies = 2;
	levels[15].aphid = 50;
	levels[15].maxaphids = 2;

	levels[16].cols = 15;
	levels[16].rows = 14;
	levels[16].start_lily = 12;
	levels[16].xspacing = 0;
	levels[16].yspacing = 2;
	levels[16].xtrim = 60;
	levels[16].ytrim = 37;
	levels[16].xoffset = 3;
	levels[16].yoffset = 14;
	levels[16].min_decay = 480;
	levels[16].decay_spread = 820;
	levels[16].start_lily_decay = 190;
	levels[16].delay_start_min = 100;
	levels[16].delay_start_max = 500;
	levels[16].bonus_small = 45;
	levels[16].bonus_neg = 16;
	levels[16].bonus_adv = 7;
	levels[16].bonus_big = 14;
	levels[16].fly = 85;
	levels[16].maxflies = 2;
	levels[16].aphid = 55;
	levels[16].maxaphids = 2;

	levels[17].cols = 16;
	levels[17].rows = 16;
	levels[17].start_lily = 13;
	levels[17].xspacing = 0;
	levels[17].yspacing = 0;
	levels[17].xtrim = 62;
	levels[17].ytrim = 38;
	levels[17].xoffset = 7;
	levels[17].yoffset = 9;
	levels[17].min_decay = 480;
	levels[17].decay_spread = 800;
	levels[17].start_lily_decay = 170;
	levels[17].delay_start_min = 100;
	levels[17].delay_start_max = 500;
	levels[17].bonus_small = 50;
	levels[17].bonus_neg = 18;
	levels[17].bonus_adv = 8;
	levels[17].bonus_big = 16;
	levels[17].fly = 89;
	levels[17].maxflies = 2;
	levels[17].aphid = 35;
	levels[17].maxaphids = 3;

	levels[18].cols = 16;
	levels[18].rows = 16;
	levels[18].start_lily = 13;
	levels[18].xspacing = 0;
	levels[18].yspacing = 0;
	levels[18].xtrim = 62;
	levels[18].ytrim = 38;
	levels[18].xoffset = 7;
	levels[18].yoffset = 9;
	levels[18].min_decay = 430;
	levels[18].decay_spread = 800;
	levels[18].start_lily_decay = 160;
	levels[18].delay_start_min = 100;
	levels[18].delay_start_max = 500;
	levels[18].bonus_small = 53;
	levels[18].bonus_neg = 20;
	levels[18].bonus_adv = 8;
	levels[18].bonus_big = 18;
	levels[18].fly = 90;
	levels[18].maxflies = 2;
	levels[18].aphid = 50;
	levels[18].maxaphids = 3;

	levels[19].cols = 17;
	levels[19].rows = 18;
	levels[19].start_lily = 14;
	levels[19].xspacing = 1;
	levels[19].yspacing = 0;
	levels[19].xtrim = 64;
	levels[19].ytrim = 40;
	levels[19].xoffset = 9;
	levels[19].yoffset = 9;
	levels[19].min_decay = 400;
	levels[19].decay_spread = 800;
	levels[19].start_lily_decay = 140;
	levels[19].delay_start_min = 100;
	levels[19].delay_start_max = 480;
	levels[19].bonus_small = 56;
	levels[19].bonus_neg = 20;
	levels[19].bonus_adv = 8;
	levels[19].bonus_big = 27;
	levels[19].fly = 65;
	levels[19].maxflies = 3;
	levels[19].aphid = 70;
	levels[19].maxaphids = 3;

	levels[20].cols = 17;
	levels[20].rows = 18;
	levels[20].start_lily = 14;
	levels[20].xspacing = 1;
	levels[20].yspacing = 0;
	levels[20].xtrim = 64;
	levels[20].ytrim = 40;
	levels[20].xoffset = 9;
	levels[20].yoffset = 9;
	levels[20].min_decay = 350;
	levels[20].decay_spread = 750;
	levels[20].start_lily_decay = 100;
	levels[20].delay_start_min = 100;
	levels[20].delay_start_max = 480;
	levels[20].bonus_small = 56;
	levels[20].bonus_neg = 20;
	levels[20].bonus_adv = 8;
	levels[20].bonus_big = 35;
	levels[20].fly = 80;
	levels[20].maxflies = 3;
	levels[20].aphid = 80;
	levels[20].maxaphids = 3;

	levels[21].cols = 19;
	levels[21].rows = 18;
	levels[21].start_lily = 15;
	levels[21].xspacing = 1;
	levels[21].yspacing = 2;
	levels[21].xtrim = 66;
	levels[21].ytrim = 41;
	levels[21].xoffset = 2;
	levels[21].yoffset = 11;
	levels[21].min_decay = 300;
	levels[21].decay_spread = 700;
	levels[21].start_lily_decay = 100;
	levels[21].delay_start_min = 100;
	levels[21].delay_start_max = 420;
	levels[21].bonus_small = 40;
	levels[21].bonus_neg = 21;
	levels[21].bonus_adv = 8;
	levels[21].bonus_big = 40;
	levels[21].fly = 85;
	levels[21].maxflies = 3;
	levels[21].aphid = 85;
	levels[21].maxaphids = 3;

	levels[22].cols = 19;
	levels[22].rows = 18;
	levels[22].start_lily = 15;
	levels[22].xspacing = 1;
	levels[22].yspacing = 2;
	levels[22].xtrim = 66;
	levels[22].ytrim = 41;
	levels[22].xoffset = 2;
	levels[22].yoffset = 11;
	levels[22].min_decay = 300;
	levels[22].decay_spread = 700;
	levels[22].start_lily_decay = 100;
	levels[22].delay_start_min = 100;
	levels[22].delay_start_max = 420;
	levels[22].bonus_small = 40;
	levels[22].bonus_neg = 22;
	levels[22].bonus_adv = 8;
	levels[22].bonus_big = 50;
	levels[22].fly = 90;
	levels[22].maxflies = 3;
	levels[22].aphid = 70;
	levels[22].maxaphids = 4;

	levels[23].cols = 19;
	levels[23].rows = 18;
	levels[23].start_lily = 15;
	levels[23].xspacing = 1;
	levels[23].yspacing = 2;
	levels[23].xtrim = 66;
	levels[23].ytrim = 41;
	levels[23].xoffset = 2;
	levels[23].yoffset = 11;
	levels[23].min_decay = 300;
	levels[23].decay_spread = 700;
	levels[23].start_lily_decay = 100;
	levels[23].delay_start_min = 100;
	levels[23].delay_start_max = 420;
	levels[23].bonus_small = 40;
	levels[23].bonus_neg = 20;
	levels[23].bonus_adv = 5;
	levels[23].bonus_big = 60;
	levels[23].fly = 90;
	levels[23].maxflies = 3;
	levels[23].aphid = 80;
	levels[23].maxaphids = 4;

	levels[24].cols = 19;
	levels[24].rows = 18;
	levels[24].start_lily = 15;
	levels[24].xspacing = 1;
	levels[24].yspacing = 2;
	levels[24].xtrim = 66;
	levels[24].ytrim = 41;
	levels[24].xoffset = 2;
	levels[24].yoffset = 11;
	levels[24].min_decay = 240;
	levels[24].decay_spread = 610;
	levels[24].start_lily_decay = 100;
	levels[24].delay_start_min = 90;
	levels[24].delay_start_max = 420;
	levels[24].bonus_small = 30;
	levels[24].bonus_neg = 5;
	levels[24].bonus_adv = 6;
	levels[24].bonus_big = 80;
	levels[24].fly = 75;
	levels[24].maxflies = 4;
	levels[24].aphid = 90;
	levels[24].maxaphids = 5;

	save("toppler.lvl"); 
	
	return 0;
}

void save(char* fn) {
	ofstream fout(fn, ios::binary);
	for (int i = 0; i < NUMLEVELS; i++) {
		fout.write((char*) &levels[i], sizeof(LevelInfo));
	}
	fout.close();
}

void load(char* fn) {
	ifstream fin(fn, ios::binary);
	for (int i = 0; i < NUMLEVELS; i++) {
		fin.read((char*) &levels[i], sizeof(LevelInfo));
	}
	fin.close();
}