/*
 *  Author: Brett Heithold
 *  File:   hashmap.c
 *  Last Modified:  6 Feb 2020
 */


#include "da.h"
#include "hashmap.h"
#include "sll.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


/********** Global Constants **********/
#define INITIAL_CAPACITY 16
#define DEFAULT_LOAD_FACTOR 0.75f


/********** Hash Map Struct **********/

typedef struct htnode {
    void *key;
    void *value;

    void (*displayKey)(void *, FILE *);
    void (*freeKey)(void *);
    void (*displayValue)(void *, FILE *);
    void (*freeValue)(void *);
} HNODE;

HNODE *newHNODE(void *key, void *value) {
    HNODE *node = malloc(sizeof(HNODE));
    node->key = key;
    node->value = value;
    return node;
}

void setHNODEdisplayKey(HNODE *node, void (*display)(void *, FILE *)) {
    assert(node != NULL);
    node->displayKey = display;
}

void setHNODEdisplayValue(HNODE *node, void (*display)(void *, FILE *)) {
    assert(node != NULL);
    node->displayValue = display;
}

void setHNODEfreeKey(HNODE *node, void (*free)(void *)) {
    assert(node != NULL);
    node->freeKey = free;
}

void setHNODEfreeValue(HNODE *node, void (*free)(void *)) {
    assert(node != NULL);
    node->freeValue = free;
}

void displayHNODE(HNODE *node, FILE *fp) {
    assert(node != NULL);
    fprintf(fp, "(");
    if (node->displayKey == NULL) {
        // if no displayKey function is provided, print the address
        fprintf(fp, "%p", node->key);
    }
    else node->displayKey(node->key, fp);
    fprintf(fp, " : ");
    if (node->displayValue == NULL) {
        // if no displayValue function is provided, print the address
        fprintf(fp, "%p", node->value);
    }
    else node->displayValue(node->value, fp);
    fprintf(fp, ")");
}

void freeHNODE(HNODE *node) {
    assert(node != NULL);
    if (node->key != NULL && node->freeKey != NULL) {
        node->freeKey(node->key);
    }
    if (node->value != NULL && node->freeValue != NULL) {
        node->freeValue(node->value);
    }
    free(node);
}


/********** Hash Map Struct **********/

struct HASHMAP {
    int size;
    int capacity;
    double loadFactor;
    int debugLevel;
    DA *store;

    void (*displayKey)(void *, FILE *);
    void (*displayValue)(void *, FILE *);
    void (*freeKey)(void *);
    void (*freeValue)(void *);
};


/********** Private Method Prototypes **********/
static int thresholdHASHMAP(HASHMAP *map);


/********** Public Method Definitions **********/

HASHMAP *newHASHMAP(void) {
    HASHMAP *map = malloc(sizeof(HASHMAP));
    assert(map != NULL);
    map->size = 0;
    map->capacity = INITIAL_CAPACITY;
    map->loadFactor = DEFAULT_LOAD_FACTOR;
    map->debugLevel = 0;
    map->store = newDA();
    // create store and initialize with singly-linked lists
    for (int i = 0; i < map->capacity; ++i) {
        insertDAback(map->store, newSLL(NULL, NULL));
    }
    shrinkToFitDA(map->store);
    return map;
}

void setHASHMAPdisplayKey(HASHMAP *map, void (*display)(void *, FILE *)) {
    assert(map != NULL);
    map->displayKey= display;
}

void setHASHMAPdisplayValue(HASHMAP *map, void (*display)(void *, FILE *)) {
    assert(map != NULL);
    map->displayValue = display;
}

void setHASHMAPfreeKey(HASHMAP *map, void (*free)(void *)) {
    assert(map != NULL);
    map->freeKey= free;
}

void setHASHMAPfreeValue(HASHMAP *map, void (*free)(void *)) {
    assert(map != NULL);
    map->freeValue = free;
}

double setHASHMAPloadFactor(HASHMAP *map, double loadFactor) {
    assert(map != NULL);
    assert(loadFactor > 0);
    double oldLoadFactor = map->loadFactor;
    map->loadFactor = loadFactor;
    return oldLoadFactor;
}

int isHASHMAPempty(HASHMAP *map) {
    assert(map != NULL);
    return map->size == 0;
}

int sizeHASHMAP(HASHMAP *map) {
    assert(map != NULL);
    return map->size;
}

int debugHASHMAP(HASHMAP *map, int level) {
    assert(map !=NULL);
    assert(level >= 0);
    map->debugLevel = level;
    return debugDA(map->store, level);
}

void freeHASHMAP(HASHMAP *map) {
    assert(map != NULL);
    freeDA(map->store);
    free(map);
}


/********** Private Method Definitions **********/

static int thresholdHASHMAP(HASHMAP *map) {
    assert(map != NULL);
    return map->capacity * map->loadFactor;
}
