/*
 *  Author: Brett Heithold
 *  File:   hashmap.h
 *  Last Modified:  11 Feb 2020
 */

#ifndef __HASHMAP_INCLUDED__
#define __HASHMAP_INCLUDED__

#include <stdbool.h>
#include <stdio.h>

typedef struct HASHMAP HASHMAP;

extern HASHMAP *newHASHMAP(int (*prehash)(void *), int (*comparator)(void *, void *));
extern void    setHASHMAPdisplayKey(HASHMAP *map, void (*display)(void *, FILE *));
extern void    setHASHMAPdisplayValue(HASHMAP *map, void (*display)(void *, FILE *));
extern void    setHASHMAPfreeKey(HASHMAP *map, void (*free)(void *));
extern void    setHASHMAPfreeValue(HASHMAP *map, void (*free)(void *));
extern double  setHASHMAPLoadFactor(HASHMAP *map, double loadFactor);
extern void    insertHASHMAP(HASHMAP *map, void *key, void *value);
extern void   *removeHASHMAP(HASHMAP *map, void *key);
extern void   *getHASHMAPvalue(HASHMAP *map, void *key);
extern void    clearHASHMAP(HASHMAP *map);
extern bool    containsKey(HASHMAP *map, void *key);
extern bool    isHASHMAPempty(HASHMAP *map);
extern int     sizeHASHMAP(HASHMAP *map);
extern void    displayHASHMAP(HASHMAP *map, FILE *fp);
extern int     debugHASHMAP(HASHMAP *map, int level);
extern void    freeHASHMAP(HASHMAP *map);

#endif // !__HASHMAP_INCLUDED__
