/*
 *  Author: Brett Heithold
 *  File:   test-htable.c
 */


#include "hashmap.h"
#include "integer.h"
#include "real.h"
#include "string.h"


int main(void) {
    // Create and initialize the HASHMAP
    HASHMAP *map = newHASHMAP();
    setHASHMAPdisplayKey(map, displaySTRING);
    setHASHMAPdisplayValue(map, displayINTEGER);
    setHASHMAPfreeKey(map, freeSTRING);
    setHASHMAPfreeValue(map, freeINTEGER);
    // display the map with debug info
    debugHASHMAP(map, 1);
    displayHASHMAP(map, stdout);
    fprintf(stdout, "\n");
    // play with some insertions
    STRING *k0 = newSTRING("var0");
    INTEGER *v0 = newINTEGER(0);
    STRING *k1 = newSTRING("var1");
    INTEGER *v1 = newINTEGER(1);
    STRING *k2 = newSTRING("test");
    INTEGER *v2 = newINTEGER(2);
    insertHASHMAP(map, k0, v0);
    insertHASHMAP(map, k1, v1);
    insertHASHMAP(map, k2, v2);
    // display the map with debug info
    debugHASHMAP(map, 1);
    displayHASHMAP(map, stdout);
    fprintf(stdout, "\n");
    return 0;
}
