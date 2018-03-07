#include <time.h>
#include "common.h"

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

Vect2 makeVect2(float x, float y){
    Vect2 v = { x, y };
    return v;
}

Vect3 makeVect3(float x, float y, float z, float w){
    Vect3 v = { x, y, z, w };
    return v;
}