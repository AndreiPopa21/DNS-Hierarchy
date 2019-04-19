#include "tasks.h"

void tree_construction(){
    fprintf(stdout,"First task...\n");
    FILE* tree_fh = fopen("tree.in","r");
    char buff[256];
    if(tree_fh){
        int dns_count = 0;
        fscanf(tree_fh,"%d",&dns_count);
        int i;
        fprintf(stdout,"%d\n",dns_count);
        for(i = 0; i<dns_count; i++){
            int curr_dns;
            int parent_dns;
            int addr_count;
            fscanf(tree_fh,"%d",&curr_dns);
            fscanf(tree_fh,"%d",&parent_dns);
            fscanf(tree_fh,"%d",&addr_count);
            fprintf(stdout,"%d ",curr_dns);
            fprintf(stdout,"%d ",parent_dns);
            fprintf(stdout,"%d ",addr_count);
            int j;
            char addr[256];
            for(j = 0; j < addr_count ; j++){
                fscanf(tree_fh,"%s",&addr);
                fprintf(stdout,"%s ",addr);
            }
            fprintf(stdout,"\n");
        }
    }else{
        fprintf(stdout,"Could not open tree.in for reading\n");
    }

    fprintf(stdout,"Finished first task!\n");
}