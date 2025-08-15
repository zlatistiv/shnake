#ifndef GAME_H
#define GAME_H

#include <termios.h>
#include <unistd.h>

#define SETUP_UNBUFFERED() \
	static struct termios oldt, newt;\
	tcgetattr(STDIN_FILENO, &oldt);\
	newt = oldt;\
	newt.c_lflag &= ~(ICANON);\
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);\

#define SETUP_BUFFERED() tcsetattr(STDIN_FILENO, TCSANOW, &oldt)


// https://stackoverflow.com/questions/17318886/fflush-is-not-working-in-linux/23885008#23885008
#define CLEAR() \
	int stdin_copy = dup(STDIN_FILENO); \
	tcdrain(stdin_copy); \
	tcflush(stdin_copy, TCIFLUSH); \
	close(stdin_copy); \

void game_main(size_t, size_t);

#endif
