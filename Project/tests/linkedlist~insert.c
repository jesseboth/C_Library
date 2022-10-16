#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "../src/linked_lib.h"

int main(int argc, char *argv[]) {

    DoublyLinkedList *list = LinkedList_init(DOUBLE, sizeof(char));
    DoublyNode *node;
    /* append */
    LinkedList_append(list, (elem) 'a');
    LinkedList_append(list, (elem) 'b');
    LinkedList_append(list, (elem) 'c');

    node = list->head;
    void  *check = (void *)'a';
    while(node){
        if(node->data != check++){
            return 1;
        }
    }

    // LinkedList_insert(list, (elem) 'b', 1);
    // LinkedList_insert(list, (elem) 'c', 2);
    // LinkedList_insert(list, (elem) 'x', 2);
    // LinkedList_append(list, (elem) 'z');
    // LinkedList_append(list, (elem) 'y');
    // LinkedList_append(list, (elem) 'd');
    // LinkedList_insert(list, (elem) 'f', 4);  
    // LinkedList_prepend(list, (elem) 'j');
    // LinkedList_insert(list, (elem) 'e', 5);  
      
    // LinkedList_print(list);

    LinkedList_free(list);




    return 0;
}