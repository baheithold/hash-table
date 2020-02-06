/*
 *  Author: Brett Heithold
 *  File:   hashmap.h
 *  Last Modified:  5 Feb 2020
 */

#ifndef __HASHMAP_INCLUDED__
#define __HASHMAP_INCLUDED__

#include <stdio.h>

typedef struct HASHMAP HASHMAP;

extern HASHMAP *newHASHMAP(void);
extern void    setHASHMAPdisplayKey(HASHMAP *map, void (*display)(void *, FILE *));
extern void    setHASHMAPdisplayValue(HASHMAP *map, void (*display)(void *, FILE *));
extern void    setHASHMAPfreeKey(HASHMAP *map, void (*free)(void *));
extern void    setHASHMAPfreeValue(HASHMAP *map, void (*free)(void *));
extern void    insertHASHMAP(HASHMAP *map, void *key, void *value);
extern void   *removeHASHMAP(HASHMAP *map, void *key);
extern int     findKey(HASHMAP *map, void *key);
extern void   *getHASHMAPvalue(HASHMAP *map, void *key);
extern void    clearHASHMAP(HASHMAP *map);
extern int     containsKey(HASHMAP *map, void *key);
extern int     containsValue(HASHMAP *map, void *value);
extern int     isHASHMAPempty(HASHMAP *map);
extern int     sizeHASHMAP(HASHMAP *map);
extern void    displayHASHMAP(HASHMAP *map, FILE *fp);
extern int     debugHASHMAP(HASHMAP *map, int level);
extern void    freeHASHMAP(HASHMAP *map);

#endif // !__HASHMAP_INCLUDED__
