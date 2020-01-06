#include<stdio.h>
#include<stdlib.h>

typedef struct tree {
	int data;
	struct tree *rc;
	struct tree *lc;
}TREE;

typedef struct edge {
	int start;
	int end;
}EDGE;

int insT(TREE*, int);
TREE* searchT(TREE*, int);
int printT(TREE*);
int initT(TREE*);

FILE *read, *write;
EDGE e2[2000];
int num_de, num_ne;
int nnum;
int nnum2;

int main() {
	read = fopen("hw3_input.txt", "r");
	write = fopen("hw3_output.txt", "w");

	TREE *t1;
	int node2[2100] = { 0, };
	t1 = (TREE*)malloc(sizeof(TREE));
	initT(t1);

	int i, count = 0;
	int count2 = 0;
	TREE *temp[1500];
	fscanf(read, "%d", &nnum);
	for (i = 0; i < nnum - 1; i++) {
		int d1, d2;
		TREE *s1, *n1, *n2, *s2;
		n1 = (TREE*)malloc(sizeof(TREE));
		n2 = (TREE*)malloc(sizeof(TREE));
		initT(n1);
		initT(n2);
		fscanf(read, "%d %d", &d1, &d2);

		e2[i].start = d1;
		e2[i].end = d2;
		node2[count++] = d1;
		node2[count++] = d2;

		n1->data = d1;
		n2->data = d2;
		if (d1 > d2) {
			n1->lc = n2;
		}
		else {
			n1->rc = n2;
		}

		s1 = searchT(t1, d1);
		if (s1 == NULL) {
			if (t1->lc == NULL && t1->rc == NULL) {
				free(t1);
				t1 = n1;
			}
			else {
				s2 = searchT(t1, d2);
				if (s2 == NULL) {
					temp[count2++] = n1;
				}
				else {
					n2->lc = s2->lc;
					n2->rc = s2->rc;
					free(s2);
					t1 = n1;
				}
			}
		}
		else {
			free(n1);
			if (d1 > d2) {
				s1->lc = n2;
			}
			else {
				s1->rc = n2;
			}
		}
	}
	int x = 0;
	int t5 = count2;
	while (1) {
		int j;
		for (i = 0; i < count2; i++) {
			TREE *s, *n;
			s = searchT(t1, temp[i]->data);
			if (s == NULL) {
				if (temp[i]->lc == NULL) {
					s = searchT(t1, temp[i]->rc->data);
					if (s == NULL) continue;
					else {
						x++;
						temp[i]->rc->lc = s->lc;
						temp[i]->rc->rc = s->rc;
						t1 = temp[i];
						for (j = i; j < count2; j++) {
							temp[j] = temp[j + 1];
						}
						i--;
						count2--;
						if (x == t5) break;
					}
				}
				else {
					s = searchT(t1, temp[i]->lc->data);
					if (s == NULL) continue;
					else {
						x++;
						temp[i]->lc->lc = s->lc;
						temp[i]->lc->rc = s->rc;
						t1 = temp[i];
						for (j = i; j < count2; j++) {
							temp[j] = temp[j + 1];
						}
						i--;
						count2--;
						if (x == t5) break;
					}
				}
			}
			else {
				x++;
				if (temp[i]->lc == NULL) {
					s->rc = temp[i]->rc;
				}
				else {
					s->lc = temp[i]->lc;
				}
				for (j = i; j < count2; j++) {
					temp[j] = temp[j + 1];
				}
				i--;
				count2--;
				if (x == t5) break;
			}
		}
		if (x == t5) break;
	}
	
	fscanf(read, "%d", &nnum2);
	if (nnum >= nnum2) {
		for (i = 0; i < count; i++) {
			node2[i] = 0;
			e2[i].start = 0;
			e2[i].end = 0;
		}
		count = 0;
		for (i = 0; i < nnum2 - 1; i++) {
			int d1, d2;
			fscanf(read, "%d %d", &d1, &d2);
			e2[i].start = d1;
			e2[i].end = d2;
			node2[count++] = d1;
			node2[count++] = d2;
		}
		for (i = 0; i < count; i++) {
			insT(t1, node2[i]);
		}
		fprintf(write, "%d\n", t1->data);
		printT(t1);
		for (i = 0; i < nnum2; i++) {
			if (e2[i].start == e2[i].end) continue;
			else num_de++;
		}
		fprintf(write, "%d\n%d", num_ne, num_de);
	}
	else {
		initT(t1);
		count2 = 0;
		for (i = 0; i < nnum - 1; i++) {
			int d1, d2;
			TREE *s1, *n1, *n2, *s2;
			n1 = (TREE*)malloc(sizeof(TREE));
			n2 = (TREE*)malloc(sizeof(TREE));
			initT(n1);
			initT(n2);
			fscanf(read, "%d %d", &d1, &d2);
			n1->data = d1;
			n2->data = d2;
			if (d1 > d2) {
				n1->lc = n2;
			}
			else {
				n1->rc = n2;
			}

			s1 = searchT(t1, d1);
			if (s1 == NULL) {
				if (t1->lc == NULL && t1->rc == NULL) {
					free(t1);
					t1 = n1;
				}
				else {
					s2 = searchT(t1, d2);
					if (s2 == NULL) {
						temp[count2++] = n1;
					}
					else {
						n2->lc = s2->lc;
						n2->rc = s2->rc;
						free(s2);
						t1 = n1;
					}
				}
			}
			else {
				free(n1);
				if (d1 > d2) {
					s1->lc = n2;
				}
				else {
					s1->rc = n2;
				}
			}
		}
		int x = 0;
		t5 = count2;
		while (1) {
			int j;
			for (i = 0; i < count2; i++) {
				TREE *s, *n;
				s = searchT(t1, temp[i]->data);
				if (s == NULL) {
					if (temp[i]->lc == NULL) {
						s = searchT(t1, temp[i]->rc->data);
						if (s == NULL) continue;
						else {
							x++;
							temp[i]->rc->lc = s->lc;
							temp[i]->rc->rc = s->rc;
							t1 = temp[i];
							for (j = i; j < count2; j++) {
								temp[j] = temp[j + 1];
							}
							i--;
							count2--;
							if (x == t5) break;
						}
					}
					else {
						s = searchT(t1, temp[i]->lc->data);
						if (s == NULL) continue;
						else {
							x++;
							temp[i]->lc->lc = s->lc;
							temp[i]->lc->rc = s->rc;
							t1 = temp[i];
							for (j = i; j < count2; j++) {
								temp[j] = temp[j + 1];
							}
							i--;
							count2--;
							if (x == t5) break;
						}
					}
				}
				else {
					x++;
					if (temp[i]->lc == NULL) {
						s->rc = temp[i]->rc;
					}
					else {
						s->lc = temp[i]->lc;
					}
					for (j = i; j < count2; j++) {
						temp[j] = temp[j + 1];
					}
					i--;
					count2--;
					if (x == t5) break;
				}
			}
			if (x == t5) break;
		}
		for (i = 0; i < count; i++) {
			insT(t1, node2[i]);
		}
		fprintf(write, "%d\n", t1->data);
		printT(t1);
		for (i = 0; i < nnum; i++) {
			if (e2[i].start == e2[i].end) continue;
			else num_de++;
		}
		fprintf(write, "%d\n%d", num_ne, num_de);
	}
}

int insT(TREE *root, int val) {
	if (root->data > val) {
		if (root->lc == NULL) {
			TREE *newnode;
			int i, c = 0;
			newnode = (TREE*)malloc(sizeof(TREE));
			initT(newnode);
			newnode->data = val;
			root->lc = newnode;
			for (i = 0; i < nnum; i++) {
				if (root->data == e2[i].start && val == e2[i].end) {
					e2[i].start = e2[i].end;
					c++;
					break;
				}
				if (root->data == e2[i].end && val == e2[i].start) {
					e2[i].start = e2[i].end;
					c++;
					break;
				}
			}
			if (c == 0) {
				num_ne++;
			}
		}
		else
			insT(root->lc, val);
	}
	else if (root->data < val) {
		if (root->rc == NULL) {
			TREE *newnode;
			int i, c = 0;
			newnode = (TREE*)malloc(sizeof(TREE));
			initT(newnode);
			newnode->data = val;
			root->rc = newnode;
			for (i = 0; i < nnum; i++) {
				if (root->data == e2[i].start && val == e2[i].end) {
					e2[i].start = e2[i].end;
					c++;
					break;
				}
				if (root->data == e2[i].end && val == e2[i].start) {
					e2[i].start = e2[i].end;
					c++;
					break;
				}
			}
			if (c == 0) {
				num_ne++;
			}
		}
		else
			insT(root->rc, val);
	}
	return 0;
}

int initT(TREE *root) {
	root->rc = NULL;
	root->lc = NULL;
	return 1;
}

int printT(TREE* root) {
	if (root->lc != NULL) {
		fprintf(write, "%d %d\n", root->data, root->lc->data);
		if (root->rc != NULL) {
			fprintf(write, "%d %d\n", root->data, root->rc->data);
		}
		printT(root->lc);
		if (root->rc != NULL) {
			printT(root->rc);
		}
	}
	else if (root->rc != NULL) {
		fprintf(write, "%d %d\n", root->data, root->rc->data);
		printT(root->rc);
	}
	else return 0;
}
TREE* searchT(TREE *root, int val) {
	TREE *ret;
	if (val>root->data) {
		if (root->rc != NULL) {
			ret = searchT(root->rc, val);
			return ret;
		}
		else {
			return NULL;
		}
	}
	if (val<root->data) {
		if (root->lc != NULL) {
			ret = searchT(root->lc, val);
			return ret;
		}
		else {
			return NULL;
		}
	}
	if (val == root->data) {
		return root;
	}
}