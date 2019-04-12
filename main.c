#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "tasks.h"
#include "structs.h"

int main(){
    fprintf(stdout,"Created exe\n");
    test_generic_list();
    return 0;
}

void test_generic_list(){
    list_t* my_list = initialize_list();
    users_node_t* head_users_node = initialize_users_node();
    users_node_t* tail_users_node = initialize_users_node();

    my_list->head = head_users_node;
    my_list->tail = tail_users_node; 

    head_users_node->user_index = 5;
    tail_users_node->user_index = 18;

    fprintf(stdout,"Head: %d\nTail:\n",((users_node_t*)my_list->head)->user_index);
}