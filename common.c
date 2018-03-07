#include "common.h"
#include <stdlib.h>
#include <time.h>

bool running = true;

long ticsToMilliseconds(long tics) {
    //we want the duration version of the platform-independent seconds, so we / 1000.
    long platformAgnosticMilliseconds = CLOCKS_PER_SEC / 1000;

    return tics / platformAgnosticMilliseconds;
}

bool due(long compareTime, double milliseconds) {
    return ticsToMilliseconds(clock() - compareTime) >= milliseconds;
}

bool timer(long *lastTime, double hertz){
    long now = clock();
    if(due(*lastTime, hertz)) {
        *lastTime = now;
        return true;
    }else{
        return false;
    }
}

int randomMq(int min, int max) {
	return (rand() % (max + 1 - min)) + min;
}

bool chance(float probability) {
	//Shortcuts for deterministic scenarios (impossible and always)
	if(probability == 0) {
		return false;
	}else if (probability == 100) {
		return true;
	}

	//TODO: Consider simplified randomMq expression based on size of probability (e.g. 50% needs only range of 1).

	int roll = randomMq(0, 100);			//dice roll up to 100 (to match with a percentage-based probability amount)
	return probability >= roll;			//e.g. 99% is higher than a roll of 5, 50, and 75.
}