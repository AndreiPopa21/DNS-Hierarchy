#ifndef _STRUCTS_H_
#define _STRUCTS_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "tasks.h"

typedef struct List{
    void* head;
    void* tail;
    int nodes_count;
}List;

typedef struct address_node_t{
    char* name;
    struct address_node_t* next;
    struct address_node_t* prev;
}address_node_t;

typedef struct dns_server_t{
    struct dns_server_t* parent;
    struct List* addresses;
    struct List* children;
    struct List* users;
    int isFault;
}dns_server_t;

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

#endif