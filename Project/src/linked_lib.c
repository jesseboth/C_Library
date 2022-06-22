#include "linked_lib.h"
#include "print.h"

#define sizeof(Meta) 16

typedef struct Meta{
    size_t length;
    uint8_t type;
    uint8_t datasize;

    void *list;
}Meta;

void *LinkedList_init(uint8_t type, uint8_t datasize){
    void *list, *data;
    Meta *meta;

    if(type == SINGLE){
        data = malloc(sizeof(Meta) + sizeof(SinglyLinkedList));
        list = data + sizeof(Meta);

        SinglyLinkedList *s = (SinglyLinkedList *)list;

        s->head = NULL;
    }
    else if (type == DOUBLE){
        data = malloc(sizeof(Meta) + sizeof(DoublyLinkedList));
        list = data + sizeof(Meta);

        DoublyLinkedList *d = (DoublyLinkedList *)list;
        
        d->head = NULL;
        d->tail = NULL;
    }

    meta = (Meta *)data;

    meta->length = 0;
    meta->type = type;
    meta->datasize = datasize;
    meta->list = NULL;

    return list;
}

size_t LinkedList_length(void *list){
    Meta *meta = list - sizeof(Meta);
    return meta->length;
}

void LinkedList_insert(void *list, void *data, size_t index){
    if(data == NULL || list == NULL){
        return;
    }

    Meta *meta = list - sizeof(Meta);
    if(index > meta->length){
        printf("Invalid index\n");
        return;
    }

    if(meta->type == SINGLE){
        SinglyLinkedList *s = list;
        SinglyNode *node = malloc(sizeof(SinglyNode));
        node->data = data;

        SinglyNode *iter = s->head;

        if(s->head == NULL){
            node->next = NULL;
            s->head = node;

            goto end;
        }

        for(int i = 0; i < index-1; i++, iter = iter->next);

        if(index = 0){
            s->head = node;    
        }

        SinglyNode *temp = iter->next;
        iter->next = node;
        node->next = temp;

    }
    else if(meta->type == DOUBLE){
        DoublyLinkedList *d = list;
        DoublyNode *node = malloc(sizeof(DoublyNode));
        node->data = data;

        DoublyNode *iter;

        if(d->head == NULL){
            node->next = NULL;
            node->prev = NULL;
            d->head = node;
            d->tail = node;
            goto end;
        }


        if(meta->length - index < index){
            iter = d->tail;
            for(int i = meta->length; i > index; i--, iter = iter->prev);

            if(meta->length == index){
                d->tail = node;
            }
        }
        else{
            iter = d->head;
            for(int i = 0; i < index-1; i++, iter = iter->next);
            if(index == 0){
                d->head = node;
            }
        }

        if(index = 0){
            d->head = node;    
        }
        DoublyNode *temp = iter->next;
        iter->next = node;
        node->next = temp;
        node->prev = iter;
    }

end:
    meta->length++;
}

void LinkedList_append(void *list, void *data){

}

void test(){
    DoublyLinkedList *list = LinkedList_init(DOUBLE, sizeof(char));
    LinkedList_insert(list, (char)'a', 0);
    LinkedList_insert(list, (char)'b', 1);
    LinkedList_insert(list, (char)'c', 2);
    LinkedList_insert(list, (char)'x', 2);
    LinkedList_insert(list, (char)'z', 5);
    
    DoublyNode *node = list->head;
    while(node){
        printf("%p: %c\n", node, node->data);
        node = node->next;
    }
    printf("length: %u\n", LinkedList_length(list));
}

