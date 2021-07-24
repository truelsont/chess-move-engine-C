#ifndef LINKEDLIST_HEADER
#define LINKEDLIST_HEADER

#include <stdio.h>
#include <stdlib.h> 
#include <stdbool.h> 

typedef void print_data_fn(void *x); 

struct node
{
    void *data;
    void *next;
};


typedef struct node* node_t;

struct list
{
    node_t head; 
    node_t tail; 
};
typedef struct list* list_t;

list_t  list_make(){
    list_t RET = malloc(sizeof(struct list)); 
    RET->head = NULL; 
    RET->tail = NULL;  
    return RET; 
}
bool list_empty(list_t list){
    if(list->head != NULL) return false; 
    return true; 
}

void list_add(void *data, list_t list){

    /*this will be the new tail as we have a stack structure*/ 
    node_t NEW = malloc(sizeof(struct node)); 
    NEW->data = data;

    node_t head = list->head; 
    NEW->next = head;
    list->head = NEW; 
    
    return;  
}

void *list_pop(list_t list){
    if(list_empty(list)) {return NULL;}
    node_t T = list->head; 
    void *RET = T->data; 
    if(list->head == list->tail){list->head = NULL; list->tail = NULL; }
    list->head = T->next;
    free(((void *)T)); 
    return RET; 
}


void list_display(list_t L, print_data_fn *F){
    if(L == NULL) { printf("NULL LIST ERROR"); return; }
    printf("[\n"); 
    node_t CUR = L->head; 
    while (CUR != NULL)
    {
        F(CUR->data); 
        printf("=>\n");
        CUR = CUR->next; 
    }
    printf("]\n");
    return; 
}

void sub_rev(node_t N, node_t NEXT){
    if(NEXT == NULL){ return; } 
    node_t TEMP = NEXT->next;
    NEXT->next = N; 
    sub_rev(NEXT, TEMP); return; 
}

void list_reverse(list_t L){
    node_t BEH = NULL;
    node_t CUR = L->head; 
    while(CUR != NULL){
        node_t HOLD = CUR->next; 
        CUR->next = BEH; 
        BEH = CUR; 
        CUR = HOLD; 
    }
    
    node_t tail = L->tail; 
    L->head = tail; 
    L->tail = NULL;
    return; 
}


void list_free(list_t L, void (*F) (void *)){ 
    if(L == NULL){ return; } 
    node_t head = L->head; 
    while(head != NULL){
        node_t next = head->next; 
        F(head->data); 
        free(head); 
        head = next;
    }
    free(L); 
    return; 
}

int list_size(list_t L){
    int ret = 0; 
    list_t temp = list_make(); 
    
    while(!list_empty(L)){
        list_add(list_pop(L), temp); 
        ret += 1; 
    }
    while(!list_empty(temp)){
        list_add(
            list_pop(temp), L); 
    }
    free(temp); 
    return ret; 
}

#endif
