#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "debug.h"

static Map *g_map = NULL;
static Player *g_player = NULL;

int Monster_attack(void *self, int damage)
{
    Monster *monster = self;
    
    if(monster) {
    	printf("You attack %s!\n", monster->_(description));
    	monster->hit_points -= damage;

	    if((monster->hit_points > 0) && !monster->is_dead) {
    	    printf("It is still alive.\n");
    	    return 0;
		} else if((monster->hit_points <= 0) && monster->is_dead) {
			printf("You spotted %s corpse!\n", monster->_(description));
			return 2;
	    } else {
    		printf("It is dead!\n");
    		monster->is_dead = 1;
    	}
    } else {
    	printf("There is nothing to attack!\n");
    }

    return 1;
}

int Monster_init(void *self)
{
    Monster *monster = self;

    monster->hit_points = 10;
    monster->is_dead = 0;
    
    return 1;
}

void Monster_destroy(void *self)
{
	Monster *monster = self;
	if(monster) Object_destroy(monster);
}

Object MonsterProto = {
	.init = Monster_init,
	.attack = Monster_attack,
	.destroy = Monster_destroy
};

int Player_attack(void *self, int damage)
{
	Player *player = self;
	Room *hall = g_map->start;
	Room *throne = hall->north;
	Room *arena = throne->west;
	Monster *monster = arena->bad_guy;
	
	if(monster) {
		printf("%s attacks %s with %d amount of damage.\n",
			monster->_(description), player->_(description), damage);
		player->hit_points -= damage;
	
		if((player->hit_points > 0) && player->is_alive) {
			printf("%s is still alive.\n", player->_(description));
			return 0;
		} else if((player->hit_points <= 0) && !player->is_alive) {
			printf("%s has fallen and it's time to quit!\n", player->_(description));
			return 2;
		} else {
			printf("%s is dead!\n", player->_(description));
			player->is_alive = 0;
		}
	}

	return 1;
}

int Player_init(void *self)
{
	Player *player = self;
	
	player->hit_points = 15;
	player->is_alive = 1;
	
	return 1;
}

void Player_destroy(void *self)
{
	Player *player = self;
	if(player) Object_destroy(player);
}

Object PlayerProto = {
	.init = Player_init,
	.attack = Player_attack,
	.destroy = Player_destroy
};

void *Room_move(void *self, Direction direction)
{
    Room *room = self;
    Room *next = NULL;

	room->player = NULL;
	
    if(direction == NORTH && room->north) {
        printf("You go north, into:\n");
        next = room->north;
    } else if(direction == SOUTH && room->south) {
        printf("You go south, into:\n");
        next = room->south;
    } else if(direction == EAST && room->east) {
        printf("You go east, into:\n");
        next = room->east;
    } else if(direction == WEST && room->west) {
        printf("You go west, into:\n");
        next = room->west;
    } else {
        printf("You can't go that direction.\n");
        next = NULL;
    }

    if(next) {
        next->_(describe)(next);
        next->player = g_player;
    }

    return next;
}

int Room_attack(void *self, int damage)
{
    Room *room = self;
    Monster *monster = room->bad_guy;
    Player *player = room->player;
    
    if(monster && player) {
    	if(!monster->is_dead) {
        	monster->_(attack)(monster, damage);
    		return 1;
    	} else {
    		printf("%s is dead.\n", monster->_(description));
    		return 0;
    	}
    	if(player->is_alive) {
    		player->_(attack)(player, damage);
    		return 1;
    	} else {
    		printf("%s is dead.\n", player->_(description));
    		return 0;
    	}
    } else {
    	printf("There is nothing in the room to attack.\n");
    }
    
    return 1;
}

void Room_destroy(void *self)
{
	Room *room = self;
	Monster *monster = room->bad_guy;
	Player *player = room->player;
	
	if(player) Player_destroy(player);
	if(monster) Monster_destroy(monster);
	if(room) Object_destroy(room);
}

Object RoomProto = {
	.move = Room_move,
	.attack = Room_attack,
	.destroy = Room_destroy
};

void *Map_move(void *self, Direction direction)
{
    Map *map = self;
    Room *location = map->location;
    Room *next = NULL;

    next = location->_(move)(location, direction);

    if(next) {
        map->location = next;
    }

    return next;
}

int Map_attack(void *self, int damage)
{
    Map *map = self;
    Room *location = map->location;

    return location->_(attack)(location, damage);
}

int Map_init(void *self)
{
    Map *map = self;

    /* make some rooms for a small map */
    Room *hall = NEW(Room, "The great Hall");
    Room *throne = NEW(Room, "The throne room");
    Room *arena = NEW(Room, "The arena, with the minotaur");
    Room *kitchen = NEW(Room, "Kitchen, you have the knife now");

    /* put the bad guy in the arena */
    arena->bad_guy = NEW(Monster, "The evil minotaur");
    
    /* setup the map rooms */
    hall->north = throne;
    throne->west = arena;
    throne->east = kitchen;
    throne->south = hall;
    arena->east = throne;
    kitchen->west = throne;

    /* start the map and the character off in the hall */
    
    map->start = hall;
    map->location = hall;

    /* put the player in the hall */
    g_player = NEW(Player, "Freddrick the Great");

    return 1;
}

void Map_destroy(void *self)
{
	Map *map = self;
	Room *hall = map->start;
	Room *throne = hall->north;
	Room *arena = throne->west;
	Room *kitchen = throne->east;

	Room_destroy(hall);
	Room_destroy(throne);
	Room_destroy(arena);
	Room_destroy(kitchen);
	
	map->start = NULL;
	map->location = NULL;

	if(map) Object_destroy(map);
}

Object MapProto = {
    .init = Map_init,
    .move = Map_move,
    .attack = Map_attack,
    .destroy = Map_destroy
};

int process_input(Map *game)
{
    printf("\n> ");

    char ch = getchar();
    getchar(); /* eat ENTER */

    int damage = rand() % 4;
    
    g_map = game;

    switch(ch) {
        case -1:
            printf("Giving up? You suck.\n");
            return 0;
            break;

        case 'n':
            game->_(move)(game, NORTH);
            break;

        case 's':
            game->_(move)(game, SOUTH);
            break;

        case 'e':
            game->_(move)(game, EAST);
            break;

        case 'w':
            game->_(move)(game, WEST);
            break;

        case 'a':
            if((game->_(attack)(game, damage)) == 2) {
            	printf("Congrats! You won.\n");
            	return 0;
            }
            break;

        case 'l':
            printf("You can go:\n");
            if(game->location->north) printf("NORTH\n");
            if(game->location->south) printf("SOUTH\n");
            if(game->location->east) printf("EAST\n");
            if(game->location->west) printf("WEST\n");
            break;

        default:
            printf("What?: %d\n", ch);
    }

    return 1;
}
