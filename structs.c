#include "utils.h"
#include "structs.h"

list_t* initialize_list(){
    list_t* new_list = (list_t*)calloc(1,sizeof(struct List));
    new_list->head = NULL;
    new_list->tail = NULL;
    new_list->nodes_count = 0;
    return new_list;
}

dns_server_t* initialize_dns_server(int server_index){
    dns_server_t* new_dns = (dns_server_t*)calloc(1,sizeof(dns_server_t));
    new_dns->parent = NULL;
    new_dns->isFault = 0;
    new_dns->addresses = NULL;
    new_dns->addresses_count = 0;
    new_dns->max_addr_count = MAX_ADDR_COUNT;
    new_dns->addresses = (char**)calloc(MAX_ADDR_COUNT,sizeof(char*));
    new_dns->children = initialize_list();
    new_dns->debugCode = 0;
    new_dns->server_index = server_index;
    return new_dns;
}

dns_node_t* initialize_dns_node(dns_server_t** dns_server){
    dns_node_t* new_dns_node = (dns_node_t*)calloc(1,sizeof(dns_node_t));
    new_dns_node->dns_server = (*dns_server);
    new_dns_node->next = NULL;
    new_dns_node->prev = NULL;
    return new_dns_node;
}

Hierarchy_t* initialize_hierarchy(){
    Hierarchy_t* dns_hierarchy = (Hierarchy_t*)calloc(1,sizeof(Hierarchy_t));
    return dns_hierarchy;
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
    dns_node_t* new_dns_node = initialize_dns_node(&dns_server);

    if(!(*list)->head){
        (*list)->nodes_count = 1;
        (*list)->head = new_dns_node;
        (*list)->tail = new_dns_node;
        return;
    }

    new_dns_node->next = NULL;
    new_dns_node->prev = (*list)->tail;
    (*list)->tail->next = new_dns_node;
    (*list)->nodes_count += 1;
    (*list)->tail = new_dns_node;
}

dns_node_t* get_dns_node_at(list_t** list, int position){
    if(!(*list)){
        fprintf(stdout,"[GET_DNS_NODE_AT]: list empty\n");
        return NULL;
    }
    if(position < 0){
        fprintf(stdout,"[GET_DNS_NODE_AT]: invalid position\n");
        return NULL;
    }
    int nodes_count = (*list)->nodes_count;
    if(position>=nodes_count){
        fprintf(stdout,"[GET_DNS_NODE_AT]: invalid position\n");
        return NULL;
    }

    dns_node_t* iterator = (*list)->head;
    int i;
    for(i = 0; i < position; i++){
        if(!iterator){
            fprintf(stdout,"[GET_DNS_NODE_AT]: you have a problem with nodes count\n");
            return NULL;
        }
        iterator = iterator->next;
    }
    return iterator;
}

void delete_at_dns_child(dns_server_t** server, int position){
    if(!(*server)){
        fprintf(stdout,"Cannot delete child of a NULL server\n");
        return;
    }
    if(!(*server)->children){
        fprintf(stdout,"Cannot delete child from a NULL children list\n");
        return;
    }
    delete_at_dns_list(&((*server)->children),position);
}

void delete_at_dns_list(list_t** list, int position){
    if(!(*list)){
        fprintf(stdout,"You gave me NULL list to delete at\n");
        return;
    }
    int nodes_count = (*list)->nodes_count;
    if(nodes_count == 0){
        fprintf(stdout,"Did not delete node at, nodes_count problematic\n");
        return;
    }

    if(position < 0){
        fprintf(stdout,"Attempted to delete node at a negative position\n");
        return;
    }
    if(position >= nodes_count){
        fprintf(stdout,"Attempted to delete node at an unoccupied position\n");
        return;
    }

    if(!(*list)->head){
        fprintf(stdout,"Attempted to delete node at position, but head NULL\n");
        return;
    }
    if(nodes_count == 1){
        free_dns_node(&((*list)->head));
        (*list)->head = NULL;
        (*list)->tail = NULL;
        (*list)->nodes_count = 0;
        return;
    }

    if(position == 0){
        fprintf(stdout,"Deleting head...\n");
        (*list)->head->next->prev = NULL;
        dns_node_t* tmp = (*list)->head->next;
        free_dns_node(&((*list)->head));
        (*list)->head = tmp;
        (*list)->nodes_count -= 1;
        return;
    }

    if(position == nodes_count -1){
        fprintf(stdout,"Deleting tail...\n");
        dns_node_t* tmp = (*list)->tail->prev;
        (*list)->tail->prev->next = NULL;
        free_dns_node(&((*list)->tail));
        (*list)->tail = tmp;
        (*list)->nodes_count -= 1;
        return;
    }
    dns_node_t* iter = (*list)->head;
    int i;
    for(i = 0; i<position; i++){
        iter = iter->next;
    }
    dns_node_t* prev = iter->prev;
    dns_node_t* next = iter->next;
    prev->next = next;
    next->prev = prev;
    free_dns_node(&iter);
    (*list)->nodes_count -=1;
}

void add_address_for_server(dns_server_t** dns_server,char* address){
    if(!(*dns_server)){
        fprintf(stdout,"Could not add address on a NULL server\n");
        return;
    }
    if(address[0]=='\0'){
        fprintf(stdout,"Could not add an empty address\n");
        return;
    }
    if(!(*dns_server)->addresses){
        fprintf(stdout,"Could not add address, NULL address container on server\n");
        return;
    }

    int curr_addr_count = (*dns_server)->addresses_count;
    int max_addr_count = (*dns_server)->max_addr_count;
    char** container = (*dns_server)->addresses;

    int is_duplicate = check_string_duplicate(container,curr_addr_count,address);
    if(is_duplicate){
        return;
    }

    if(curr_addr_count >= max_addr_count){
        int new_max_addr_count = max_addr_count + 20;
        (*dns_server)->addresses = (char**)realloc((*dns_server)->addresses,new_max_addr_count);
        (*dns_server)->max_addr_count = new_max_addr_count;
        fprintf(stdout,"Needed to reallocate addresses space\n");
    }
    (*dns_server)->addresses[curr_addr_count] = address;
    (*dns_server)->addresses_count += 1;
}





void print_dns_list(list_t** list){
    if(!(*list)){
        fprintf(stdout,"There is no list to be printed\n");
        return;
    }

    if((*list)->head == NULL){
        fprintf(stdout,"Could not print list, head is NULL\n");
        return;
    }

    if((*list)->tail == NULL){
        fprintf(stdout,"I won't print list, tail should not be NULL\n");
        return;
    }

    if((*list)->nodes_count == 0){
        fprintf(stdout,"Could not print list, nodes count 0\n");
        return;
    }

    dns_node_t* iterator;
    fprintf(stdout,"List has %d nodes\n",(*list)->nodes_count);
    fprintf(stdout,"Printing list: ");
    for(iterator = (*list)->head; iterator!=NULL; iterator = iterator->next){
        fprintf(stdout,"%d - ", get_dns_node_server_index(&iterator));
    }
    fprintf(stdout,"NULL\n");

}
void print_dns_list_reverse(list_t** list){
    if(!(*list)){
        fprintf(stdout,"There is no list to be printed\n");
        return;
    }

    if((*list)->head == NULL){
        fprintf(stdout,"Could not print list, head is NULL\n");
        return;
    }

    if((*list)->tail == NULL){
        fprintf(stdout,"I won't print list, tail should not be NULL\n");
        return;
    }

    if((*list)->nodes_count == 0){
        fprintf(stdout,"Could not print list, nodes count 0\n");
        return;
    }
    dns_node_t* iterator;
    fprintf(stdout,"List has %d nodes\n",(*list)->nodes_count);
    fprintf(stdout,"Printing list in reverse: ");
    for(iterator = (*list)->tail; iterator!=NULL; iterator = iterator->prev){
        fprintf(stdout,"%d - ", get_dns_node_server_index(&iterator));
    }
    fprintf(stdout,"NULL\n");
}
void print_dns_server_childern(dns_server_t** dns_server){
    if(!(*dns_server)){
        fprintf(stdout,"You gave me an empty dns to print children for\n");
        return;
    }
    if(!(*dns_server)->children){
        fprintf(stdout,"The children list for this dns is NULL");
        return;
    }
    int children_count = (*dns_server)->children->nodes_count;
    fprintf(stdout,"This dns_server has %d children\n",children_count);
    print_dns_list(&((*dns_server)->children));

}
void print_dns_server_parent_index(dns_server_t** dns_server){
    if(!(*dns_server)){
        fprintf(stdout,"Passed NULL server\n");
        return;
    }
    int parent_index = get_dns_server_parent_index(dns_server);
    if(parent_index == -200){
        fprintf(stdout,"Passed NULL server\n");
        return;
    }
    if(parent_index == -100){
        fprintf(stdout,"Server has a NULL parent\n");
        return;
    }
    int dns_server_index = (*dns_server)->server_index;
    fprintf(stdout,"For server %d, parent has %d index\n",dns_server_index,parent_index);
}
void print_dns_server_addresses(dns_server_t** dns_server){
    if(!(dns_server)){
        fprintf(stdout,"Could not print addresses for a NULL server\n");
        return;
    }
    if(!(*dns_server)->addresses){
        fprintf(stdout,"Could not print addresses, server has NULL container\n");
        return;
    }
    if((*dns_server)->addresses_count == 0){
        fprintf(stdout,"Server has no addresses to be printed\n");
        return;
    }
    int i;
    int dns_server_index = (*dns_server)->server_index;
    int addresses_count = (*dns_server)->addresses_count;
    fprintf(stdout,"Printing addresses for %d server: ",dns_server_index);
    for( i = 0; i < addresses_count; i++){
        fprintf(stdout,"%s - ",(*dns_server)->addresses[i]);
    }
    fprintf(stdout,"NULL\n");
    fprintf(stdout,"%d addresses printed\n",addresses_count);
}
void print_temp_struct(temp_dns_struct_t** tmp){
    if(!(*tmp)){
        fprintf(stdout,"Could not prin temp_dns, is NULL\n");
        return;
    }

    fprintf(stdout,"Printing temp structure....\n");
    fprintf(stdout,"Server index: %d\n",(*tmp)->server_index);
    fprintf(stdout,"Parent index %d\n",(*tmp)->parent_index);
    fprintf(stdout,"Addresses count: %d\n",(*tmp)->addresses_count);
    fprintf(stdout,"Addresses: ");
    int j;
    for(j=0;j<(*tmp)->addresses_count;j++){
        fprintf(stdout,"%s - ",(*tmp)->addresses[j]);
    }
    fprintf(stdout,"\n\n");
}

int get_dns_node_server_index(dns_node_t** dns_node){
    if(!(*dns_node)){
        fprintf(stdout,"Get server index on empty dns node\n");
        return -100;
    }

    if(!(*dns_node)->dns_server){
        fprintf(stdout,"Get server index, dns node has an empty dns_server ref\n");
        return -100;
    }

    return (*dns_node)->dns_server-> server_index;
}
int get_dns_server_parent_index(dns_server_t** dns_server){
    if(!(*dns_server)){
        fprintf(stdout,"Could not get parent index, NULL dns_server\n");
        return -200;
    }
    if(!(*dns_server)->parent){
        fprintf(stdout,"Could not get parent index, parent NULL\n");
        return -100;
    }
    return (*dns_server)->parent->server_index;
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
    //dns_node_t* iterator;
   /* for(iterator = (*dns)->children->head; iterator!=NULL; ){
        dns_node_t* tmp = iterator;
        iterator = iterator->next;
        free(tmp);
    }
    free((*dns)->children);*/
    free_dns_list(&((*dns)->children));

    free((*dns));
}

void free_dns_node(dns_node_t** dns_node){
    if(!(*dns_node))
        return;
    fprintf(stdout,"Se curata %p\n",*dns_node);
    (*dns_node)->dns_server = NULL;
    (*dns_node)->next = NULL;
    (*dns_node)->prev = NULL;
    free((*dns_node));
}

void free_dns_list(list_t** list){
    if(!(*list))
        return;

    dns_node_t* tmp;
    while(!(*list)->head){
        tmp = (*list)->head;
        (*list)->head = (*list)->head->next;
        free_dns_node(&tmp);
    }

    (*list)->nodes_count = 0;
    (*list)->head = NULL;
    (*list)->tail = NULL;
    free((*list));
}

void free_temp_dns_array(temp_dns_struct_t*** temp_array,int size){
    if(!(*temp_array)){
        fprintf(stdout,"Temp array is NULL, unable to free\n");
        return;
    }
    int i;
    for(i = 0; i < size; i++){
        (*temp_array)[i]->server_index = 0;
        (*temp_array)[i]->parent_index = 0;
        int addr_count = (*temp_array)[i]->addresses_count;
        (*temp_array)[i]->addresses_count = 0;
        int j;
        for( j=0 ; j<addr_count; j++){
            free((*temp_array)[i]->addresses[j]);
        }
        free((*temp_array)[i]->addresses);
        free((*temp_array)[i]);
    }
    free((*temp_array));
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
