#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "tasks.h"
#include "structs.h"

int main(){
    fprintf(stdout,"Created exe\n");

    dns_server_t* new_dns = initialize_dns_server();
    free_dns_server(&new_dns);
    
    fprintf(stdout,"Finished exe\n");

    return 0;
}
