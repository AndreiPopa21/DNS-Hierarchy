#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "tasks.h"
#include "structs.h"

void test();
void test_get_node_at(list_t** list,int position);

int main(){
    fprintf(stdout,"Created exe\n");

    //tree_construction();
    test();

    fprintf(stdout,"Finished exe\n");

    return 0;
}

void test(){

    dns_server_t* first = initialize_dns_server(1);
    dns_server_t* second = initialize_dns_server(2);
    dns_server_t* third = initialize_dns_server(3);
    dns_server_t* fourth = initialize_dns_server(4);

    push_back_dns_child(&first,&second);
    push_back_dns_child(&first,&fourth);
    push_back_dns_child(&first,&third);
    print_dns_server_childern(&first);
   // delete_at_dns_child(&first,0);
   // delete_at_dns_child(&first,0);
   // delete_at_dns_child(&first,0);
    //delete_at_dns_child(&first,0);
    //print_dns_server_childern(&first);
    //list_t* list = initialize_list();
    //push_back_dns_list(&list,second);
   // push_back_dns_list(&list,third);
    //push_back_dns_list(&list,fourth);
    //print_dns_list(&list);
    //print_dns_list_reverse(&list);
    //delete_at_dns_list(&list,0);
    //print_dns_list_reverse(&list);
    //push_back_dns_child(&first,&third);
    //push_back_dns_child(&first,&second);
    //push_back_dns_child(&second,&fourth);
    //print_dns_server_childern(&first);

/*
    push_back_dns_list(&list,first);
    push_back_dns_list(&list,third);
    push_back_dns_list(&list,fourth);

    print_dns_list(&list);
   // test_get_node_at(&list,4);
    print_dns_list(&list);

    free_dns_list(&list);*/
    //print_dns_list(&list);
    //free_dns_list(&list);
    //print_dns_list(&list);

    //free_dns_server(&first);
    //free_dns_server(&second);
    //free_dns_server(&third);
    //free_dns_server(&fourth);
}

void test_get_node_at(list_t** list,int position){
    fprintf(stdout,"Test get node at\n");
    dns_node_t* dns = get_dns_node_at(list,position);
    if(!dns){
        fprintf(stdout,"You received empty node\n");
    }else{
        fprintf(stdout,"Index of node at %d position is %d\n",position,dns->dns_server->server_index);
       // dns->dns_server->server_index = 1212;
    }
}

