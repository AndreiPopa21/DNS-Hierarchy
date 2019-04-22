#include "utils.h"
#include "structs.h"

int check_string_duplicate(char** container,int containter_size, char* new_char) {
    
    int i = 0;
    int is_duplicate = 0;

    if(!container) {
        fprintf(stdout,"Cannot check duplicate, NULL container\n");
        return 0;
    }

    if(containter_size == 0) {
        return 0;
    }
    
    for( i = 0; i < containter_size; i++) {
        char* cont_str = container[i];
        if(strcmp(cont_str,new_char) == 0) {
            is_duplicate = 1;
            break;
        }
    }

    if(is_duplicate)
        return 1;
    else
        return 0;
}

temp_dns_struct_t** read_from_tree_in(Hierarchy_t** hierarchy,int* servers_count) {

    FILE* tree_fh = fopen("tree.in","r");

    int i = 0;
    int j = 0;
    int dns_count = 0;
    int curr_dns = 0;
    int parent_dns = 0;
    int addr_count = 0;
    char addr[256];
    temp_dns_struct_t** servers = NULL;

    if(tree_fh) {

        fscanf(tree_fh,"%d",&dns_count);

        *servers_count = dns_count;
        servers = (temp_dns_struct_t**)malloc(dns_count*sizeof(temp_dns_struct_t*));

        for(i = 0; i<dns_count; i++) {
            servers[i] = (temp_dns_struct_t*)malloc(dns_count*sizeof(temp_dns_struct_t));

            fscanf(tree_fh,"%d",&curr_dns);
            fscanf(tree_fh,"%d",&parent_dns);
            fscanf(tree_fh,"%d",&addr_count);

            servers[i]->server_index = curr_dns;
            servers[i]->parent_index = parent_dns;
            servers[i]->addresses_count = addr_count;

            servers[i]->addresses = (char**)malloc(addr_count*sizeof(char*));

            for(j = 0; j < addr_count ; j++) {
                fscanf(tree_fh,"%s",addr);
                servers[i]->addresses[j]=(char*)calloc(strlen(addr)+1,sizeof(char));
                strcpy(servers[i]->addresses[j],addr);
            }
        }
        return servers;
    } else {
        fprintf(stdout,"Could not open tree.in for reading\n");
        return NULL;
    }
}

void read_children_index_recursively(dns_server_t** dns_server,FILE* fh) {
    
    if(!(*dns_server)) {
        return;
    }

    fprintf(fh,"%d",(*dns_server)->server_index);
    
    if(hasChildren(dns_server)) {

        dns_node_t* iter = (*dns_server)->children->head;
        while(iter) {
            dns_server_t* child = iter->dns_server;
            iter = iter->next;
            fprintf(fh," %d",child->server_index);
        }

        fprintf(fh,"\n");
        
        iter = (*dns_server)->children->head;
        
        while(iter) {
            read_children_index_recursively(&(iter->dns_server),fh);
            iter = iter->next;
        }
    } else {
        fprintf(fh,"\n");
    }
}

void cluster_children_addresses(dns_server_t** parent, dns_server_t** node) {
    
    int i = 0;
    int addr_count = 0;

    if(!(*node)) {
        return;
    }

    if(hasChildren(node)) {
        dns_node_t* iter = (*node)->children->head;
        while(iter!=NULL) {
            dns_server_t* child = iter->dns_server;
            cluster_children_addresses(node,&child);
            iter = iter->next;
        }
    }

    if(!parent) {
        return;
    }

    if(!(*parent)) {
        return;
    }

    addr_count = (*node)->addresses_count;
    
    for( i = 0; i < addr_count; i++) {
        add_address_for_server(parent,(*node)->addresses[i]);
    }
}

void read_dns_servers_recursively(dns_server_t** dns_server,FILE* fh) {
    
    int i = 0;
    int addr_count = 0;
    
    if(!(*dns_server)) {
        return;
    }
    if(!fh) {
        fprintf(stdout,"Cannot write to file for second task\n");
        return;
    }
    
    fprintf(fh,"%d",(*dns_server)->server_index);
    addr_count = (*dns_server)->addresses_count;
    
    for ( i=0; i<addr_count; i++) {
        fprintf(fh," %s",(*dns_server)->addresses[i]);
    }

    fprintf(fh,"\n");

    if(hasChildren(dns_server)) {
        dns_node_t* iter = (*dns_server)->children->head;
        while(iter!=NULL) {
            dns_server_t* child = iter->dns_server;
            read_dns_servers_recursively(&child,fh);
            iter = iter->next;
        }
    }
}


int get_server(dns_server_t** root, dns_server_t** found,int server_index) {

    if(!root) {
        fprintf(stdout,"Double-pointer NULL when getting server\n");
        return 0;
    }

    if(!(*root)) {
        fprintf(stdout,"Root missing\n");
        return 0;
    }

    if((*found)) {
        return 1;
    }

    if((*root)->server_index == server_index) {
        (*found) = (*root);
        return 1;
    } else {
        if(hasChildren(root)) {
            dns_node_t* iter = (*root)->children->head;
            int toContinue = 1;
            int result = 0;
            while(toContinue && iter) {
                result = get_server(&(iter->dns_server), found, server_index);
                if(result) {
                    toContinue = 0;
                }
                iter = iter->next;
            }
            if(result==1) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    }

    return 0;
}

int get_user(user_list_t** users_list,user_node_t** found, int user_index) {
    
    if(!(*users_list)) {
        fprintf(stdout,"Passed null users list in get_user function\n");
        return 0;
    }
    if(!found) {
        fprintf(stdout,"NULL found object to store found user in\n");
        return 0;
    }
    int toContinue = 1;
    user_node_t* iter = (*users_list)->head;
    while(toContinue && iter) {
        if(iter->user_index == user_index) {
            *found = iter;
            toContinue = 0;
            break;
        }
        iter = iter->next;
    }
    if(toContinue == 0)
        return 1;
    else
        return 0;
}

void traverse_bottom_up(dns_server_t** child) {
    
    if(!(*child)) {
        fprintf(stdout,"Cannot traverse from NULL child\n");
        return;
    } else {
        dns_server_t* parent = (*child)->parent;
        if(!parent) {
            fprintf(stdout," - (-1)\n");
        } else {
            fprintf(stdout," - %d",(*child)->server_index);
            traverse_bottom_up(&parent);
        }
    }
}

void traverse_bottom_up_for_address(dns_server_t** child, char* address, FILE* fh) {

    if(!fh) {
        fprintf(stdout,"No file to write stream to\n");
        return;
    }

    if(!(*child)) {
        fprintf(stdout,"Cannot traverse from NULL child for address\n");
        return;
    } else {
        fprintf(fh,"%d",(*child)->server_index);
        if(contains_address(child,address)==1) {
            fprintf(fh,"\n");
            return;
        } else {
            add_address_for_server(child,address);
            dns_server_t* parent = (*child)->parent;
            fprintf(fh," ");
            traverse_bottom_up_for_address(&parent,address,fh);
        }
    }
}

int contains_address(dns_server_t** dns_server, char* address) {
    
    int i;
    int contains = 0;
    int addr_count = 0;

    if(!(*dns_server)) {
        fprintf(stdout,"NULL server passed to contains_address() function\n");
        return 0;
    }

    addr_count = (*dns_server)->addresses_count;

    for(i = 0; i<addr_count; i++) {
        if(strcmp(address,(*dns_server)->addresses[i]) == 0) {
            contains = 1;
            break;
        }
    }

    return contains;
}

void pass_faulty_server_users(dns_server_t** dns_server, user_list_t** users_list) {
    
    int server_index = 0;
    int user_server_index = 0;
    user_node_t* iter = NULL;
    dns_server_t* parent = NULL;
    
    if(!(*dns_server)) {
        fprintf(stdout,"Passed NULL server to faulty server function\n");
        return;
    }
    if(!(*users_list)) {
        fprintf(stdout,"NULL users list passed\n");
        return;
    }

    iter = (*users_list)->head;
    server_index = (*dns_server)->server_index;
    parent = (*dns_server)->parent;
    
    while(iter) {
        user_server_index = iter->server->server_index;
        if(server_index == user_server_index) {
            iter->server = parent;
        }
        iter = iter->next;
    }
}

void pass_faulty_server_children(dns_server_t** dns_server) {
    
    if(!(*dns_server)) {
        fprintf(stdout,"NULL server,faulty server children\n");
        return;
    }
    dns_node_t* iter = (*dns_server)->children->head;

    while(iter) {
        iter->dns_server->parent = (*dns_server)->parent;
        push_back_dns_child(&((*dns_server)->parent),&(iter->dns_server));
        iter=iter->next;
    }
}