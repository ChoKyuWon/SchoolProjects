#include<stdio.h>

int factorial(int n);

int main() {
	FILE *read, *write;

	read = fopen("tomjerry.inp", "r");
	write = fopen("tomjerry.out", "w");

	int c, r;
	int res;
	fscanf(read, "%d %d", &c, &r);
	res = factorial(r + c - 2) / factorial(r - 1);
	res /= factorial(c - 1);
	fprintf(write, "%d", res);
}

int factorial(int n) {
	if (n == 0)
		return 1;
	return n * factorial(n - 1);
}