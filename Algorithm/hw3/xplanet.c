#include<stdio.h>

int STRlen(char * arry);

int main() {
	FILE *write, *read;
	int n;
	int max = 0;
	int i, j;
	int ABC[27] = { 0, };
	read = fopen("xplanet.inp", "r");
	write = fopen("xplanet.out", "w");
	char DNA[100][1000] = { 0, };
	fscanf(read, "%d", &n);
	for (i = 0; i < n; i++) {
		fscanf(read, "%s", DNA[i]);
	}
	for (i = 0; i < n; i++) {
		int index = 0;
		int tmp_max = 0;
		for (j = 0; j < STRlen(DNA[i]) - 2; j++) {
			index = 0;
			index += (DNA[i][j] - 'A') * 9;
			index += (DNA[i][j + 1] - 'A') * 3;
			index += (DNA[i][j + 2] - 'A');
			ABC[index]++;
		}
	}
	for (j = 0; j < 27; j++) {
		if (ABC[j] > max)
			max = ABC[j];
	}
	fprintf(write, "%d", max);
}

int STRlen(char * arry) {
	int res;
	for (res = 0;; res++) {
		if (arry[res] == 0)
			return res;
	}
}