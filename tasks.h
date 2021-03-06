#ifndef _TASKS_H_
#define _TASKS_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "structs.h"

void tree_construction(Hierarchy_t** hierarchy);
void hierarchy_initialization(Hierarchy_t** hierarchy);
void user_queries(Hierarchy_t** hierarchy,user_list_t** user_list);
void fault_tollerance();

#endif
