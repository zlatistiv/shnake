#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include "game.h"

static void help(void) {
	printf("-c <COLS>: number of columns\n");
	printf("-r <ROWS>: number of rows\n");
	printf("How to play:\n");
	printf("\tMove with hjkl (LEFT DOWN UP RIGHT)\n");
	printf("\tAvoid the walls and your tail, collect the apples.\n");
}

static struct option long_options[] = {
	{"help",	no_argument,		NULL, 'h'},
	{"rows",	required_argument,	NULL, 'r'},
	{"cols",	required_argument,	NULL, 'c'},
	{0, 0, 0, 0}
};

static struct termios original, modified;

static void get_terminal_size(size_t *rows, size_t *cols) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    
    *rows = w.ws_row;
    *cols = w.ws_col;
}


static int setup_term() {
	if (tcgetattr(STDIN_FILENO, &original) == -1) {
		perror("tcgetattr");
		return -1;
	}
	modified = original;

	modified.c_lflag &= ~(ICANON | ECHO);

	if (tcsetattr(STDIN_FILENO, TCSANOW, &modified) == -1) {
		perror("tcsetattr");
		return -1;
	}
	    
	return 0;
}


static int restore_term() {
	if (tcsetattr(STDIN_FILENO, TCSANOW, &original) == -1) {
		perror("tcsetattr");
		return -1;
	}
	    
	return 0;
}

void sigint_handler(int sig) {
	if (restore_term() == -1)
		_exit(1);
	_exit(0);
}


int main(int argc, char *argv[]) {
	int opt, option_index;
	size_t rows, cols;

	get_terminal_size(&rows, &cols);
	rows -= 4;
	cols -= 2;
	option_index = 0;

	while ((opt = getopt_long(argc, argv, "hr:c:", long_options, &option_index)) != -1) {
		switch (opt) {
			case 'h':
				help();
				exit(0);
				break;
			case 'r':
				rows = strtoul(optarg, NULL, 10);
				if (errno == EINVAL || errno == ERANGE || optarg[0] == '-') {
					perror("Error parsing row count");
				}
				break;
			case 'c':
				cols = strtoul(optarg, NULL, 10);
				if (errno == EINVAL || errno == ERANGE || optarg[0] == '-') {
					perror("Error parsing column count");
				}
				break;
		}
	}

	signal(SIGINT, sigint_handler);

	if (setup_term() == -1) {
		exit(1);
	}

	game_main(rows, cols);

	if (restore_term() == -1) {
		exit(1);
	}

	return 0;
}
