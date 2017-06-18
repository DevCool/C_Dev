/*****************************************************
 * ctut1.c - simple ncurses program.
 * by 5n4k3
 *****************************************************
 */

/* include ncurses and string headers */
#include <ncurses.h>
#include <string.h>

/* main() - entry point for all programs.
 */
int main(void) {
	/* setup some variables */
	WINDOW *mainWnd;
	char *msgs[] = {
		"This is only a small test program.\n",
		"Please do not hesitate to ask how to\n",
		"do this. Because it's easy to explain.\n"
	};
	int scrnX, scrnY;
	int i, j;

	/* create main window with error handling */
	if((mainWnd = initscr()) == NULL) { /* initialize standard screen */
		printf("Failed to initialize ncurses.\n");
		return 1;
	}
	cbreak();  /* allow user to force close */

	/* get max screen size */
	getmaxyx(stdscr, scrnX, scrnY);
	/* loop through all of the messages
	 * printing each one. */
	for(i = 0; i < 3; i++) {
		move(scrnY/2+i, scrnX/2-strlen(msgs[i]));
		for(j = 0; j < strlen(msgs[i]); j++)
			addch(msgs[i][j]);
		refresh();
	}

	/* print a message on the screen */
	printw("Press any key to quit . . .");
	/* get a key press */
	getch();
	/* delete window and cleanup */
	delwin(mainWnd);
	endwin();
	/* refresh the screen one final time */
	refresh();

	return 0;	/* return success */
}