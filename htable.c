/*
 *  Author: Brett Heithold
 *  File:   htable.c
 */


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

void setHTNODEkeyDisplay(HTNODE *node, void (*display)(void *, FILE *)) {
    assert(node != NULL);
    node->displayKey = display;
}

void setHTNODEkeyFree(HTNODE *node, void (*free)(void *)) {
    assert(node != NULL);
    node->freeKey = free;
}

void setHTNODEvalueDisplay(HTNODE *node, void (*display)(void *, FILE *)) {
    assert(node != NULL);
    node->displayValue = display;
}

void setHTNODEvalueFree(HTNODE *node, void (*free)(void *)) {
    assert(node != NULL);
    node->freeValue = free;
}

void displayHTNODE(HTNODE *node, FILE *fp) {
    assert(node != NULL);
    fprintf(fp, "(");
    node->displayKey(node->key, fp);
    fprintf(fp, " : ");
    node->displayValue(node->value, fp);
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

    void (*display)(void *, FILE *);
    void (*free)(void *);
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

void setHTABLEdisplay(HTABLE *table, void (*display)(void *, FILE *)) {
    assert(table != NULL);
    table->display = display;
}

void setHTABLEfree(HTABLE *table, void (*free)(void *)) {
    assert(table != NULL);
    table->free = free;
}


/********** Private Method Definitions **********/
