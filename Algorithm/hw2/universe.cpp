#include<stdio.h>
#include<stdlib.h>

int issame(int *, int*, int);

int main() {
	FILE *read, *write;
	int universe[10][3000] = { 0, };
	int rank[10][3000] = { 0, };
	int i, j, k, m, n;
	int numuni = 1;
	read = fopen("universe.inp", "r");
	write = fopen("universe.out", "w");
	int flag = 0;
	fscanf(read, "%d %d", &m, &n);
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			fscanf(read, "%d", &universe[i][j]);
		}
	}
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			for (k = 0; k < n; k++) {
				if (universe[i][j] > universe[i][k])
					rank[i][j]++;
			}
		}
	}

	for (i = 0; i < m; i++) {
		for (j = i + 1; j < m; j++) {
			for (k = 0; k < n; k++) {
				if (rank[i][k] != rank[j][k]) {
					flag++;
					break;
				}
			}
			if (flag == 0) {
				numuni++;
			}
			flag = 0;
		}
		if (numuni != 1)
			break;
	}
	if (numuni == 1)
		numuni -= 1;
	fprintf(write, "%d", numuni);
}

int issame(int *a, int *b, int length) {
	int i;
	for (i = 0; i < length; i++) {
		if (a[i] != b[i])
			return 0;
	}
	return 1;
}