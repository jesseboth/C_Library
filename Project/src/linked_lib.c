#include "linked_lib.h"
#include "utility/print.h"

#define sizeof(Meta) 16

typedef struct Meta{
    size_t length;
    uint8_t type;
    uint8_t datasize;

    void *list;
}Meta;

typedef union Data{
    void *data;
    char character;
    int i;
}Data;



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

void *LinkedList_insert(void *list, void *data, size_t index){
    void *n = NULL;
    if(data == NULL || list == NULL){
        return NULL;
    }

    Meta *meta = list - sizeof(Meta);
    if(index > meta->length){
        printf("Invalid index\n");
        return NULL;
    }

    if(meta->type == SINGLE){
        SinglyLinkedList *s = list;
        SinglyNode *node = malloc(sizeof(SinglyNode));
        n = node;
        node->data = data;

        SinglyNode *iter = s->head;

        if(s->head == NULL){
            node->next = NULL;
            s->head = node;

            goto end;
        }
        
        for(int i = 0; i+1 < index; i++, iter = iter->next);

        if(index == 0){
            s->head = node; 
            node->next = iter;   
        }
        else{
            SinglyNode *temp = iter->next;
            iter->next = node;
            node->next = temp;
        }


    }
    else if(meta->type == DOUBLE){
        DoublyLinkedList *d = list;
        DoublyNode *node = malloc(sizeof(DoublyNode));
        n = node;
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
            for(int i = 0; i+1 < index; i++, iter = iter->next);
        }

        if(index == 0){
            d->head->prev = node; 
            node->next = d->head;
            d->head = node;
        }
        else{
            DoublyNode *temp = iter->next;
            if(temp) iter->next->prev = node;
            iter->next = node;
            node->next = temp;
            node->prev = iter;
        }
    }

end:
    meta->length++;
    return n;
}

void *LinkedList_prepend(void *list, void *data){
    return LinkedList_insert(list, data, 0);
}

void *LinkedList_append(void *list, void *data){
    Meta *meta = list - sizeof(Meta);
    return LinkedList_insert(list, data, meta->length);
}

void *LinkedList_insertionSort(void *list, void *data){
    void *n = NULL;
    if(data == NULL || list == NULL){
        return NULL;
    }

    Meta *meta = list - sizeof(Meta);

    if(meta->type == SINGLE){
        SinglyLinkedList *s = list;
        SinglyNode *node = malloc(sizeof(SinglyNode));
        n = node;
        node->data = data;

        SinglyNode *iter = s->head;

        if(s->head == NULL){
            node->next = NULL;
            s->head = node;

            goto end;
        }
        
        int i = 0;
        for(;iter->next && iter->next->data < data; i++, iter = iter->next);

        if(i == 0){
            s->head = node; 
            node->next = iter;   
        }
        else{
            SinglyNode *temp = iter->next;
            iter->next = node;
            node->next = temp;
        }
    }
    else if(meta->type == DOUBLE){
        DoublyLinkedList *d = list;
        DoublyNode *node = malloc(sizeof(DoublyNode));
        n = node;
        node->data = data;

        DoublyNode *iter = d->head;

        if(d->head == NULL){
            node->next = NULL;
            node->prev = NULL;
            d->head = node;
            d->tail = node;
            goto end;
        }

        int i = 0;
        for(;iter->next && iter->next->data < data; i++, iter = iter->next);
        
        if(i == 0){
            d->head->prev = node; 
            node->next = d->head;
            d->head = node;
        }
        else{
            DoublyNode *temp = iter->next;
            if(temp) iter->next->prev = node;
            iter->next = node;
            node->next = temp;
            node->prev = iter;
        }
    }
end:
    meta->length++;
    return n;
}

static void Node_print(void *node, uint8_t datasize){
    if(!node) return;
    Data d = (Data)((SinglyNode *)node)->data;

    if(datasize == sizeof(uint8_t)){
        printf("%c", d.character);
    }
    else if(datasize <= sizeof(uint32_t)){
        printf("%d", d.i);
    }
    else{
        printf("%p", d.data);
    }
}

void LinkedList_print(void *list){
    Meta *meta = list - sizeof(Meta);
    char *mid;
    if(meta->type == SINGLE){
        mid = " -> ";
    }
    else if(meta->type == DOUBLE){
        mid = " <-> ";
    }

    SinglyNode *node = ((SinglyLinkedList *)list)->head;

    printf("Length: %lu\n", meta->length);
    while(node){
        Node_print(node, meta->datasize);
        (node->next) ? printf("%s", mid) : printf("\n\n");
        node = node->next;
    }

}

void LinkedList_reversePrint(void *list){
    Meta *meta = list - sizeof(Meta);
    char *mid = " <-> ";
    if(meta->type == SINGLE){
        return;
    }

    DoublyNode *node = ((DoublyLinkedList *)list)->tail;

    printf("Length: %lu\n", meta->length);
    while(node){
        Node_print(node, meta->datasize);
        (node->prev) ? printf("%s", mid) : printf("\n\n");
        node = node->prev;
    }

}

static void Node_swap(void *src, void *src_prev, void *dest, void *dest_prev, uint8_t type){
    if(src == dest){
        return;
    }

    DoublyNode *nsrc = src;
    DoublyNode *ndest = dest;
    DoublyNode *nsrc_p = src_prev;
    DoublyNode *ndest_p = dest_prev;

    void *temp;

    if(type == DOUBLE){
        if(ndest->next == nsrc){
            if(nsrc->next) nsrc->next->prev = ndest;

            nsrc->prev = ndest->prev;
            ndest->prev = nsrc;

        }
        else{


            if(nsrc->next) nsrc->next->prev = ndest;
            if(ndest->next) ndest->next->prev = nsrc;

            temp = nsrc->prev;
            nsrc->prev = ndest->prev;
            ndest->prev = temp;
        }

    }

    if(ndest->next == nsrc){
        temp = nsrc->next;
        ndest->next = nsrc->next;
        nsrc->next = ndest;

        if(ndest_p) ndest_p->next = nsrc;
        ndest->next = temp;
    }
    else{
        temp = nsrc->next;
        nsrc->next = ndest->next;
        ndest->next = temp;

        temp = nsrc_p->next;
        nsrc_p->next = ndest;
        if(ndest_p) ndest_p->next = temp;
    }
}

int LinkedList_Node_swap(void *list, void *src, void *dest){
    Meta *meta = list - sizeof(Meta);
    SinglyNode *node = meta->list;
    
    void *temp;
    if(meta->type == DOUBLE){
        while(node && node != src && node != dest){
            node = node->next;
        }
        if(!node) return 0;
        else if(node == src){
            temp = src;
            src = dest;
            dest = temp;
        }
        Node_swap(src, ((DoublyNode *)src)->prev, dest, ((DoublyNode *)dest)->prev, meta->type);
    }
    else if(meta->type == SINGLE){
        SinglyNode *nsrc, *nsrc_p, *ndest, *ndest_p;
        ndest = NULL;
        while(node && node != src && node != dest){
            ndest_p = ndest;
            ndest = node;
            node = node->next;
        }
        if(!node) return 0;
        while(node && node != src && node != dest){
            nsrc_p = ndest;
            nsrc = node;
            node = node->next;
        }
        if(!node) return 0;
        Node_swap(nsrc, nsrc_p, ndest, ndest_p, meta->type);
    }
    return 1;
}

void LinkedList_sort(void *list){
    Meta *meta = list - sizeof(Meta);
    SinglyNode *set, *check, *prev_set, *prev_check;
    SinglyNode *home = ((SinglyLinkedList *)list)->head;
    SinglyNode *prev_home = NULL;

    for(int i = 0; home; i++){
        void *data = home->data;
        set = NULL;
    
        check = home->next;
        prev_check = home;

        while(check){
            if(check->data < data){
                data = check->data;
                set = check;
                prev_set = prev_check;
            }
            prev_check = check;
            check = check->next;
        }
        
        if(set){
            if(i == 0){
                ((SinglyLinkedList *)list)->head = set;
            }
        
            if(home){
                Node_swap(set, prev_set, home, prev_home, meta->type);
            }

            if(i+1 < meta->length){
                home = set;
            }
        }
        
        prev_home = home;
        if(!(home->next) && meta->type == DOUBLE){
            ((DoublyLinkedList *)list)->tail = (DoublyNode *)home;
        }
        home = home->next;
    }
}

static void *Node_reverse(void *ptr, uint8_t type){
    if(type == DOUBLE){}
    else if(type == SINGLE){
        SinglyNode *node = ptr;
        if(!node || !(node->next))  return node;
        void *res = Node_reverse(node->next, type);
        node->next->next = node;
        node->next = NULL;
        return res;
    }
    return NULL;
}

void LinkedList_reverse(void *list){
    Meta *meta = list - sizeof(Meta);
    DoublyLinkedList *l = meta->list;
    l->head = Node_reverse(l->head, meta->type);
}


/* If data inside node is malloc'd, it is the 
    users responsibility to free it */
void LinkedList_free(void *list){
    Meta *meta = list - sizeof(Meta);
    if(meta->type == DOUBLE){
        DoublyLinkedList *ll = meta->list;
        DoublyNode *node = ll->head;
        DoublyNode *next;
        while(node){
            next = node->next;
            node->next = NULL;
            node->prev = NULL;
            node->data = NULL;
            free(node);
            node = next;
        }

        ll->head = NULL;
        ll->tail = NULL;
    }
    else if(meta->type == SINGLE){
        SinglyLinkedList *ll = meta->list;
        SinglyNode *node = ll->head;
        SinglyNode *next;
        while(node){
            next = node->next;
            node->next = NULL;
            node->data = NULL;
            free(node);
            node = next;
        }
        ll->head = NULL;
    }

    meta->datasize = 0;
    meta->length = 0;
    meta->type = 0;
    meta->list = NULL;

    free(meta);
}

void test(){
    
    printf("Double\n");
    DoublyLinkedList *list = LinkedList_init(DOUBLE, sizeof(char));
    LinkedList_insert(list, (elem) 'a', 0);
    LinkedList_insert(list, (elem) 'b', 1);
    LinkedList_insert(list, (elem) 'c', 2);
    LinkedList_insert(list, (elem) 'x', 2);
    LinkedList_append(list, (elem) 'z');
    LinkedList_append(list, (elem) 'y');
    LinkedList_append(list, (elem) 'd');
    LinkedList_insert(list, (elem) 'f', 4);  
    LinkedList_prepend(list, (elem) 'j');
    LinkedList_insert(list, (elem) 'e', 5);  
      
    LinkedList_print(list);


    LinkedList_sort(list);

    LinkedList_insertionSort(list, (elem) 'd');

    LinkedList_print(list);
    LinkedList_reversePrint(list);
    
    LinkedList_free(list);

    printf("Single\n");
    list = LinkedList_init(SINGLE, sizeof(int *));
    LinkedList_insert(list, (elem) 'a', 0);
    LinkedList_insert(list, (elem) 'b', 1);
    LinkedList_insert(list, (elem) 'c', 2);
    LinkedList_insert(list, (elem) 'x', 2);
    LinkedList_append(list, (elem) 'z');
    LinkedList_append(list, (elem) 'y');
    LinkedList_append(list, (elem) 'd');
    LinkedList_insert(list, (elem) 'f', 4);  
    LinkedList_prepend(list, (elem) 'j');
    LinkedList_insert(list, (elem) 'e', 5);  
      
    LinkedList_print(list);

    LinkedList_sort(list);
    LinkedList_print(list);
    
    LinkedList_reverse(list);
    LinkedList_print(list);

    LinkedList_free(list);
}

