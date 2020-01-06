#include<stdio.h>

int main() {
	FILE *read, *write;
	read = fopen("seed.inp", "r");
	write = fopen("seed.out", "w");
	int map[100][100] = { 0, };
	int n, m;
	int i, j;
	fscanf(read, "%d %d", &n, &m);
	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++) {
			fscanf(read, "%d", &map[i][j]);
		}
	}
	for (j = 1; j < m; j++) {
		for (i = 0; i < n; i++) {
			int large;
			large = map[i][j - 1];
			if (i != 0) {
				if (large < map[i - 1][j - 1])
					large = map[i - 1][j - 1];
			}
			if (i != n - 1) {
				if (large < map[i + 1][j - 1])
					large = map[i + 1][j - 1];
			}
			map[i][j] += large;
		}
	}
	int large = map[0][m];
	for (i = 0; i < n; i++) {
		if (large < map[i][m-1])
			large = map[i][m-1];
	}
	fprintf(write, "%d", large);
}