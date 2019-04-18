#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "tasks.h"

typedef struct List{
    struct dns_server_t* head;
    struct dns_server_t* tail;
    int nodes_count;
}list_t;

/*
typedef struct address_node_t{
    char* name;
    struct address_node_t* next;
    struct address_node_t* prev;
}address_node_t;*/

typedef struct dns_server_t{
    struct dns_server_t* parent;
    char** addresses;
    struct List* children;
    int addresses_count;
    /*struct List* users;*/
    int isFault;
}dns_server_t;

/*
typedef struct users_node_t{
    int user_index;
    struct dns_server_t* dns_server;
    struct users_node_t* next;
    struct users_node_t* prev;
}users_node_t;


typedef struct dns_node_t{
    struct dns_servet_t* dns_server;
    struct dns_node_t* next;
    struct dns_node_t* prev;
}dns_node_t;


users_node_t* initialize_users_node();
dns_node_t* initialize_dns_node();
address_node_t* initialize_address_node();
list_t* initialize_list();
dns_server_t* initialize_dns_server();

void free_users_node(users_node_t** users_node);
void free_address_node(address_node_t** address_node);
void free_dns_node(dns_node_t** dns_node);
void free_dns_server(dns_server_t** dns_server);
void free_addresses_list(list_t** list);
void free_users_list(list_t** list);
void free_children_list(list_t** list);
*/

list_t* initialize_list();
dns_server_t* initialize_dns_server();

void free_list(list_t** list);
void free_dns_server(dns_server_t** dns);

#endif