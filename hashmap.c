/*
 *  Author: Brett Heithold
 *  File:   hashmap.c
 *  Last Modified:  10 Feb 2020
 */


#include "da.h"
#include "hashmap.h"
#include "sll.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


/********** Global Constants **********/
#define INITIAL_CAPACITY 17
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

void displayHNODE(void *node, FILE *fp) {
    assert(node != NULL);
    fprintf(fp, "(");
    if (((HNODE *)node)->displayKey == NULL) {
        // if no displayKey function is provided, print the address
        fprintf(fp, "%p", ((HNODE *)node)->key);
    }
    else ((HNODE *)node)->displayKey(((HNODE *)node)->key, fp);
    fprintf(fp, " : ");
    if (((HNODE *)node)->displayValue == NULL) {
        // if no displayValue function is provided, print the address
        fprintf(fp, "%p", ((HNODE *)node)->value);
    }
    else ((HNODE *)node)->displayValue(((HNODE *)node)->value, fp);
    fprintf(fp, ")");
}

void freeHNODE(void *node) {
    assert(node != NULL);
    if (((HNODE *)node)->key != NULL && ((HNODE *)node)->freeKey != NULL) {
        ((HNODE *)node)->freeKey(((HNODE *)node)->key);
    }
    if (((HNODE *)node)->value != NULL && ((HNODE *)node)->freeValue != NULL) {
        ((HNODE *)node)->freeValue(((HNODE *)node)->value);
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
static int hash(HASHMAP *map, void *key);


/********** Public Method Definitions **********/

HASHMAP *newHASHMAP(void) {
    HASHMAP *map = malloc(sizeof(HASHMAP));
    assert(map != NULL);
    map->size = 0;
    map->capacity = INITIAL_CAPACITY;
    map->loadFactor = DEFAULT_LOAD_FACTOR;
    map->debugLevel = 0;
    // create store and initialize with singly-linked lists
    map->store = newDA();
    for (int i = 0; i < map->capacity; ++i) {
        insertDAback(map->store, newSLL(displayHNODE, freeHNODE));
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

void insertHASHMAP(HASHMAP *map, void *key, void *value) {
    assert(map != NULL);
    assert(key != NULL);
    // grow the store if the size of the map exceeds the calculated threshold
    if (map->size > thresholdHASHMAP(map)) {
        /* grow(map); */
    }
    // create HNODE for the key/value pair
    HNODE *node = newHNODE(key, value);
    setHNODEdisplayKey(node, map->displayKey);
    setHNODEdisplayValue(node, map->displayValue);
    setHNODEfreeKey(node, map->freeKey);
    setHNODEfreeValue(node, map->freeValue);
    // get hash value
    int index = hash(map, key);
    // get sll chain at correct hash index
    SLL *chain = getDA(map->store, index);
    // insert key/value into correct spot
    insertSLL(chain, sizeSLL(chain), node);
    map->size++;
}

void *getHASHMAPvalue(HASHMAP *map, void *key) {
    assert(map != NULL);
    assert(key != NULL);
    int index = hash(map, key);
    SLL *chain = getDA(map->store, index);
    for (int i = 0; i < sizeSLL(chain); ++i) {
        if (((HNODE *)getSLL(chain, i))->key == key) {
            return ((HNODE *)getSLL(chain, i))->value;
        }
    }
    // if key is not found, return NULL
    return NULL;
}

void clearHASHMAP(HASHMAP *map) {
    assert(map != NULL);
    // clear the store
    for (int i = 0; i < map->capacity; ++i) {
        freeSLL(getDA(map->store, i));
    }
    freeDA(map->store);
    // reset fields
    map->size = 0;
    map->capacity = INITIAL_CAPACITY;
    // create store and initialize with singly-linked lists
    map->store = newDA();
    for (int i = 0; i < map->capacity; ++i) {
        insertDAback(map->store, newSLL(displayHNODE, freeHNODE));
    }
    shrinkToFitDA(map->store);
}

bool containsKey(HASHMAP *map, void *key) {
    assert(map != NULL);
    assert(key != NULL);
    int index = hash(map, key);
    SLL *chain = getDA(map->store, index);
    printf("index: %d\n", index);
    for (int i = 0; i < sizeSLL(chain); ++i) {
        if (((HNODE *)getSLL(chain, i))->key == key) {
            return true;
        }
    }
    return false;
}

bool isHASHMAPempty(HASHMAP *map) {
    assert(map != NULL);
    return map->size == 0;
}

int sizeHASHMAP(HASHMAP *map) {
    assert(map != NULL);
    return map->size;
}

void displayHASHMAP(HASHMAP *map, FILE *fp) {
    assert(map != NULL);
    if (map->debugLevel > 0) {
        fprintf(fp, "Size: %d\n", map->size);
        fprintf(fp, "Capacity: %d\n", map->capacity);
        fprintf(fp, "Load Factor: %f\n", map->loadFactor);
        fprintf(fp, "Threshold: %d\n", thresholdHASHMAP(map));
    }
    fprintf(fp, "[");
    for (int i = 0; i < map->capacity; ++i) {
        fprintf(fp, "%d: ", i);
        if (map->debugLevel > 0) {
            displaySLLdebug(getDA(map->store, i), fp);
        }
        else displaySLL(getDA(map->store, i), fp);
        if (i < map->capacity - 1) fprintf(fp, ", ");
    }
    fprintf(fp, "]");
}

int debugHASHMAP(HASHMAP *map, int level) {
    assert(map !=NULL);
    assert(level >= 0);
    map->debugLevel = level;
    return debugDA(map->store, level);
}

void freeHASHMAP(HASHMAP *map) {
    assert(map != NULL);
    for (int i = 0; i < map->capacity; ++i) {
        freeSLL(getDA(map->store, i));
    }
    freeDA(map->store);
    free(map);
}


/********** Private Method Definitions **********/

static int thresholdHASHMAP(HASHMAP *map) {
    assert(map != NULL);
    return map->capacity * map->loadFactor;
}

static int hash(HASHMAP *map, void *key) {
    assert(key != NULL);
    long address = (long) key;
    return address % map->capacity;
}
