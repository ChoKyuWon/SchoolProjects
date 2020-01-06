/*////////////////////////////////////////////////////////////////////////////////////////////////
Data structure HW 4 ver 0.5
Last modified Date: 2017.12.3 10:11
Coder:2017314875 Cho Kyu Won
////////////////////////////////////////////////////////////////////////////////////////////////*/

#include<stdio.h>
#include<stdlib.h>

typedef struct list {
	int end;
	int w;
	struct list *next;
}List;

typedef struct graph {
	int start;
	int flag;
	int listnum;
	List *list;
}G;

typedef struct result {

	int market;
	int distance;
}RES;

void addE(int end, int w, int index);
void addV(G* graph, int numofV, int start);
int searchG(int val, int numofV);
void flagreset(int numofV);
int _DFS(int index, int sum, int numofV);
RES* DFS(int, int, int);

G *node = NULL;
FILE * write, *read;
int index_d;
int cchk;
int cchk_f;

int main() {
	read = fopen("hw4_input.txt", "r");
	write = fopen("hw4_output.txt", "w");

	RES *res[500001];
	int i = 0;
	int numofV = 0;
	int flag = 0;
	int max = 10;
	int j = 0;
	while (!feof(read)){
		flag = 0;
		int start, end, w;
		fscanf(read, "%d %d %d", &start, &end, &w);

		if (j == 0) {
			j++;
			node = (G*)malloc(sizeof(G) * max);
		}
		if (numofV > max - 5) {
			node = (G*)realloc(node, sizeof(G)*max * 2);
			max *= 2;
		}
		for (i = 0; i < numofV; i++) {
			if (node[i].start == start) {
				addV(node, numofV, end);
				numofV++;
				addE(end, w, i);
				addE(start, w, numofV - 1);
				node[numofV-1].listnum = 1;
				node[i].listnum++;
				flag = 1;
				break;
			}
		}
		if (flag == 0) {
			flag++;
			addV(node, numofV, start);
			node[numofV].listnum = 1;
			numofV++;
			addV(node, numofV, end);
			node[numofV].listnum = 1;
			numofV++;
			addE(end, w, numofV - 2);
			addE(start, w, numofV - 1);
		}
	}
	
//	res = (RES**)malloc(sizeof(RES*)*numofV);
	for (i = 0; i < numofV; i++) {
		res[i] = DFS(i, 0, numofV);
		if (cchk_f == 1) {
			fprintf(write, "NULL");
			return 0;
		}
	}
	int min, index;
	min = res[0]->distance;
	index = 0;
	for (i = 0; i < numofV; i++) {
		if (min > res[i]->distance) {
			index = i;
			min = res[i]->distance;
		}
	}
	fprintf(write, "%d\n%d", node[index].start, min);
}

RES* DFS(int index, int sum, int numofV) {
	int i;
	RES* res;
	res = (RES*)malloc(sizeof(RES));
	cchk = 0;
	sum = _DFS(index, 0, numofV);
	if (cchk != numofV) {
		cchk_f = 1;
		return NULL;
	}
	flagreset(numofV);
	res->market = index_d;
	res->distance = sum;
	return res;
}

int _DFS(int index, int sum, int numofV) {
	if (node[index].flag == 1) {
		return 0;
	}
	cchk++;
	int indlist;
	node[index].flag = 1;
	int *sumlist;
	sumlist = (int*)malloc(sizeof(int)*node[index].listnum);
	List *l;
	l = node[index].list;
	int i;
	for (i = 0; i < node[index].listnum; i++) {
		sumlist[i] = _DFS(searchG(l->end, numofV), (sum + l->w), numofV);
		l = l->next;
	}
	for (i = 0; i < node[index].listnum; i++) {
		if (sum < sumlist[i]) {
			sum = sumlist[i];
			indlist = i;
		}
	}
	free(sumlist);
	free(l);
	return sum;
}

void addE(int end, int w, int index) {
	List *cur = node[index].list;
	if (cur == NULL) {
		node[index].list = (List*)malloc(sizeof(List));
		node[index].list->end = end;
		node[index].list->w = w;
		node[index].list->next = NULL;
		return;
	}
	while (cur->next != NULL) {
		cur = cur->next;
	}
	List *n;
	n = (List*)malloc(sizeof(List));
	n->end = end;
	n->w = w;
	n->next = NULL;
	cur->next = n;
	return;
}

inline void addV(G* graph, int numofV, int start) {
	graph[numofV].start = start;
	graph[numofV].list = NULL;
}

int searchG(int val, int numofV) {
	int i;
	for (i = 0; i < numofV; i++) {
		if (node[i].start == val) {
			return i;
		}
	}
}

inline void flagreset(int numofV) {
	numofV--;
	while (numofV >= 0) {
		node[numofV].flag = 0;
		numofV--;
	}
}