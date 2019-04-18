#include "structs.h"

list_t* initialize_list(){
    list_t* new_list = (list_t*)calloc(1,sizeof(struct List));
    new_list->head = NULL;
    new_list->tail = NULL;
    new_list->nodes_count = 0;
    return new_list;
}

dns_server_t* initialize_dns_server(){
    dns_server_t* new_dns = (dns_server_t*)calloc(1,sizeof(dns_server_t));
    new_dns->parent = NULL;
    new_dns->isFault = 0;
    new_dns->addresses = NULL;
    new_dns->addresses_count = 0;
    new_dns->max_addr_count = MAX_ADDR_COUNT;
    new_dns->addresses = (char**)calloc(MAX_ADDR_COUNT,sizeof(char*));
    new_dns->children = initialize_list();
    new_dns->debugCode = 0;
    return new_dns;
}

dns_node_t* initialize_dns_node(){
    dns_node_t* new_dns_node = (dns_node_t*)calloc(1,sizeof(dns_node_t));
    new_dns_node->dns_server = NULL;
    new_dns_node->next = NULL;
    new_dns_node->prev = NULL;
    return new_dns_node;
}

void push_back_dns_child(dns_server_t** parent, dns_server_t** child){
    if(!(*parent))
        return;
    if(!(*child))
        return;
    (*child) -> parent = (*parent);
    push_back_dns_list(&((*parent)->children),*child);
}

void push_back_dns_list(list_t** list, dns_server_t* dns_server){
    if(!(*list))
        return;
    if(!dns_server)
        return;
    dns_node_t* new_dns_node = initialize_dns_node();
    new_dns_node->dns_server = dns_server;
    new_dns_node->next = NULL;
    new_dns_node->prev = (*list)->tail;
    (*list)->nodes_count += 1;
    (*list)->tail = new_dns_node;
}




void free_dns_server(dns_server_t** dns){
    if(!(*dns))
        return;
    (*dns)->isFault = 0;
    (*dns)->parent = NULL;
    int i;
    for(i = 0;i< (*dns)->max_addr_count; i++){
        if((*dns)->addresses[i]!=NULL){
            free((*dns)->addresses[i]);
        }
    }
    free((*dns)->addresses);
    /*for (i = 0; i < (*dns)->children->nodes_count; i++)
        free_dns_node()*/
    dns_node_t* iterator;
    for(iterator = (*dns)->children->head; iterator!=NULL; ){
        dns_node_t* tmp = iterator;
        iterator = iterator->next;
        free(tmp);
    }
    free((*dns)->children);
    free((*dns));
}

void free_dns_node(dns_node_t** dns_node){
    if(!(*dns_node))
        return;
    (*dns_node)->dns_server = NULL;
    (*dns_node)->next = NULL;
    (*dns_node)->prev = NULL;
    free((*dns_node));
}


/*
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

void free_users_node(users_node_t** users_node){
    if(!(*users_node)){
        fprintf(stdout,"Attempted to free null users node\n");
        return;
    }
    (*users_node)->next = NULL;
    (*users_node)->prev = NULL;
    (*users_node)->dns_server = NULL;

    free(*users_node);
}

void free_address_node(address_node_t** address_node){
    if(!(*address_node)){
        fprintf(stdout,"Attempted to free null address node\n");
        return;
    }
    if(!(*address_node)->name){
        free((*address_node)->name);
    }
    (*address_node)->next = NULL;
    (*address_node)->prev = NULL;
    free(*address_node);
}

void free_dns_node(dns_node_t** dns_node){
    if(!(*dns_node)){
        fprintf(stdout,"Attempted to free null dns node\n");
        return;
    }
    (*dns_node)->dns_server = NULL;
    (*dns_node)->next = NULL;
    (*dns_node)->prev = NULL;
    free(*dns_node);
}

void free_dns_server(dns_server_t** dns_server){
    if(!(*dns_server)){
        fprintf(stdout,"Attempted to free null dns_server\n");
        return;
    }
    (*dns_server)->isFault = 0;
    (*dns_server)->parent = NULL;
    free_addresses_list(&((*dns_server)->addresses));
    free_users_list(&((*dns_server)->users));
    free_children_list(&((*dns_server)->children));
    free(*dns_server);
}

void free_addresses_list(list_t** list){    
    if(!(*list)){
        fprintf(stdout,"Attempted to free null list\n");
        return;
    }
    address_node_t* head = (*list)->head;
    address_node_t* iterator;

    for(iterator = head; iterator!=NULL;){
        address_node_t* tmp = iterator;
        iterator = iterator->next;
        free_address_node(&tmp);
    }
    
    (*list)->head = NULL;
    (*list)->tail = NULL;
    (*list)->nodes_count = 0;

    free(*list);
}

void free_users_list(list_t** list){
    if(!(*list)){
        fprintf(stdout,"Attempted to free null list\n");
        return;
    }
    users_node_t* head = (*list)->head;
    users_node_t* iterator;

    for(iterator = head; iterator!=NULL;){
        users_node_t* tmp = iterator;
        iterator = iterator->next;
        free_users_node(&tmp);
    }
    
    (*list)->head = NULL;
    (*list)->tail = NULL;
    (*list)->nodes_count = 0;

    free(*list);
}

void free_children_list(list_t** list){
    if(!(*list)){
        fprintf(stdout,"Attempted to free null list\n");
        return;
    }
    dns_node_t* head = (*list)->head;
    dns_node_t* iterator;

    for(iterator = head; iterator!=NULL;){
        dns_node_t* tmp = iterator;
        iterator = iterator->next;
        free_dns_node(&tmp);
    }
    
    (*list)->head = NULL;
    (*list)->tail = NULL;
    (*list)->nodes_count = 0;

    free(*list);
}*/