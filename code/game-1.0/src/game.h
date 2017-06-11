#ifndef __GAME_H_
#define __GAME_H_

//===========================================================//
//
// include the Allegro header since this project uses Allegro
//
//===========================================================//
#include <allegro.h>

//===========================================================//
//
// define the game video screen-resolution
//
//===========================================================//
#define SCREEN_WIDTH    640
#define SCREEN_HEIGHT   480
#define SCREEN_DEPTH    16

//===========================================================//
//
// define some run-states for the game
//
//===========================================================//
#define STATE_TITLE     0
#define STATE_GAME      1
#define STATE_SHUTDOWN  255

//===========================================================//
//
// define movement directions
//
//===========================================================//
#define DIR_NORTH       0
#define DIR_EAST        1
#define DIR_SOUTH       2
#define DIR_WEST        3

//===========================================================//
//
// define tile engine parameters
//
//===========================================================//
#define TILE_SIZE       32
#define TILE_COUNT      3
#define MAP_WIDTH       80
#define MAP_HEIGHT      60
#define CAM_WIDTH       20
#define CAM_HEIGHT      15

//===========================================================//
//
// define some macros for easily checking key presses
//
//===========================================================//
#define IF_EXIT_KEY     if (key[KEY_ESC])
#define IF_ENTER_KEY    if (key[KEY_ENTER])
#define IF_MENU_KEY     if (key[KEY_SPACE])
#define IF_UP_KEY       if (key[KEY_UP])
#define IF_DOWN_KEY     if (key[KEY_DOWN])
#define IF_LEFT_KEY     if (key[KEY_LEFT])
#define IF_RIGHT_KEY    if (key[KEY_RIGHT])

//===========================================================//
//
// declare Allegro BITMAP structure pointer that will serve
// as our drawing surface that will be blit (drawn) to the screen
// after all of our drawing has finished
//
//===========================================================//
BITMAP* DoubleBuffer;

//===========================================================//
//
// declare Allegro BITMAP structure pointer that will hold
// the title screen graphic
//
//===========================================================//
BITMAP* TitleScreen;

//===========================================================//
//
// declare Allegro BITMAP structure pointer that will hold
// the tile graphics that are used in the game
//
//===========================================================//
BITMAP* Tiles[TILE_COUNT];

//===========================================================//
//
// declare Allegro BITMAP structure pointer that will hold
// the sprite graphics for the player
//
//===========================================================//
BITMAP* Hero[4];

//===========================================================//
//
// declare the array that holds the map
//
//===========================================================//
int Map[MAP_HEIGHT][MAP_WIDTH];

//===========================================================//
//
// define a very basic structure for our NPCs
//
//===========================================================//
typedef struct NPC_typ
{
 int x, y;   // NPC screen position
 int wx, wy; // NPC world position
 int dir;    // direction NPC is facing
} NPC;

//===========================================================//
//
// define a very simple structure for our camera
//
//===========================================================//
typedef struct CAMERA_typ
{
 int x, y; // camera position
 int w, h; // camera size
} CAMERA;

//===========================================================//
//
// declare an instance of the CAMERA structure for our camera
//
//===========================================================//
CAMERA Camera;

//===========================================================//
//
// declare an instance of the NPC structure for our hero
//
//===========================================================//
NPC HeroNPC;

//===========================================================//
//
// declare and initialize our game run-state tracking variable
//
//===========================================================//
int game_state = STATE_TITLE;

//===========================================================//
//
// declare function prototypes that we will define later
//
//===========================================================//

//===========================================================//
//
// showScreen ()
// blit (draw) the DoubleBuffer to the screen
//
//===========================================================//
void showScreen ();

//===========================================================//
//
// initializeGameEngine ()
// init Allegro, any game variables, and set the game run-state
//
//===========================================================//
void initializeGameEngine ();

//===========================================================//
//
// processGameEngine ()
// processes each run-state for our game
//
//===========================================================//
void processGameEngine ();

//===========================================================//
//
// shutdownGameEngine ()
// cleanup after ourselves and Allegro before we terminate the program
//
//===========================================================//
void shutdownGameEngine ();

//===========================================================//
//
// loadMap ()
// loads a map from disk into memory
//
//===========================================================//
void loadMap (const char* szFilename);

//===========================================================//
//
// renderMap ()
// draws the map onto the double buffer
//
//===========================================================//
void renderMap ();

//===========================================================//
//
// scrollMap ()
// pan the camera around the map
//
//===========================================================//
void scrollMap (int direction);

//===========================================================//
//
// isTileSolid ()
// returns true if the tile is solid (cannot be walked on)
//
//===========================================================//
int isTileSolid (int x, int y);


//===========================================================//
//
// shadowPrint ()
// prints a string on the double buffer using a simple shadow
// technique
//
//===========================================================//
void shadowPrint (char* txt, int x, int y, int color, int shadow);


//===========================================================//
//
// initGame ()
// init game variables (new game)
//
//===========================================================//
void initGame ();


//===========================================================//
//
// scrollNorth ()
// scroll the camera or move the player as needed
//
//===========================================================//
void scrollNorth ();

//===========================================================//
//
// scrollSouth
// scroll the camera or move the player as needed
//
//===========================================================//
void scrollSouth();

//===========================================================//
//
// scrollEast ()
// scroll the camera or move the player as needed
//
//===========================================================//
void scrollEast ();

//===========================================================//
//
// scrollWest ()
// scroll the camera or move the player as needed
//
//===========================================================//
void scrollWest ();

#endif
