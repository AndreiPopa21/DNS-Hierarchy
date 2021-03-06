#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_ADDR_COUNT 20

typedef struct List{
    struct dns_node_t* head;
    struct dns_node_t* tail;
    int nodes_count;
}list_t;

typedef struct dns_server_t{
    int server_index;
    struct dns_server_t* parent;
    struct List* children;

    char** addresses;
    int max_addr_count;
    int addresses_count;

    int isFault;
    int visited;
}dns_server_t;

typedef struct dns_node_t{
    struct dns_server_t* dns_server;
    struct dns_node_t* next;
    struct dns_node_t* prev;
}dns_node_t;

typedef struct temp_dns_struct{
    int server_index;
    int parent_index;
    int addresses_count;
    char** addresses;
}temp_dns_struct_t;

typedef struct Hierarchy{
    struct dns_server_t* root;
    int servers_count;
}Hierarchy_t;

typedef struct user_node_t{
    int user_index;
    struct dns_server_t* server;
    struct user_node_t* next;
    struct user_node_t* prev;
}user_node_t;

typedef struct UserList{
    int users_count;
    struct user_node_t* head;
    struct user_node_t* tail;
}user_list_t;

/*
typedef struct users_node_t{
    int user_index;
    struct dns_server_t* dns_server;
    struct users_node_t* next;
    struct users_node_t* prev;
}users_node_t;
*/

list_t* initialize_list();
dns_server_t* initialize_dns_server(int server_index);
dns_node_t* initialize_dns_node(dns_server_t** dns_server);
Hierarchy_t* initialize_hierarchy();
user_list_t* initialize_user_list();
user_node_t* initialize_user_node(int user_index, dns_server_t** dns_server);

void push_back_dns_child(dns_server_t** parent, dns_server_t** child);
void delete_at_dns_child(dns_server_t** server, int position);
void push_user_node(Hierarchy_t** hier,user_list_t** user_list,int user_index, int server_index);

void push_back_dns_list(list_t** list, dns_server_t* dns_server);
dns_node_t* get_dns_node_at(list_t** list, int position);
void delete_at_dns_list(list_t** list, int position);
void add_address_for_server(dns_server_t** dns_server,char* address);

void print_dns_list(list_t** list);
void print_dns_list_reverse(list_t** list);
void print_dns_server_childern(dns_server_t** dns_server);
void print_dns_server_parent_index(dns_server_t** dns_server);
void print_dns_server_addresses(dns_server_t** dns_server);
void print_temp_struct(temp_dns_struct_t** tmp);
void print_users_list(user_list_t** users_list);

int get_dns_node_server_index(dns_node_t** dns_node);
int get_dns_server_parent_index(dns_server_t** dns_server);
int hasChildren(dns_server_t** dns_server);

void free_dns_server(dns_server_t** dns_server);
void free_dns_node(dns_node_t** dns_node);
void free_dns_list(list_t** list);
void free_temp_dns_array(temp_dns_struct_t*** temp_array, int size);
void free_users_list(user_list_t** users_list);
void free_hierarchy(Hierarchy_t** hierarchy);
void free_tree_recursively(dns_server_t** parent);

#endif
