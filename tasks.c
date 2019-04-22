#include "tasks.h"
#include "utils.h"

void tree_construction(Hierarchy_t** hierarchy){
    fprintf(stdout,"First task...\n");

    if(!(*hierarchy)){
        fprintf(stdout,"Hierarchy is NULL\n");
        return;
    }

    int servers_count = 0;
    temp_dns_struct_t** temp_dns = read_from_tree_in(hierarchy,&servers_count);
    int i;/* 
    for(i = 0 ; i < servers_count; i++){
        print_temp_struct(&temp_dns[i]);
    }*/
    
    (*hierarchy)->servers_count = servers_count;

    dns_server_t** server_array = (dns_server_t**)malloc(servers_count*sizeof(dns_server_t*));
    
    for(i = 0; i < servers_count; i++){
        int index = temp_dns[i]->server_index;
        int par_index = temp_dns[i]->parent_index;
        server_array[i] = initialize_dns_server(index);
        if(par_index == -1){
            (*hierarchy)->root = server_array[i];
            server_array[i] -> parent = NULL;
        }
    }
    
    int j;
    for(i = 0; i < servers_count; i++){
        int par_index = temp_dns[i]->parent_index;
        for(j = 0; j < servers_count; j++){
            if(i!=j){
                if(server_array[j]->server_index == par_index){
                    server_array[i] -> parent = server_array[j];
                    push_back_dns_child(&server_array[j],&server_array[i]);
                }
            }
        }
    }
    (*hierarchy)->root->parent = NULL;
    
    for(i = 0; i<servers_count; i++){
        int addr_count = temp_dns[i]->addresses_count;
        for (j = 0; j<addr_count ;j++){
            //char* address = (char*)malloc(100*sizeof(char))
            //strcpy(address,temp_dns[i]->addresses[j]);
            add_address_for_server(&server_array[i],temp_dns[i]->addresses[j]);
        }
    }
    
    fprintf(stdout,"----------------------------------\n");
    /*for(i = 0; i<servers_count; i++){
       // print_dns_server_addresses(&server_array[i]);
        if(hasChildren(&server_array[i])){
            fprintf(stdout,"Server %d has children\n",server_array[i]->server_index);
        }else{
            fprintf(stdout,"Server %d does not have children\n",server_array[i]->server_index);
        }
    }*/
    
    FILE* fh = fopen("tree.out","w+");
    read_children_index_recursively(&(*hierarchy)->root,fh);

        //dns_server_t* root = (*hierarchy)->root;
          //  cluster_children_addresses(NULL,&root);

        //read_dns_servers_recursively(&root,fh);

    fclose(fh);
    free_temp_dns_array(&temp_dns,servers_count);

    free(server_array);
    fprintf(stdout,"Finished first task!\n");

   /* fh = fopen("hierarchy.out","w+");

    dns_server_t* root = (*hierarchy)->root;
    cluster_children_addresses(NULL,&root);
    read_dns_servers_recursively(&root,fh);
    fclose(fh);
    fprintf(stdout,"Finished second task!\n");*/

    //hierarchy_initialization(hierarchy);
}

void hierarchy_initialization(Hierarchy_t** hierarchy){
    
    fprintf(stdout,"Second task...\n");
    if(!(*hierarchy)){
        fprintf(stdout,"Passed NULL hierarchy to second task\n");
        return;
    }
    FILE* fh = fopen("hierarchy.out","w+");

    dns_server_t* root = (*hierarchy)->root;
    cluster_children_addresses(NULL,&root);
    read_dns_servers_recursively(&root,fh);
    fclose(fh);
    fprintf(stdout,"Finished second task!\n");
}

void user_queries(Hierarchy_t** hierarchy, user_list_t** users_list){

    fprintf(stdout,"Third task...\n");
    if(!(*hierarchy)){
        fprintf(stdout,"Passed NULL hierarchy to third task\n");
        return;
    }
    if(!(*users_list)){
        fprintf(stdout,"Passed NULL users list to third task\n");
        return;
    }

    FILE* fh = fopen("users.in","r+");

    if(!fh){
        fprintf(stdout,"Could not open users.in file\n");
        return;
    }

    int users_count = 0;
    fscanf(fh,"%d",&users_count);
    fprintf(stdout,"Users count: %d\n", users_count);


    int i;
    int user_index;
    int server_index;
    for(i=0;i<users_count;i++){
        fscanf(fh,"%d",&user_index);
        fscanf(fh,"%d",&server_index);
        push_user_node(hierarchy,users_list,user_index,server_index);
        
    }
    fclose(fh);

    int queries_count = 0;
    fh = fopen("queries.in","r+");
    if(!fh){
        fprintf(stdout,"Could not open queries.in file\n");
        return;
    }
    fscanf(fh,"%d",&queries_count);
    //printf("Queries count: %d\n",queries_count);
    char param[10];
    char looked_address[50];
    for(i = 0; i < queries_count; i++){
        fscanf(fh,"%s",param);
        if(strcmp(param,"q")==0){
            printf("We have query!\n");
            fscanf(fh,"%d",&user_index);
            fscanf(fh,"%s",looked_address);
            //printf("%s %d %s\n",param,user_index,looked_address); 
        }else{
            if(strcmp(param,"f")==0){

            }
        }
    }


    fprintf(stdout,"Finished third task\n");
}