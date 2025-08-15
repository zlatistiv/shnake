#include "snake.h"
#include "misc.h"
#include <stdio.h>
#include <stdlib.h>

void move_snake(struct snake *snek) {
	for (size_t i = snek->size - 1; i >= 1; i--)
		snek->segments[i] = snek->segments[i - 1];
	switch (snek->direction) {
		case UP:
			snek->y--;
			snek->segments[0] = DOWN;
			break;
		case DOWN:
			snek->y++;
			snek->segments[0] = UP;
			break;
		case RIGHT:
			snek->x++;
			snek->segments[0] = LEFT;
			break;
		case LEFT:
			snek->x--;
			snek->segments[0] = RIGHT;
			break;
	}
}

int snake_loops(struct snake *snek) {
	int x, y;
	x = snek->x;
	y = snek->y;

	for (size_t i = 0; i < snek->size; i++) {
		switch (snek->segments[i]) {
		case UP:
			y--;
			break;
		case DOWN:
			y++;
			break;
		case RIGHT:
			x++;
			break;
		case LEFT:
			x--;
			break;
		}

		if (x == snek->x && y == snek->y)
			return 1;
	}
	return 0;
}

void grow_snake(struct snake *snek) {
	if (snek->size >= snek->size_preallocated) {
		snek->size_preallocated *= 2;
		snek->segments = realloc(snek->segments, snek->size_preallocated);
	}
	snek->segments[snek->size] = snek->segments[snek->size - 1];
	snek->size++;
}

void apple_respawn(struct apple *appl, size_t row_max, size_t col_max) {
	appl->x = randint(0, col_max);
	appl->y = randint(0, row_max);
}
