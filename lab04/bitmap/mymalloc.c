#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"
#include "llist.h"
#include "mymalloc.h"

char _heap[MEMSIZE] = {0};
unsigned char bitmap[MEMSIZE/64];
TNode *linklist = NULL;

// Do not change this. Used by the test harness.
// You may however use this function in your code if necessary.
long get_index(void *ptr) {
    if(ptr == NULL)
        return -1;
    else
        return (long) ((char *) ptr - &_heap[0]);
}

void print_memlist() {
    // Implement this to call print_map from bitmap.c
    print_map(bitmap, MEMSIZE/8);
}

// Allocates size bytes of memory and returns a pointer
// to the first byte.
void *mymalloc(size_t size) {
    long address = search_map(bitmap, MEMSIZE/8, size);
    if (address != -1) {
        allocate_map(bitmap, address, size);
        TData *data = malloc(sizeof(TData));
        data->len = size;
        TNode *newNode = make_node(address, data);
        insert_node(&linklist, newNode, 0);
        return &_heap[address];
    } else {
        return NULL;
    }
}

// Frees memory pointer to by ptr.
void myfree(void *ptr) {
    long position = get_index(ptr);
    TNode *removedNode = find_node(linklist, position);
    if (removedNode == NULL) {
        return;
    }
    free_map(bitmap, get_index(ptr), removedNode->pdata->len);
    delete_node(&linklist, removedNode);
}