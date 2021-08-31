#include "queue.h"

int test_linked_list(){
    linked_list *list = new_linked_list();

    linked_list_append(list, "test");   // 2
    linked_list_prepend(list, "test2"); // 1
    linked_list_append(list, "test3");  // 3
    linked_list_prepend(list, "test4"); // 0

    if(linked_list_index(list, 2) != "test"){
        printf("Indexing fail");
        return 1;
    }
    else if(linked_list_index(list, 0) != "test4"){
        printf("Append Fail (index 0)");
        return 1;
    }
    else if(linked_list_index(list, 1) != "test2"){
        printf("Append Fail (index 1)");
        return 1;
    }
    else if(linked_list_index(list, 3) != "test3"){
        printf("Prepend Fail (index 4)");
        return 1;
    }

    print_linked_list(list);
    free_linked_list(list);
    return 0;
}

int test_queue(){
    queue *q = new_queue();

    enqueue(q, "test");
    enqueue(q, "test1");
    enqueue(q, "test2");
    enqueue(q, "test3");
    enqueue(q, "test4");
    print_queue(q);

    if(peek(q) != "test"){
        printf("Bad Head");
        return 1;
    }

    if(dequeue(q) != "test" || peek(q) != "test1"){
        printf("Bad Dequeue %s", peek(q));
        return 1;
    }
    else if(dequeue(q) != "test1" || peek(q) != "test2"){
        printf("Bad Dequeue %s", peek(q));
        return 1;
    }

    enqueue(q, "test5");

    dequeue(q);
    dequeue(q);
    dequeue(q);

    if(peek(q) != "test5"){
        printf("Bad enqueue after dequeue");
    }

    if(dequeue(q) != "test5" && peek(q) != NULL && dequeue(q) != NULL){
        printf("Bad end");
    }
    
    free_queue(q);

}

int main(int argc, char *argv){

    printf("LINKED LIST\n");
    test_linked_list();

    printf("\nQUEUE\n");
    test_queue();

    return 0;
}