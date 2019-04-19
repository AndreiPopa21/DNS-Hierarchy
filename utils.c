#include "utils.h"

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
