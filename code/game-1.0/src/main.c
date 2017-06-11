#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"


//===========================================================//
//
// program entry point - all programs should have a main function
//
//===========================================================//
int main ()
{
 // call our engine initialization function
 initializeGameEngine ();
 
 // call our engine process function
 processGameEngine ();
 
 // call our engine shutdown function
 shutdownGameEngine ();
 
 // terminate the program
 return 0;
}
END_OF_MAIN();


//===========================================================//
//
// define all of our functions that we prototyped above
//
//===========================================================//



//===========================================================//
//
// showScreen ()
// blit (draw) the DoubleBuffer to the screen
//
//===========================================================//
void showScreen ()
{
 // blit the whole double buffer to the screen
 blit (DoubleBuffer,     // source Allegro BITMAP*
       screen,           // destination Allegro BITMAP*
       0, 0,             // source x,y
       0, 0,             // destination x, y
       DoubleBuffer->w,  // destination width
       DoubleBuffer->h); // destination height
}

//===========================================================//
//
// initializeGameEngine ()
// init Allegro, any game variables, and set the game run-state
//
//===========================================================//
void initializeGameEngine ()
{

 // init Allegro
 allegro_init ();
 
 fprintf (stderr, "Allegro initialized\n");
 
 // init Allegro timer module
 install_timer ();
 
 fprintf (stderr, "Allegro Timer initialized\n");
 
 // init Allegro keyboard handler module
 install_keyboard ();
 
 fprintf (stderr, "Allegro Keyboard initialized\n");
 
 // set video resolution
 set_color_depth (SCREEN_DEPTH);
 
 set_gfx_mode (GFX_AUTODETECT_WINDOWED, SCREEN_WIDTH, 
  SCREEN_HEIGHT, 0, 0);
 
 fprintf (stderr, "Allegro Window Created (%dx%dx%d)\n", SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_DEPTH);
    
 // create our double buffer
 DoubleBuffer = create_bitmap (SCREEN_WIDTH, SCREEN_HEIGHT);
 if (DoubleBuffer == NULL)
 {
  fprintf (stderr, "double buffer could not be created. program terminated\n");
  exit(1);
 }
 
 fprintf (stderr, "Allegro Double Buffer Created (%dx%d)\n", SCREEN_WIDTH, SCREEN_HEIGHT);
 
 // make sure the double buffer is not filled with garbage pixels
 clear (DoubleBuffer);
 
 // seed the random number generator
 srand (time (NULL));
 
 fprintf (stderr, "Random Number Generator Seeded\n");
 
 // load graphics for our tiles
 int index;
 for (index = 0; index < TILE_COUNT; index++)
 {
  char tile_filename[64];
  sprintf (tile_filename, DATADIR "/tile%d.bmp", index);
  Tiles[index] = load_bitmap (tile_filename, NULL);
  if (Tiles[index] == NULL)
  {
   fprintf (stderr, "tile%d.bmp could not be loaded. program terminated\n", index);
   exit(1); // terminate program
  }
 }
 
 fprintf (stderr, "Tiles loaded\n");
 
 // load graphics for our hero sprite
#ifdef DATADIR
 char* filenames[] = {DATADIR "/heronorth.bmp",
                      DATADIR "/heroeast.bmp",
                      DATADIR "/herosouth.bmp",
                      DATADIR "/herowest.bmp"};
#else
 char* filenames[] = {"../data/heronorth.bmp",
                      "../data/heroeast.bmp",
                      "../data/herosouth.bmp",
                      "../data/herowest.bmp"};
#endif
 for (index = 0; index < 4; index++)
 {
  Hero[index] = load_bitmap (filenames[index], NULL);
  if (Hero[index] == NULL)
  {
   fprintf (stderr, "%s could not be loaded. program terminated\n", filenames[index]);
   exit(1);
  }
 }
 
 fprintf (stderr, "Sprites loaded\n");
 
 // load title screen graphic
#ifdef DATADIR
 TitleScreen = load_bitmap (DATADIR "/title.bmp", NULL);
#else
 TitleScreen = load_bitmap ("../data/title.bmp", NULL);
#endif
 if (TitleScreen == NULL)
 {
  fprintf (stderr, "title.bmp could not be loaded. program terminated\n");
  exit(1);
 }
 
 fprintf (stderr, "Title Screen loaded\n");
 
 // set our game run-state
 game_state = STATE_TITLE;
 
 fprintf (stderr, "Game Initialization Complete\n");
}

//===========================================================//
//
// processGameEngine ()
// processes each run-state for our game
//
//===========================================================//
void processGameEngine ()
{
 // this is the game main loop
 // each game run-state is handled here
 // each run-state should be very easy to understand
 
 fprintf (stderr, "Starting Game Loop\n");
 
 renderMap ();
 
 while (game_state != STATE_SHUTDOWN)
 {
  switch (game_state)
  {
   case STATE_TITLE:
   {
    IF_ENTER_KEY
    {
     initGame ();
     game_state = STATE_GAME;
     renderMap ();
     break;
    }
    
    IF_EXIT_KEY
    {
     game_state = STATE_SHUTDOWN;
    }
    
    // display the title screen
    blit (TitleScreen,     // source Allegro BITMAP*
          DoubleBuffer,           // destination Allegro BITMAP*
          0, 0,             // source x,y
          0, 0,             // destination x, y
          DoubleBuffer->w,  // destination width
          DoubleBuffer->h); // destination height
          
    // call our double buffer blit function
    showScreen ();
   } break;
  
  
   case STATE_GAME:
   {
    
    IF_EXIT_KEY
    {
     game_state = STATE_TITLE;
    }
    
    IF_UP_KEY
    {
     // make hero face north
     HeroNPC.dir = DIR_NORTH;
     
     // scroll north
     scrollMap (DIR_NORTH);
    }
    
    IF_DOWN_KEY
    {
     // make hero face south
     HeroNPC.dir = DIR_SOUTH;
     
     // scroll south
     scrollMap (DIR_SOUTH);
    }
    
    IF_LEFT_KEY
    {
     // make hero face WEST
     HeroNPC.dir = DIR_WEST;
     
     // scroll west
     scrollMap (DIR_WEST);
    }
    
    IF_RIGHT_KEY
    {
     // make hero face east
     HeroNPC.dir = DIR_EAST;
     
     // scroll east
     scrollMap (DIR_EAST);
    }
    
    rest (64); // use Allegro to slow things down a little
    
   } break;
   
   default:
   {
    fprintf (stderr, "DEBUG: Unhandled Run-State Encountered...uhoh\n");
    fprintf (stderr, "DEBUG: Returning to GAME Run-State to prevent lockups\n");
    game_state = STATE_GAME;
   } break;
  } // end switch run-state
 } // end main while loop
}

//===========================================================//
//
// shutdownGameEngine ()
// cleanup after ourselves and Allegro before we terminate the program
//
//===========================================================//
void shutdownGameEngine ()
{
 // we declared pointers to Allegro BITMAP structures in our globals
 // so we need to make sure to release any memory that we allocated 
 // in our initialization
 
 int index = 0;
 
 for (index = 0; index < TILE_COUNT; index++)
 {
  if (Tiles[index] != NULL)
  {
   destroy_bitmap (Tiles[index]);
  }
 }
 fprintf (stderr, "Tiles destroyed\n");
 
 for (index = 0; index < 4; index++)
 {
  if (Hero[index] != NULL)
  {
   destroy_bitmap (Hero[index]);
  }
 }
 fprintf (stderr, "Sprites destroyed\n");
  
 if (DoubleBuffer != NULL)
 {
  destroy_bitmap (DoubleBuffer);
 }
 fprintf (stderr, "doublebuffer destroyed\n");
 
 // exit from Allegro
 allegro_exit ();
 fprintf (stderr, "Allegro shutdown\n");
}

//===========================================================//
//
// renderMap ()
// draws the map onto the double buffer
//
//===========================================================//
void renderMap ()
{
 // this new map rendering routine (as opposed to the renderMap
 // routine in part 2 supports scrolling maps, so we need to
 // only draw the tiles that are within the cameras 'view'
 
 int mx, my;
 
 for (my = Camera.y; my < Camera.y + Camera.h; my++)
 {
  for (mx = Camera.x; mx < Camera.x + Camera.w; mx++)
  {
   // get requested tile index number
   int requested_tile = Map[my][mx];
   
   // calculate drawing position by multiplying the
   // map position times the tile size and subtracting the
   // camera's tile position
   int cam_x  = Camera.x * Tiles[requested_tile]->w;
   int cam_y  = Camera.y * Tiles[requested_tile]->h;
   int tile_x = mx * Tiles[requested_tile]->w - cam_x;
   int tile_y = my * Tiles[requested_tile]->h - cam_y;
   
   blit (Tiles[requested_tile], 
         DoubleBuffer, 
         0, 0,
         tile_x, tile_y,
         Tiles[requested_tile]->w, Tiles[requested_tile]->h);
  }
 }
 
 // lets draw our hero while we are at it, since this is such
 // a simple example, its not really neccessary to create another
 // function for this..we will though in the next article

 // we use draw_sprite instead of blit for our sprites because
 // our sprites have areas on them that we do not want to draw
 // use RGB (255, 0, 255) in your drawing program when creating
 // your graphics to create transparent sections that will not
 // be drawn by draw_sprite
 draw_sprite (DoubleBuffer,                      // destination
              Hero[HeroNPC.dir],                 // source
              HeroNPC.x * Hero[HeroNPC.dir]->w,  // x
              HeroNPC.y * Hero[HeroNPC.dir]->h); // y
 
 // lets print some debugging information on the screen
 // just so you can see how to do it
 int color = makecol (0, 255, 255);
 int shadow = makecol (0, 64, 64);
 char* descriptions[] = {"GRASS", "PATH", "WALL"}; 
 int t = Map[Camera.y + HeroNPC.y][Camera.x + HeroNPC.x];
 
 char txt[128];
 sprintf (txt, "X %d, Y %d", Camera.x + HeroNPC.x, Camera.y + HeroNPC.y);
 
 // we are going to display the info next to our hero as he walks around
 shadowPrint (txt, TILE_SIZE+3+HeroNPC.x*TILE_SIZE, HeroNPC.y*TILE_SIZE-8, color, shadow);
 shadowPrint (descriptions[t], TILE_SIZE+3+HeroNPC.x*TILE_SIZE, HeroNPC.y*TILE_SIZE+8, color, shadow);
 
 // call our double buffer blit function
 showScreen ();
}



//===========================================================//
//
// shadowPrint ()
// prints a string on the double buffer using a simple shadow
// technique
//
//===========================================================//
void shadowPrint (char* txt, int x, int y, int color, int shadow)
{
 textprintf_ex (DoubleBuffer, font, x-2, y, shadow, -1, txt);
 textprintf_ex (DoubleBuffer, font, x+2, y, shadow, -1, txt);
 textprintf_ex (DoubleBuffer, font, x, y-2, shadow, -1, txt);
 textprintf_ex (DoubleBuffer, font, x, y+2, shadow, -1, txt);
 
 textprintf_ex (DoubleBuffer, font, x-1, y, shadow, -1, txt);
 textprintf_ex (DoubleBuffer, font, x+1, y, shadow, -1, txt);
 textprintf_ex (DoubleBuffer, font, x, y-1, shadow, -1, txt);
 textprintf_ex (DoubleBuffer, font, x, y+1, shadow, -1, txt);
 textprintf_ex (DoubleBuffer, font, x, y, color, -1, txt);
}



//===========================================================//
//
// loadMap ()
// loads a map from disk into memory
//
//===========================================================//
void loadMap (const char* szFilename)
{
 int mx, my;
 // we are going to use text files for our maps for now
 // to keep things simple
 
 FILE* fp; // pointer to a FILE structure

 // attempt to open the file
 fp = fopen (szFilename, "r");
 
 if (NULL != fp)
 {
  // okay the file opened, lets read the data
  for (my = 0; my < MAP_HEIGHT; my++)
   for (mx = 0; mx < MAP_WIDTH; mx++)
    fscanf (fp, "%d", &Map[my][mx]);
    
  // finished reading the data, close the file  
  fclose (fp);  
 }
 else
 {
  // uhoh, cannot open the map file
  // kill the program
  fprintf (stderr, "Uhoh, cannot read %s\nProgram terminated.\n", szFilename);
  exit (1);
 }
}


//===========================================================//
//
// scrollMap ()
// pan the camera around the map
//
//===========================================================//
void scrollMap (int direction)
{
 // get the hero's world coordinates
 HeroNPC.wx = Camera.x + HeroNPC.x;
 HeroNPC.wy = Camera.y + HeroNPC.y;
  
 // what direction are we trying to scroll?
 switch (direction)
 {
  case DIR_NORTH: { scrollNorth (); } break;
  case DIR_SOUTH: { scrollSouth (); } break;
  case DIR_EAST:  { scrollEast ();  } break;
  case DIR_WEST:  { scrollWest ();  } break;
 } // end switch direction
 
 // redraw the map
 renderMap ();
}

//===========================================================//
//
// isTileSolid ()
// returns true if the tile is solid (cannot be walked on)
//
//===========================================================//
int isTileSolid (int x, int y)
{ 
 // this is not the best way to do this, but it will suffice
 // at least until the next article when we use map events!
 if (Map[y][x] > 1)
  return 1;
 else
  return 0; 
}

//===========================================================//
//
// initGame ()
// init game variables (new game)
//
//===========================================================//
void initGame ()
{
 // load the map
#ifdef DATADIR
 loadMap (DATADIR "/world1.map");
#else
 loadMap ("../data/world1.map");
#endif
 fprintf (stderr, "Map loaded\n");
 
 // position our camera at the upper-left corner of the map
 Camera.x = 0;
 Camera.y = 0;
 
 // set camera size
 Camera.w = CAM_WIDTH;
 Camera.h = CAM_HEIGHT;
 
 // position hero in the center of the camera
 HeroNPC.x = Camera.x + Camera.w / 2;
 HeroNPC.y = Camera.y + Camera.h / 2;
 
 // make the hero face south
 HeroNPC.dir = DIR_SOUTH;
}

//===========================================================//
//
// scrollNorth ()
// scroll the camera or move the player as needed
//
//===========================================================//
void scrollNorth ()
{
 // Now for a much nicer scrolling method than the last one we used

 // if the hero Y screen position is below the middle of the screen
 if (HeroNPC.y > Camera.h / 2)
 {
  // check for a clear path to move to and then move the hero north
  if (!isTileSolid (HeroNPC.wx, HeroNPC.wy - 1))
   HeroNPC.y--;
 }
 else
 {
  // since the player has reached the middle of the screen we need to check
  // to see if the camera can scroll north any further
  if (Camera.y > 0)
  {
   // if the camera can scroll we need to first check that we do not move our
   // hero into a solid tile so check for a clear path again and move the
   // camera north
   if (!isTileSolid (HeroNPC.wx, HeroNPC.wy - 1))
    Camera.y--;
  }
  else
  {
   // the camera cannot scroll anymore, so lets move the hero north
   // but only if there is a clear path that we can travel
   if (HeroNPC.y > 0)
    if (!isTileSolid (HeroNPC.wx, HeroNPC.wy - 1))
     HeroNPC.y--;
  } // end camera scroll check
 } // end hero scroll check
}

//===========================================================//
//
// scrollSouth
// scroll the camera or move the player as needed
//
//===========================================================//
void scrollSouth()
{  
 if (HeroNPC.y < Camera.h / 2)
 {
  if (!isTileSolid (HeroNPC.wx, HeroNPC.wy + 1))
   HeroNPC.y++;
 }
 else
 {
  if (Camera.y < MAP_HEIGHT - Camera.h)
  {
   if (!isTileSolid (HeroNPC.wx, HeroNPC.wy + 1))
    Camera.y++;
  }
  else
  {
   if (HeroNPC.y < Camera.h)
    if (!isTileSolid (HeroNPC.wx, HeroNPC.wy + 1))
     HeroNPC.y++;
  }
 }

}

//===========================================================//
//
// scrollEast ()
// scroll the camera or move the player as needed
//
//===========================================================//
void scrollEast ()
{
 if (HeroNPC.x < Camera.w / 2)
 {
  if (!isTileSolid (HeroNPC.wx + 1, HeroNPC.wy))
   HeroNPC.x++;
 }
 else
 {
  if (Camera.x < MAP_WIDTH - Camera.w)
  {
   if (!isTileSolid (HeroNPC.wx + 1, HeroNPC.wy))
    Camera.x++;
  }
  else
  {
   if (HeroNPC.x < Camera.w)
    if (!isTileSolid (HeroNPC.wx + 1, HeroNPC.wy))
     HeroNPC.x++;
  }
 }
}

//===========================================================//
//
// scrollWest ()
// scroll the camera or move the player as needed
//
//===========================================================//
void scrollWest ()
{
 if (HeroNPC.x > Camera.w / 2)
 {
  if (!isTileSolid (HeroNPC.wx - 1, HeroNPC.wy))
   HeroNPC.x--;
 }
 else
 {
  if (Camera.x > 0)
  {
   if (!isTileSolid (HeroNPC.wx - 1, HeroNPC.wy))
    Camera.x--;
  }
  else
  {
   if (HeroNPC.x > 0)
    if (!isTileSolid (HeroNPC.wx - 1, HeroNPC.wy))
     HeroNPC.x--;
  }
 }
}
