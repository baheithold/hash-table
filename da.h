#ifndef __DA_INCLUDED__
#define __DA_INCLUDED__

#include <stdio.h>

typedef struct DA DA;

extern DA   *newDA(void);
extern void  setDAdisplay(DA *items, void (*display)(void *, FILE *));
extern void  setDAfree(DA *items, void (*free)(void *));
extern void  insertDA(DA *items, int index, void *value);
extern void *removeDA(DA *items, int index);
extern void  unionDA(DA *recipient, DA *donor);
extern void *getDA(DA *items, int index);
extern void *setDA(DA *items, int index, void *value);
extern int   sizeDA(DA *items);
extern void  displayDA(DA *items, FILE *fp);
extern int   debugDA(DA *items, int level);
extern void  freeDA(DA *items);

#define insertDAback(items, value)      insertDA(items, sizeDA(items), value)
#define removeDAback(items)             removeDA(items, sizeDA(items) - 1)
#define insertDAfront(items, value)     insertDA(items, 0, value)
#define removeDAfront(items)            removeDA(items, 0)

#endif // !__da_INCLUDED__
