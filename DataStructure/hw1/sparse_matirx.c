#include<stdio.h>
#include<stdlib.h>

int cleanBuf(FILE*);
int ADD(FILE*, int **, int**, int);
int MUL(FILE*, int **, int**, int);
int TRN(FILE*, int**, int);
int printMatrix(FILE*, int **, int);
int sparse(FILE*, int**, int, int);

int main() {
	FILE *rEad, *wRite;
	int n, m;
	int i = 0;
	int j = 0;
	char cmd[100] = { 0, };
	int **a, **b;
	rEad = fopen("hw1_input.txt", "r");
	wRite = fopen("hw1_output.txt", "w");

	fscanf(rEad, "%d", &n);
	cleanBuf(rEad);

	a = (int**)malloc(sizeof(int*)*n);
	for (i = 0; i < n; i++) {
		a[i] = (int*)malloc(sizeof(int)*n);
	}
	b = (int**)malloc(sizeof(int*)*n);
	for (i = 0; i < n; i++) {
		b[i] = (int*)malloc(sizeof(int)*n);
	}
	
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			fscanf(rEad, "%d", &a[i][j]);
		}
	}
	cleanBuf(rEad);

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			fscanf(rEad, "%d", &b[i][j]);
		}
	}

	cleanBuf(rEad);
	fscanf(rEad, "%d", &m);

	for (i = 0; i < m; i++) {
		fscanf(rEad, "%s", cmd);
		switch (cmd[0]) {
		case 'A':
			ADD(wRite, a, b, n);
			break;
		case 'M':
			switch (cmd[9]) {
			case 'A':
				MUL(wRite, a, b, n);
				break;
			case 'B':
				MUL(wRite, b, a, n);
				break;
			}
			break;
		case 'T':
			switch (cmd[10]) {
			case 'A':
				TRN(wRite, a, n);
				break;
			case 'B':
				TRN(wRite, b, n);
				break;
			}
			break;
		}
		if (i < m - 1) {
			fprintf(wRite, "*\n");
		}
	}
	free(a);
	free(b);
	fclose(rEad);
	fclose(wRite);
}

int cleanBuf(FILE* rEad) {
	char buf = 0;
	for (;;) {
		fscanf(rEad, "%c", &buf);
		if (buf == '*') {
			break;
		}
	}
	return 0;
}

int ADD(FILE *wRite, int **a, int **b, int n) {
	int **New;
	int i = 0;
	int j = 0;
	int t = 0;
	New = (int**)malloc(sizeof(int*)*n);
	for (i = 0; i < n; i++) {
		New[i] = (int*)malloc(sizeof(int)*n);
	}

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			New[i][j] = a[i][j] + b[i][j];
			if (New[i][j] != 0) {
				t++;
			}
		}
	}
	if (t > (n*n) / 2) {
		printMatrix(wRite, New, n);
	}
	else {
		sparse(wRite, New, n, t);
	}
	free(New);
	return 0;
}

int MUL(FILE *wRite, int **a, int **b, int n) {
	int **New;
	int i, j;
	int k, r;
	int t = 0;
	int sum = 0;
	New = (int**)malloc(sizeof(int*)*n);
	for (i = 0; i < n; i++) {
		New[i] = (int*)malloc(sizeof(int)*n);
	}

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			for (k = 0; k < n; k++) {
				sum += a[i][k] * b[k][j];
			}
			New[i][j] = sum;
			if (New[i][j] != 0) {
				t++;
			}
			sum = 0;
		}
	}
	if (t > n*n / 2) {
		printMatrix(wRite, New, n);
	}
	else {
		sparse(wRite, New, n, t);
	}
	free(New);
	return 0;
}

int TRN(FILE *wRite, int **a, int n) {
	int **New;
	int i = 0;
	int j = 0;
	int t = 0;
	New = (int**)malloc(sizeof(int*)*n);
	for (i = 0; i < n; i++) {
		New[i] = (int*)malloc(sizeof(int)*n);
	}

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			New[i][j] = a[j][i];
			if (a[i][j] != 0) {
				t++;
			}
		}
	}
	if (t > n*n / 2) {
		printMatrix(wRite, New, n);
	}
	else {
		sparse(wRite, New, n, t);
	}
	free(New);
	return 0;
}

int printMatrix(FILE *wRite, int **a, int n) {
	int i, j;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			fprintf(wRite, "%d ", a[i][j]);
		}
		fprintf(wRite, "\n");
	}
	return 0;
}

int sparse(FILE *wRite, int **a, int n, int t) {
	int i, j;
	int **sp = 0;
	int count = 0;
	sp = (int**)malloc(sizeof(int*)*t);
	for (i = 0; i < t; i++) {
		sp[i] = (int*)malloc(sizeof(int)*3);
	}
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			if (a[i][j] != 0) {
				sp[0][count] = i + 1;
				sp[1][count] = j + 1;
				sp[2][count] = a[i][j];
				count++;
			}
		}
	}
	for (i = 0; i < 3; i++) {
		for (j = 0; j < t; j++) {
			fprintf(wRite, "%d ", sp[i][j]);
		}
		fprintf(wRite, "\n");
	}
	return 0;
	free(sp);
}