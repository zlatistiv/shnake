#include <time.h>
#include <stdlib.h>

int randint(int min, int max) {
	srand(time(NULL));
	return rand() % (max - min) + min;
}
