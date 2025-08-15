#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <sys/select.h>
#include "game.h"
#include "snake.h"

#define FRAME_COL 0
#define FRAME_ROW 3
#define INITIAL_SIZE 3

static inline void move_cursor(size_t col, size_t row) {
	printf("\033[%d;%dH", row, col);
}

static void draw_score(int score, int highscore) {
		printf("\033[2J");
		printf("\033[H");
		printf("Score: %d\n", score);
		printf("Highscore: %d\n", highscore);
		printf("\n");
}

static void draw_frame(size_t rows, size_t cols) {
	move_cursor(FRAME_COL, FRAME_ROW);
	printf("╔");

	for (size_t i = 1; i < cols + 1; i++)
		printf("═");
	printf("╗");
	
	for (size_t i = 1; i < rows + 1; i++) {
		move_cursor(FRAME_COL, FRAME_ROW + i);
		printf("║");
		move_cursor(cols + 2, FRAME_ROW + i);
		printf("║");
	}

	move_cursor(FRAME_COL, FRAME_ROW + rows + 1);
	printf("╚");
	for (size_t i = 1; i < cols + 1; i++)
		printf("═");
	printf("╝");

	fflush(stdout);
}

static void draw_snake(struct snake *snek) {
	int x, y;
	x = snek->x;
	y = snek->y;
	move_cursor(FRAME_COL + x + 2, FRAME_ROW + y + 1);
	printf("%c", '0');
	
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
			default:
				break;
		}
		move_cursor(FRAME_COL + x + 2, FRAME_ROW + y + 1);
		printf("%c", '#');
	}
	fflush(stdout);
}

static void draw_apple(struct apple *appl) {
	move_cursor(FRAME_COL + 2 + appl->x, FRAME_ROW + 1 + appl->y);
	printf("%c", '$');
	fflush(stdout);
}

static int inputAvailable() {
	struct timeval tv = {0L, 0L};
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(STDIN_FILENO, &fds);
	return select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
}

void game_main(size_t rows, size_t cols) {
	int sleep_ms;
	size_t highscore;
	struct snake snek = {0};
	struct apple appl = {0};
	highscore = INITIAL_SIZE;

start:
	if (snek.segments)
		free(snek.segments);
	if (snek.size > highscore)
		highscore = snek.size;
	sleep_ms = 70000;
	snek.x = cols / 2;
	snek.y = rows / 2;
	snek.size = INITIAL_SIZE;
	snek.size_preallocated = snek.size;
	snek.segments = malloc(snek.size);
	memset(snek.segments, RIGHT, snek.size);
	snek.direction = LEFT;
	apple_respawn(&appl, rows, cols);

	while (1) {
		if (inputAvailable()) {
			char c = getchar();
			switch (c) {
				case 'k':
					if (snek.direction != DOWN) snek.direction = UP;
					break;
				case 'j':
					if (snek.direction != UP) snek.direction = DOWN;
					break;
				case 'l':
					if (snek.direction != LEFT) snek.direction = RIGHT;
					break;
				case 'h':
					if (snek.direction != RIGHT) snek.direction = LEFT;
					break;
				default:
					break;
			}
		}

		draw_score(snek.size - INITIAL_SIZE, highscore - INITIAL_SIZE);
		draw_snake(&snek);
		draw_apple(&appl);
		draw_frame(rows, cols);

		move_snake(&snek);

		if (snake_loops(&snek))
			goto start;

		if (snek.x < 0 || snek.x >= cols || snek.y < 0 || snek.y >= rows)
			goto start;

		if (snek.x == appl.x && snek.y == appl.y) {
			grow_snake(&snek);
			apple_respawn(&appl, rows, cols);
		}

		usleep(sleep_ms);
	}
}
