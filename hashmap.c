/*
 *  Author: Brett Heithold
 *  File:   hmap.c
 *  Last Modified:  5 Feb 2020
 */


#include "da.h"
#include "hashmap.h"
#include "sll.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


/********** Hash Table Struct **********/

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


/********** Hash Table Struct **********/

struct HASHMAP {
    int size;
    int debugLevel;
    double loadFactor;
    DA *store;

    void (*displayKey)(void *, FILE *);
    void (*displayValue)(void *, FILE *);
    void (*freeKey)(void *);
    void (*freeValue)(void *);
};


/********** Private Method Prototypes **********/


/********** Public Method Definitions **********/

HASHMAP *newHASHMAP(void) {
    HASHMAP *map = malloc(sizeof(HASHMAP));
    assert(map != NULL);
    map->size = 0;
    map->debugLevel = 0;
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
    debugDA(map->store, level);
}

void freeHASHMAP(HASHMAP *map) {
    assert(map != NULL);
    freeDA(map->store);
}


/********** Private Method Definitions **********/
