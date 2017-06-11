#ifndef _game_h_
#define _game_h_

#include "object.h"

#ifdef _cplusplus
extern "C" {
#endif

struct Monster {
	Object proto;
	int hit_points;
	unsigned char is_dead;
};

typedef struct Monster Monster;

int Monster_attack(void *self, int damage);
int Monster_init(void *self);
void Monster_destroy(void *self);

Object MonsterProto;

struct Player {
	Object proto;
	int hit_points;
	unsigned char is_alive;
};

typedef struct Player Player;

int Player_attack(void *self, int damage);
int Player_init(void *self);
void Player_destroy(void *self);

Object PlayerProto;

struct Room {
	Object proto;

	Monster *bad_guy;
	Player *player;

	struct Room *north;
	struct Room *south;
	struct Room *east;
	struct Room *west;
};

typedef struct Room Room;

void *Room_move(void *self, Direction direction);
int Room_attack(void *self, int damage);
int Room_init(void *self);
void Room_destroy(void *self);

Object RoomProto;

struct Map {
	Object proto;
	Room *start;
	Room *location;
};

typedef struct Map Map;

void *Map_move(void *self, Direction direction);
int Map_attack(void *self, int damage);
int Map_init(void *self);
void Map_destroy(void *self);

Object MapProto;

int process_input(Map *game);

#ifdef _cplusplus
}
#endif

#endif
