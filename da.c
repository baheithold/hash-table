#include "da.h"
#include <assert.h>
#include <stdlib.h>

#define GROWTH_FACTOR 2
#define MIN_SIZE_CAPACITY_RATIO 0.25
#define ARRAY_FRONT 0


/********** Dynamic Array Struct **********/

struct DA {
    int capacity;
    int size;
    void **store;
    int debugLevel;

    void (*display)(void *, FILE *);
    void (*free)(void *);
};


/********** Private Method Prototypes **********/
static void grow(DA *items);
static void shrink(DA *items);
static void addToFront(DA *items, void *value);
static void addToBack(DA *items, void *value);
static void addBetweenFrontAndBack(DA *items, int index, void *value);
static void shiftValuesRightOfIndex(DA *items, int index);
static void *removeFromFront(DA *items);
static void *removeFromBack(DA *items);
static void *removeBetweenFrontAndBack(DA *items, int index);


/********** Public Method Definitions **********/

DA *newDA(void) {
    DA *da = malloc(sizeof(DA));
    assert(da != NULL);
    da->capacity = 1;
    da->size = 0;
    da->store = malloc(sizeof(void **));
    da->debugLevel = 0;
    return da;
}

void setDAdisplay(DA *items, void (*display)(void *, FILE *)) {
    assert(items != NULL);
    items->display = display;
}

void setDAfree(DA *items, void (*free)(void *)) {
    assert(items != NULL);
    items->free = free;
}

void insertDA(DA *items, int index, void *value) {
    assert(items != NULL);
    assert(index >= 0);

    // if the store is full, grow the store
    if (items->size == items->capacity) {
        grow(items);
    }

    if (index == ARRAY_FRONT) {
        // inserting value at front of array
        addToFront(items, value);
    }
    else if (index == items->size) {
        // inserting value at back of array
        addToBack(items, value);
    }
    else {
        // inserting between front and back of array
        addBetweenFrontAndBack(items, index, value);
    }

    // increment the size of the array
    items->size++;
}

void *removeDA(DA *items, int index) {
    assert(items != NULL);
    assert(items->size > 0);
    void *oldValue;
    if (index == ARRAY_FRONT) {
        // remove from front of array
        oldValue = removeFromFront(items);
    }
    else if (index == items->size) {
        // remove from back of array
        oldValue = removeFromBack(items);
    }
    else {
        // remove between front and back of array
        oldValue = removeBetweenFrontAndBack(items, index);
    }
    items->size--;
    // shrink if necessary
    if ((double)items->size / (double)items->capacity < MIN_SIZE_CAPACITY_RATIO) {
        shrink(items);
    }
    // return removed value
    return oldValue;
}

void unionDA(DA *recipient, DA *donor) {
    assert(recipient != NULL);
    assert(donor != NULL);
    for (int i = 0; i < donor->size; ++i) {
        insertDA(recipient, recipient->size, removeDA(donor, 0));
    }
    free(donor);
}

void *getDA(DA *items, int index) {
    assert(items != NULL);
    assert(index >= 0);
    assert(index < items->size);
    return items->store[index];
}

void *setDA(DA *items, int index, void *value) {
    assert(items != NULL);
    assert(index >= 0);
    assert(index <= items->size);
    void *oldValue = NULL;
    // if index is less than the current size of the array
    // save the old value to return later and set the new value
    if (index < items->size) {
        oldValue = items->store[index];
        items->store[index] = value;
    }
    // else add the new value to the back of the array
    else {
        insertDA(items, items->size, value);
    }
    // return the old value
    return oldValue;
}

int sizeDA(DA *items) {
    assert(items != NULL);
    return items->size;
}

void displayDA(DA *items, FILE *fp) {
    assert(items != NULL);
    fprintf(fp, "[");
    for (int i = 0; i < items->size; ++i) {
        // if no display function was provided, print the address
        if (items->display == NULL) {
            fprintf(fp, "%p", items->store[i]);
        }
        // else use the provided display function to print the object
        else {
            items->display(items->store[i], fp);
        }
        // if index is less than the size of the array
        if (i < items->size - 1) {
            fprintf(fp, ",");
        }
    }
    // if debug level is greater than zero
    // print the number of empty slots in the array
    if (items->debugLevel > 0) {
        if (items->size > 0) fprintf(fp, ",");
        fprintf(fp, "[%d]", items->capacity - items->size);
    }
    fprintf(fp, "]");
}

int debugDA(DA *items, int level) {
    assert(items != NULL);
    assert(level >= 0);
    int oldLevel = items->debugLevel;
    items->debugLevel = level;
    return oldLevel;
}

void freeDA(DA *items) {
    assert(items != NULL);
    if (items->free != NULL) {
        for (int i = 0; i < items->size; ++i) {
            items->free(items->store[i]);
        }
    }
    free(items->store);
    free(items);
}


/********** Private Method Definitions **********/

static void grow(DA *items) {
    assert(items != NULL);
    // Calculate new capacity
    int newCapacity = items->capacity * GROWTH_FACTOR;
    // realloc store
    items->store = realloc(items->store, sizeof(void *) * newCapacity);
    // Update the capacity
    items->capacity = newCapacity;
}

static void shrink(DA *items) {
    assert(items != NULL);
    // Calculate new capacity
    int newCapacity = (items->size == 0) ? 1 : items->capacity / GROWTH_FACTOR;
    // realloc store
    items->store = realloc(items->store, sizeof(void *) * newCapacity);
    // Update capacity
    items->capacity = newCapacity;
}

static void addToFront(DA *items, void *value) {
    assert(items != NULL);
    shiftValuesRightOfIndex(items, ARRAY_FRONT);
    items->store[ARRAY_FRONT] = value;
}

static void addToBack(DA *items, void *value) {
    assert(items != NULL);
    assert(items->size < items->capacity);
    items->store[items->size] = value;
}

static void addBetweenFrontAndBack(DA *items, int index, void *value) {
    assert(items != NULL);
    assert(items->size >= 0);
    assert(items->size < items->capacity);
    assert(index > 0);
    assert(index < items->size);
    shiftValuesRightOfIndex(items, index);
    items->store[index] = value;
}

static void shiftValuesRightOfIndex(DA *items, int index) {
    assert(items != NULL);
    assert(index >= 0);
    for (int i = items->size - 1; i >= index; --i) {
        items->store[i + 1] = items->store[i];
    }
}

static void *removeFromFront(DA *items) {
    assert(items != NULL);
    assert(items->size > 0);
    // get return value
    void *oldValue = items->store[ARRAY_FRONT];
    // shift values to the right
    for (int i = 0; i < items->size; ++i) {
        items->store[i] = items->store[i + 1];
    }
    // return old value
    return oldValue;
}

static void *removeFromBack(DA *items) {
    assert(items != NULL);
    assert(items->size > 0);
    // get return value
    void *oldValue = items->store[items->size];
    // remove value from back
    items->store[items->size] = NULL;
    // return old value
    return oldValue;
}

static void *removeBetweenFrontAndBack(DA *items, int index) {
    assert(items != NULL);
    assert(items->size > 0);
    assert(index > 0);
    assert(index < items->size);
    // get return value
    void *oldValue = items->store[index];
    // shift values to left
    for (int i = index; i < items->size - 1; ++i) {
        items->store[i] = items->store[i + 1];
    }
    // return old value
    return oldValue;
}
