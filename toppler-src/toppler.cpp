/*****************************************************************************************/
/* Toppler - toppler.cpp                                                                 */
/* WinMain()                                                                             */
/*                                                                                       */
/*****************************************************************************************/

#define WIN32_LEAN_AND_MEAN						/* don't include MFC stuff */

#include "globals.h"
#include "game.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	Game* game = new Game(hInstance);	/* the most important object */

	if (error != ERR_OK) {	/* test if the Game constructor failed */
		if (lpDD && hwnd) 
			lpDD->SetCooperativeLevel(hwnd, DDSCL_NORMAL);
		MessageBox(0, error_msg[error], "FATAL ERROR", MB_OK);
		delete game;
		exit(1);
	}

	game->run();	/* run until game exit */
	
	delete game;	/* destroy everything */

	return error;
}
