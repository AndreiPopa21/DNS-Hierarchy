#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "tasks.h"
#include "structs.h"

void test();

int main(){
    fprintf(stdout,"Created exe\n");

    test();

    fprintf(stdout,"Finished exe\n");

    return 0;
}

void test(){

    dns_server_t* first = initialize_dns_server(1);
    dns_server_t* second = initialize_dns_server(2);
    dns_server_t* third = initialize_dns_server(3);
    dns_server_t* fourth = initialize_dns_server(4);

    list_t* list = initialize_list();
    push_back_dns_list(&list,first);
    push_back_dns_list(&list,second);
    push_back_dns_list(&list,third);
    push_back_dns_list(&list,fourth);

    print_dns_list(&list);

    
}

