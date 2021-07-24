#ifndef LINKEDLIST_HEADER
#define LINKEDLIST_HEADER

#include <stdio.h>
#include <stdlib.h> 
#include <stdbool.h> 

typedef struct list* list_t; 
typedef void print_data_fn(void *x); 

list_t list_make();

void list_add(void *data, list_t list);

bool list_empty(list_t list); 

void *list_pop(list_t list);

void list_display(list_t list, print_data_fn *F);

void list_reverse(list_t list);

void list_free(list_t list, void (*F) (void *));

int list_size(list_t list); 

#endif
