#include "structs.h"

users_node_t* initialize_users_node(){
    users_node_t* users_node = (users_node_t*)calloc(1,sizeof(users_node_t));
    users_node->user_index = -1;
    users_node->dns_server = NULL;
    users_node->next = NULL;
    users_node->prev = NULL;
    return users_node;
}

dns_node_t* initialize_dns_node(){
    dns_node_t* dns_node = (dns_node_t*)calloc(1,sizeof(dns_node_t));
    dns_node->dns_server = NULL;
    dns_node->next = NULL;
    dns_node->prev = NULL;
    return dns_node;
}

address_node_t* initialize_address_node(){
    address_node_t* address_node = (address_node_t*)calloc(1,sizeof(address_node_t));
    address_node->name = NULL;
    address_node->next = NULL;
    address_node->prev = NULL;
    return address_node;
}

list_t* initialize_list(){
    list_t* new_list = (list_t*)calloc(1,sizeof(list_t));
    new_list->head = NULL;
    new_list->nodes_count = 0;
    new_list->tail = NULL;
    return new_list;
}

dns_server_t* initialize_dns_server(){
    dns_server_t* new_dns_server = (dns_server_t*)calloc(1,sizeof(dns_server_t));
    new_dns_server->parent = NULL;
    new_dns_server->addresses = NULL;
    new_dns_server->children = NULL;
    new_dns_server->users = NULL;
    new_dns_server->isFault = 0;
    return new_dns_server;
}