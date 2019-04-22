#include "tasks.h"
#include "utils.h"

void tree_construction(Hierarchy_t** hierarchy){
    
    int i;
    int j;
    int servers_count = 0;

    if(!(*hierarchy)){
        fprintf(stdout,"Hierarchy is NULL\n");
        return;
    }

    temp_dns_struct_t** temp_dns = read_from_tree_in(hierarchy,&servers_count);

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
            add_address_for_server(&server_array[i],temp_dns[i]->addresses[j]);
        }
    }
    
    FILE* fh = fopen("tree.out","w+");
    
    read_children_index_recursively(&(*hierarchy)->root,fh);

    fclose(fh);

    free_temp_dns_array(&temp_dns,servers_count);
    free(server_array);
}

void hierarchy_initialization(Hierarchy_t** hierarchy){
    
    if(!(*hierarchy)){
        fprintf(stdout,"Passed NULL hierarchy to second task\n");
        return;
    }

    FILE* fh = fopen("hierarchy.out","w+");

    if(!fh){
        fprintf(stdout,"Could not open hierarchy.out\n");
        return;
    }

    dns_server_t* root = (*hierarchy)->root;
    cluster_children_addresses(NULL,&root);
    read_dns_servers_recursively(&root,fh);

    fclose(fh);
}

void user_queries(Hierarchy_t** hierarchy, user_list_t** users_list){

    int i;
    int users_count = 0;
    int user_index;
    int server_index;
    int queries_count = 0;
    char param[10];

    if(!(*hierarchy)){
        fprintf(stdout,"Passed NULL hierarchy to third task\n");
        return;
    }
    if(!(*users_list)){
        fprintf(stdout,"Passed NULL users list to third task\n");
        return;
    }

    FILE* fh = fopen("users.in","r+");
    FILE* q_fh = fopen("queries.out","w+");

    if(!fh){
        fprintf(stdout,"Could not open users.in file\n");
        return;
    }
    
    if(!q_fh){
        fprintf(stdout,"Could not open queries.out file\n");
        return;
    }

    fscanf(fh,"%d",&users_count);

    if(!q_fh){
        fprintf(stdout,"Could not open queries.out file\n");
        return;
    }

    for(i=0;i<users_count;i++){
        fscanf(fh,"%d",&user_index);
        fscanf(fh,"%d",&server_index);
        push_user_node(hierarchy,users_list,user_index,server_index);
        
    }

    fclose(fh);

    user_node_t** found_user = (user_node_t**)calloc(1,sizeof(user_node_t*));
    dns_server_t** found_server = (dns_server_t**)calloc(1,sizeof(dns_server_t*));
    
    fh = fopen("queries.in","r+");

    if(!fh){
        fprintf(stdout,"Could not open queries.in file\n");
        return;
    }

    fscanf(fh,"%d",&queries_count);
    
    char** looked_addresses = (char**)calloc(queries_count,sizeof(char*));
    
    for(i = 0;i<queries_count;i++){
        looked_addresses[i] = (char*)calloc(50,sizeof(char));
    }

    for(i = 0; i < queries_count; i++){
        fscanf(fh,"%s",param);
        (*found_server)= NULL;
        (*found_user)= NULL;
        
        if(strcmp(param,"q")==0){
            fscanf(fh,"%d",&user_index);
            fscanf(fh,"%s",looked_addresses[i]);
            
            int result = get_user(users_list,found_user,user_index);
            if(result){
                dns_server_t* user_server = (*found_user)->server;
                traverse_bottom_up_for_address(&user_server,looked_addresses[i],q_fh);
            }
        } else {
            if(strcmp(param,"f")==0){
                (*found_server)=NULL;
                fscanf(fh,"%d",&server_index);
                int result = get_server(&((*hierarchy)->root),found_server,server_index);
                if(result == 1){
                    pass_faulty_server_users(found_server,users_list);
                }  
            }
        }
    }

    for(i = 0; i<queries_count;i++){
        if(looked_addresses[i]){
            free(looked_addresses[i]);
        }
    }
    
    fclose(fh);
    fclose(q_fh);

    free(looked_addresses);
    free(found_user);
    free(found_server);
}