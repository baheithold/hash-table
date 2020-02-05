/*
 *  Author: Brett Heithold
 *  File:   htable.h
 */

#ifndef __HTABLE_INCLUDED__
#define __HTABLE_INCLUDED__

#include <stdio.h>

typedef struct HTABLE HTABLE;

extern HTABLE *newHTABLE(void);
extern void    setHTABLEdisplay(HTABLE *table, void (*display)(void *, FILE *));
extern void    setHTABLEfree(HTABLE *table, void (*free)(void *));
extern void    insertHTABLE(HTABLE *table, void *key, void *value);
extern void   *removeHTABLE(HTABLE *table, void *key);
extern int     findKey(HTABLE *table, void *key);
extern void   *getHTABLEvalue(HTABLE *table, void *key);
extern void    clearHTABLE(HTABLE *table);
extern int     containsKey(HTABLE *table, void *key);
extern int     containsValue(HTABLE *table, void *value);
extern int     isHTABLEempty(HTABLE *table);
extern int     sizeHTABLE(HTABLE *table);
extern void    displayHTABLE(HTABLE *table, FILE *fp);
extern int     debugHTABLE(HTABLE *table, int level);
extern void    freeHTABLE(HTABLE *table);

#endif // !__HTABLE_INCLUDED__
