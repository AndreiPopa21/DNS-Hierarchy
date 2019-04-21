#include "utils.h"
#include "structs.h"

int check_string_duplicate(char** container,int containter_size, char* new_char){
    if(!container){
        fprintf(stdout,"Cannot check duplicate, NULL container\n");
        return 0;
    }
    if(containter_size == 0){
        fprintf(stdout, "Container has no addresses\n");
        return 0;
    }
    int is_duplicate = 0;
    int i;
    for( i = 0; i < containter_size; i++){
        char* cont_str = container[i];
        if(strcmp(cont_str,new_char) == 0){
            is_duplicate = 1;
            break;
        }
    }
    if(is_duplicate)
        return 1;
    else
        return 0;
}

temp_dns_struct_t** read_from_tree_in(Hierarchy_t** hierarchy,int* servers_count){

    FILE* tree_fh = fopen("tree.in","r");
    temp_dns_struct_t** servers = NULL;

    if(tree_fh){
        int dns_count = 0;
        fscanf(tree_fh,"%d",&dns_count);
        fprintf(stdout,"%d\n",dns_count);
        int i;
        *servers_count = dns_count;
        servers = (temp_dns_struct_t**)calloc(dns_count,sizeof(temp_dns_struct_t*));
        for(i = 0; i<dns_count; i++){
            int curr_dns;
            int parent_dns;
            int addr_count;

            servers[i] = (temp_dns_struct_t*)calloc(dns_count,sizeof(temp_dns_struct_t));

            fscanf(tree_fh,"%d",&curr_dns);
            fscanf(tree_fh,"%d",&parent_dns);
            fscanf(tree_fh,"%d",&addr_count);

            fprintf(stdout,"%d ",curr_dns);
            fprintf(stdout,"%d ",parent_dns);
            fprintf(stdout,"%d ",addr_count);
            //servers[i].server_index = curr_dns;
            //servers[i].parent_index = parent_dns;
            //servers[i].addresses_count = addr_count;
            servers[i]->server_index = curr_dns;
            servers[i]->parent_index = parent_dns;
            servers[i]->addresses_count = addr_count;

            servers[i]->addresses = (char**)calloc(addr_count,sizeof(char*));
            
            int j;
            char addr[256];
            for(j = 0; j < addr_count ; j++){
                fscanf(tree_fh,"%s",addr);
                fprintf(stdout,"%s ",addr);
                servers[i]->addresses[j]=(char*)calloc(1,sizeof(char));
                strcpy(servers[i]->addresses[j],addr);
            }
            fprintf(stdout,"\n");
        }
        return servers;
    }else{
        fprintf(stdout,"Could not open tree.in for reading\n");
        return NULL;
    }
}
