#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include "llist.h"

char _heap[MEMSIZE] = {0};
TNode *_memlist = NULL; // To maintain information about length

// Do not change this. Used by the test harness.
// You may however use this function in your code if necessary.
long get_index(void *ptr) {
    if(ptr == NULL)
        return -1;
    else
        return (long) ((char *) ptr - &_heap[0]);
}

// Allocates size bytes of memory and returns a pointer
// to the first byte.
void *mymalloc(size_t size) {
    if (_memlist == NULL) {
        TData *data1 = malloc(sizeof(TData));
        data1->memsize = size;
        data1->start_address = 0;
        data1->status = 'o';
        TNode *node1 = make_node(0, data1);
        insert_node(&_memlist, node1, 0);

        TData *data2 = malloc(sizeof(TData));
        data2->memsize = 64 * 1024 - size;
        data2->start_address = size;
        data2->status = 'u';
        TNode *node2 = make_node(data2->start_address, data2);
        insert_node(&_memlist, node2, 0);
        return &_heap[0];
    } else {
        reset_traverser(_memlist, FRONT);
        TNode *worstFit, *trav;
        int furthest = size;

        // Finding the worst fit
        do {
            trav = succ(_memlist);
            if (trav == NULL) {
                break;
            }
            TData *data = trav->pdata;
            if (data->status == 'u' && data->memsize >= size) {
                if (data->memsize - size > furthest - size) {
                    furthest = data->memsize;
                    worstFit = trav;
                }
            }
        } while (trav != NULL);

        // Returning worst fit
        if (worstFit->pdata->memsize != size) {
            TData *newData = malloc(sizeof(TData));
            newData->memsize = worstFit->pdata->memsize - size;
            newData->start_address = worstFit->pdata->start_address + size;
            newData->status = 'u';
            TNode *newNode = make_node(newData->start_address, newData);
            insert_node(&_memlist, newNode, 0);
        }
        worstFit->pdata->status = 'o';
        worstFit->pdata->memsize = size;
        return &_heap[worstFit->pdata->start_address];
    }
}

// Frees memory pointer to by ptr.
void myfree(void *ptr) {
    long position = get_index(ptr);
    TNode *node = find_node(_memlist, position);
    if (node == NULL) {
        return;
    }
    node->pdata->status = 'u';
    reset_traverser(_memlist, FRONT);
    TNode *trav = succ(_memlist);
    TNode *nextNode;
    while (trav != NULL) {
        if (trav->pdata->status == 'u') {
            nextNode = trav->next;
            while (nextNode != NULL && nextNode->pdata->status == 'u') {
                trav->pdata->memsize += nextNode->pdata->memsize;
                delete_node(&_memlist, nextNode);
                nextNode = trav->next;
            }
        }
        trav = succ(_memlist);
    }
}

// Helper function to print a single node
void print_node(TNode *node) {
    if (node != NULL) {
        if (node->pdata->status == 'u') {
            printf("Status: FREE Start index: %d Length: %d\n", node->pdata->start_address, node->pdata->memsize);
        } else {
            printf("Status: ALLOCATED Start index: %d Length: %d\n", node->pdata->start_address, node->pdata->memsize);
        }
    } else {
        return;
    }
}

// Prints the entire memlist
void print_memlist() {
    process_list(_memlist, print_node);
}