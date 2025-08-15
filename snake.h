#ifndef SNAKE_H
#define SNAKE_H
#include <stddef.h>

enum Direction {
	UP = 'u',
	DOWN = 'd',
	RIGHT = 'r',
	LEFT = 'l'
};

struct snake {
	int x;
	int y;
	size_t size;
	char *segments; 
	char direction;
	size_t size_preallocated;
};

struct apple {
	int x;
	int y;
};

void move_snake(struct snake*);

int snake_loops(struct snake*);

void grow_snake(struct snake*);

void apple_respawn(struct apple*, size_t, size_t);

#endif
