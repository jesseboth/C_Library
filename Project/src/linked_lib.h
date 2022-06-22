#include "stdint.h"
#include "stdlib.h"

#define SINGLE 0
#define DOUBLE 1

typedef struct SinglyNode{
    struct SinglyNode *next;
    void *data;
}SinglyNode;

typedef struct DoublyNode{
    struct DoublyNode *next;
    void *data;
    struct DoublyNode *prev;
}DoublyNode;

typedef struct SinglyLinkedList{
    SinglyNode *head;
}SinglyLinkedList;

typedef struct DoublyLinkedList{
    DoublyNode *head;
    DoublyNode *tail;
}DoublyLinkedList;