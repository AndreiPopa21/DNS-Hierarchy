#include "tasks.h"

void tree_construction(Hierarchy** hierarchy){
    fprintf(stdout,"First task...\n");
    temp_dns_struct* temp_dns = read_from_tree_in(hierarchy);
    fprintf(stdout,"Finished first task!\n");
}