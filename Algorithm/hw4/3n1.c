#include<stdio.h>
#include<stdlib.h>

typedef struct tree {
	unsigned long long data;
	struct tree *lchild;
	struct tree *rchild;
}TREE;

void treemake(TREE*, unsigned long long, int);
void init(TREE*);
void searchmin(TREE*, int);
int istree(TREE*, unsigned long long);
unsigned long long min;
TREE *root;

int main() {
	FILE *read, *write;
	int k = 0;
	int tmp = 1;
	root = (TREE*)malloc(sizeof(TREE));
	read = fopen("3n1.inp", "r");
	write = fopen("3n1.out", "w");
	fscanf(read, "%d", &k);
	unsigned long long max;
	max = 1;
	min = 1;
	root->data = 1;
	max = max << k;
	min = min << k;
	init(root);
	root->data = 1;
	treemake(root, 1, k);
	searchmin(root, k);
	fprintf(write, "%lld %lld", min, max);
}

void treemake(TREE* prev, unsigned long long data, int k) {
	if (k == 0)
		return;
	prev->lchild = (TREE*)malloc(sizeof(TREE));
	init(prev->lchild);
	prev->lchild->data = data * 2;
	treemake(prev->lchild, prev->lchild->data, k - 1);
	double save;
	save = (double)(data - 1) / 3;
	if (save == (unsigned long long)save) {
		if (save == 0)
			return;
		if (istree(root, (unsigned long long)save)==1) {
			return;
		}
		prev->rchild = (TREE*)malloc(sizeof(TREE));
		init(prev->rchild);
		prev->rchild->data = (unsigned long long)save;
		treemake(prev->rchild, prev->rchild->data, k - 1);
	}
}

void init(TREE* roo) {
	roo->data = 0;
	roo->lchild = NULL;
	roo->rchild = NULL;
}

void searchmin(TREE* root, int k) {
	if (k > 0) {
		if(root->lchild!=NULL)
			searchmin(root->lchild, k-1);
		if(root->rchild!=NULL)
			searchmin(root->rchild, k-1);
	}
	else {
		if (root->data < min)
			min = root->data;
	}
}

int istree(TREE *tree, unsigned long long data) {
	if (tree->data == data)
		return 1;
	else {
		if (tree->lchild != NULL) {
			if (istree(tree->lchild, data) == 1)
				return 1;
		}
		if (tree->rchild != NULL) {
			if (istree(tree->lchild, data) == 1)
				return 1;
		}
		else if (tree->lchild == NULL && tree->rchild == NULL)
			return 0;
	}
}