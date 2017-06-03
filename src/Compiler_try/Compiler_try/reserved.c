#include "reserved.h"
#include "util.h"

Nearest_List nearest_list(int tokpos, int token, string name, Nearest_List next) {
	Nearest_List l = checked_malloc(sizeof(*l));
	l->tokpos = tokpos; l->token = token; strcpy(l->name,name); l->next = next;
	return l;
}

static int RE_distance(string id, string reserved) {
	int distance = 0;
	int i = 0, j = 0;
	while (id[i] != '\0' && reserved[j] != '\0') {
		char c1 = id[i], c2 = reserved[j];
		if (c1 >= 'A' && c1 <= 'Z')
			c1 += 32;
		if (c2 >= 'A' && c2 <= 'Z') {
			c2 += 32;
		}
		if(c1 != c2){
			distance += 1;
        }
		i++;j++;
	}
	while (id[i] != '\0') {
		distance += 1;
		i++;
	}
	while (reserved[j] != '\0') {
		distance += 1;
		j++;
	}
	return distance;
}
int RE_find_nearest(string id, Reserved* reserved, int size) {
	int i = 0;
	int min = 500;
	int distance;
	int index = 0;
	while (i < size) {
		distance = RE_distance(id, reserved[i].name);
		if (min > distance) {
			min = distance;
			index = i;
		}
		i++;
	}
	return index;
}