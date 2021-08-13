#include "linked_list.h"

void test_linked_list(){
    linked_list *list = new_linked_list();

    linked_list_append(list, "test");
    linked_list_prepend(list, "test2");
    linked_list_append(list, "test3");
    linked_list_prepend(list, "test4");

    printf("%s\n", linked_list_index(list, 2));

    print_linked_list(list);
    free_linked_list(list);
}

int main(int argc, char *argv){

    test_linked_list();
    return 0;
}