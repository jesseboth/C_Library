#include "linked_list.h"

typedef struct queue{
    struct linked_list *list;
}queue;

queue *new_queue(){
    queue *ret = malloc(sizeof(queue));
    linked_list *l = malloc(sizeof(linked_list));

    ret->list = l;
    return ret;   
}

void enqueue(queue *q, linked_list_element val){
    linked_list_append(q->list, val);
}

linked_list_element dequeue(queue *q){
    linked_list_element ret = q->list->head->val;
    if(q->list->head != NULL){
        q->list->head = q->list->head->next;
    }
    return ret;
}

linked_list_element peek(queue *q){
    return q->list->head->val;
}

void print_queue(queue *q){
    print_linked_list(q->list);
}

void free_queue(queue *q){
    free_linked_list(q->list);
    free(q);
}