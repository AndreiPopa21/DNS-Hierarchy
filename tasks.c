#include "tasks.h"
#include "utils.h"
void tree_construction(Hierarchy_t** hierarchy){
    fprintf(stdout,"First task...\n");
    int servers_count = 0;
    temp_dns_struct_t** temp_dns = read_from_tree_in(hierarchy,&servers_count);
    int i; 
    for(i = 0 ; i < servers_count; i++){
        print_temp_struct(&temp_dns[i]);
    }
    //fprintf(stdout,"%d\n",temp_dns[0]->addresses_count);
    free_dns_array(&temp_dns,servers_count);
    fprintf(stdout,"Finished first task!\n");
}