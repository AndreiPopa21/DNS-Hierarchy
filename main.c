#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "tasks.h"
#include "structs.h"

int main(){
    fprintf(stdout,"Created exe\n");

    list_t* list = initialize_list();

    fprintf(stdout,"Finished exe\n");
    return 0;
}
