#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "tasks.h"
#include "structs.h"

int main(){
    fprintf(stdout,"Created exe\n");

    Hierarchy_t* dns_hierarchy = initialize_hierarchy();
    tree_construction(&dns_hierarchy);
    hierarchy_initialization(&dns_hierarchy);

    user_list_t* users_list = initialize_user_list(5);
    user_queries(&dns_hierarchy,&users_list);
 
    free_users_list(&users_list);

    free_hierarchy(&dns_hierarchy);
    
    fprintf(stdout,"Finished exe\n");

    return 0;
}