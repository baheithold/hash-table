/*
 *  Author: Brett Heithold
 *  File:   htable.c
 *  Last Modified:  5 Feb 2020
 */


#include "da.h"
#include "htable.h"
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
} HTNODE;

HTNODE *newHTNODE(void *key, void *value) {
    HTNODE *node = malloc(sizeof(HTNODE));
    node->key = key;
    node->value = value;
    return node;
}

void setHTNODEdisplayKey(HTNODE *node, void (*display)(void *, FILE *)) {
    assert(node != NULL);
    node->displayKey = display;
}

void setHTNODEfreeKey(HTNODE *node, void (*free)(void *)) {
    assert(node != NULL);
    node->freeKey = free;
}

void setHTNODEdisplayValue(HTNODE *node, void (*display)(void *, FILE *)) {
    assert(node != NULL);
    node->displayValue = display;
}

void setHTNODEfreeValue(HTNODE *node, void (*free)(void *)) {
    assert(node != NULL);
    node->freeValue = free;
}

void displayHTNODE(HTNODE *node, FILE *fp) {
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

void freeHTNODE(HTNODE *node) {
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

struct HTABLE {
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

HTABLE *newHTABLE(void) {
    HTABLE *table = malloc(sizeof(HTABLE));
    assert(table != NULL);
    table->size = 0;
    table->debugLevel = 0;
    return table;
}

void setHTABLEdisplayKey(HTABLE *table, void (*display)(void *, FILE *)) {
    assert(table != NULL);
    table->displayKey= display;
}

void setHTABLEdisplayValue(HTABLE *table, void (*display)(void *, FILE *)) {
    assert(table != NULL);
    table->displayValue = display;
}

void setHTABLEfreeKey(HTABLE *table, void (*free)(void *)) {
    assert(table != NULL);
    table->freeKey= free;
}

void setHTABLEfreeValue(HTABLE *table, void (*free)(void *)) {
    assert(table != NULL);
    table->freeValue = free;
}

int isHTABLEempty(HTABLE *table) {
    assert(table != NULL);
    return table->size == 0;
}

int sizeHTABLE(HTABLE *table) {
    assert(table != NULL);
    return table->size;
}

int debugHTABLE(HTABLE *table, int level) {
    assert(table !=NULL);
    assert(level >= 0);
    table->debugLevel = level;
    debugDA(table->store, level);
}

void freeHTABLE(HTABLE *table) {
    assert(table != NULL);
    freeDA(table->store);
}


/********** Private Method Definitions **********/
