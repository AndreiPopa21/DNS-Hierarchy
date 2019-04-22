#include "utils.h"
#include "structs.h"

list_t* initialize_list(){
    list_t* new_list = (list_t*)malloc(1*sizeof(struct List));
    new_list->head = NULL;
    new_list->tail = NULL;
    new_list->nodes_count = 0;
    return new_list;
}

user_list_t* initialize_user_list(int users_count){
    if(users_count<=0){
        fprintf(stdout,"No users for this list\n");
        return NULL;
    }
    user_list_t* user_list = (user_list_t*)calloc(1,sizeof(user_list_t));
    user_list->head = NULL;
    user_list->tail = NULL;
    user_list->users_count = users_count;
    return user_list;
}

dns_server_t* initialize_dns_server(int server_index){
    dns_server_t* new_dns = (dns_server_t*)malloc(1*sizeof(dns_server_t));
    new_dns->parent = NULL;
    new_dns->isFault = 0;
    new_dns->addresses = NULL;
    new_dns->addresses_count = 0;
    new_dns->max_addr_count = MAX_ADDR_COUNT;
    new_dns->addresses = (char**)malloc(MAX_ADDR_COUNT*sizeof(char*));
    int i;
    for(i=0;i<MAX_ADDR_COUNT;i++){
        new_dns->addresses[i]=NULL;
        new_dns->addresses[i]='\0';
    }
    new_dns->children = initialize_list();
    new_dns->visited = 0;
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
int times = 0;
void add_address_for_server(dns_server_t** dns_server,char* address){
    
    //times++;
    //int curr_index = (*dns_server)->server_index;
            //fprintf(stdout,"TESSTTTT: %s\n",address);
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
        int new_max_addr_count = max_addr_count + 100;
        (*dns_server)->addresses = (char**)realloc((*dns_server)->addresses,new_max_addr_count*sizeof(char*));
        //int i;
        /*for(i = curr_addr_count;i<new_max_addr_count;i++){
            (*dns_server)->addresses[i] = NULL;
            (*dns_server)->addresses[i] = '\0';
        }*/
        if((*dns_server)->addresses == NULL){
            fprintf(stdout,"WTFFFFF\n");
        }
        (*dns_server)->max_addr_count = new_max_addr_count;
        fprintf(stdout,"Needed to reallocate addresses space\n");
    }
    
   //fprintf(stdout,"MAX: %d | ",(*dns_server)->max_addr_count);
   //fprintf(stdout,"TIMES: %d | SERVER_INDEX: %d| ADDR: %s| COUNT: %d\n",times,curr_index,address,(*dns_server)->addresses_count);


    (*dns_server)->addresses[curr_addr_count] = address;
    
    curr_addr_count +=1;
   
   (*dns_server)->addresses_count= curr_addr_count;

    //fprintf(stdout,"DAAAA\n");
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
    fprintf(stdout,"Dns_server %d has %d children\n",(*dns_server)->server_index,children_count);
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

int hasChildren(dns_server_t** dns_server){
    if(!(*dns_server)){
        fprintf(stdout,"Unable to check existence of children on NULL\n");
        return 0;
    }
    list_t* children_list = (*dns_server)->children;
    if(!children_list){
        fprintf(stdout,"Children list NULL\n");
        return 0;
    }
    int children_count = (*dns_server)->children->nodes_count;
    if(children_count > 0){
        return 1;
    }else{
        return 0;
    }
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
        //int addr_count = (*temp_array)[i]->addresses_count;
        (*temp_array)[i]->addresses_count = 0;
       // int j;
        /*for( j=0 ; j<addr_count; j++){
            free((*temp_array)[i]->addresses[j]);
        }*/
        free((*temp_array)[i]->addresses);
        free((*temp_array)[i]);
    }
    free((*temp_array));
}

void free_users_list(user_list_t** users_list){
    if(!(*users_list)){
        fprintf(stdout,"No users list to be freed\n");
        return;
    }
    user_node_t* iter = (*users_list)->head;
    while(iter){
        user_node_t* tmp = iter;
        free(tmp);
        iter = iter->next;
    }
    free(*users_list);
}

void free_hierarchy(Hierarchy_t** hierarchy){
    if(!(*hierarchy)){
        fprintf(stdout,"No hierarchy to be freed\n");
        return;
    }
    free(*hierarchy);
}