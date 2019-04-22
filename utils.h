#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tasks.h"
#include "structs.h"

int check_string_duplicate(char** container,int containter_size, char* new_char);
temp_dns_struct_t** read_from_tree_in(Hierarchy_t** hierarchy,int* servers_count);

void read_children_index_recursively(dns_server_t** dns_server, FILE* fh);
void cluster_children_addresses(dns_server_t** parent,dns_server_t** node);
void read_dns_servers_recursively(dns_server_t** dns_server,FILE* fh);

int get_server(dns_server_t** root,dns_server_t** found,int server_index);

void traverse_bottom_up(dns_server_t** child);

#endif