
/*
Autor: Holguer A Becerra.
Year 2013-2
Course: Programacion 1 UPB- Bucaramanaga
*/


#include "time.h"

int delay (unsigned int ms) {
    clock_t t,t2;
    t = clock();//init time
    t2=t+(CLOCKS_PER_SEC/1000)*ms;
    while(clock()<t2);
}

