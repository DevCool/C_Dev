#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "game.h"

int main(int argc, char *argv[])
{
    /* simple way to setup the randomness */
    srand(time(NULL));

    /* make our map to work with */
    Map *game = NEW(Map, "The Hall of the Minotaur.");

    printf("You enter ");
    game->location->_(describe)(game->location);

    while(process_input(game)) {
    }

	game->_(destroy)(game);
	
    return 0;
}
