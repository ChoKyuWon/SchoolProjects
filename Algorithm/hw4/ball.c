#include<stdio.h>
#include<stdlib.h>

int main() {
	FILE *read, *write;
	read = fopen("ball.inp", "r");
	write = fopen("ball.out", "w");
	int n, m, i, j;
	int *pipe, *balls;
	int *pipball;
	fscanf(read, "%d", &n);
	pipe = (int*)malloc(sizeof(int)*n);
	pipball = (int*)malloc(sizeof(int)*n);
	for (i = 0; i < n; i++) {
		fscanf(read, "%d", &pipe[i]);
		pipball[i] = pipe[i];
	}
	pipball[0]++;
	for (i = 1; i < n; i++) {
		pipball[i] += pipball[i - 1];
	}
	fscanf(read, "%d", &m);
	balls = (int*)malloc(sizeof(int)*m);
	for (i = 0; i < m; i++) {
		fscanf(read, "%d", &balls[i]);
	}
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			if (balls[i] > pipball[j])
				continue;
			else
				break;
		}
		fprintf(write, "%d\n", j + 1);
	}
}