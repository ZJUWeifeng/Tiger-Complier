#ifndef TIGER_RESERVED_H_
#define TIGER_RESERVED_H_
#include "util.h"
typedef struct {
	char name[9];
	int token;
} Reserved;

typedef struct nearest_List{
	int tokpos;
	int token;
	char name[9];
	struct nearest_List* next;
}* Nearest_List;

Nearest_List nearest_list(int tokpos, int token, char* name, Nearest_List next);
int RE_find_nearest(int size, Reserved* reserved, char* id);
#endif