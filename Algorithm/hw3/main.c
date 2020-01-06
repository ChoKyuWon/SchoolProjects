#include "ds.h"
#include <string.h>
#include<time.h>

# ifdef sun
# define const
# endif

extern const char *
okfail(int val)
{
    return val ? "[ OK ]" : "[FAIL]";
}

int array[1000000];
//prototype
void read_input(char *, char *, int);
void convert_PARRAY(char *, PARRAY);
void convert_STACK(char *, STACK);
void convert_QUEUE(char *, QUEUE);
void convert_HEAP(char *, HEAP);
void swap(int *, int *);
int Partition(int, int);
void merge(int, int, int);

//sorting function 
void rsort(int size);
void qsort(int, int);
void mergesort(int, int);
HEAP Heap_h(HEAP);

int main(int argc, char *argv[])
{
	printf("Algorithm - Homework # 2 - <2017314875>\n");	
	
	FILE *write;
	char *filename;
	char *filename_w;
	char buf[2] = { 'r', };
	int n, m;
	time_t start = 0, end = 0;
	float timegap = 0;
	filename_w = (char*)malloc(sizeof(char) * 29);
	filename = (char*)malloc(sizeof(char) * 29);
	strcpy(filename, "..\\input\\input_200000_0.txt");
	strcpy(filename_w, "..\\input\\input_200000_0.out");
	for (n = 2; n < 10; n += 2) {
		for (m = 0; m < 10; m++)
		{
			filename[15] = n + '0';
			filename[22] = m + '0';
			filename_w[15] = n + '0';
			filename_w[22] = m + '0';
			write = fopen(filename_w, "w");
			read_input(filename, buf, n*100000);
			start = clock();
			qsort(0, n * 100000);
			int i;
			end = clock();
			for (i = 0; i < n * 100000; i++) {
				fprintf(write, "%d ", array[i]);
			}
			timegap = (float)(end - start) / (CLOCKS_PER_SEC);
			fprintf(write, "qsort time is %1f", timegap);
			fclose(write);
		}
	}
	n = 10;
	strcpy(filename, "..\\input\\input_1000000_0.txt");
	strcpy(filename_w, "..\\input\\input_1000000_0.out");
	for (m = 0; m < 10; m++)
	{
		filename[23] = m + '0';
		filename_w[23] = m + '0';
		write = fopen(filename_w, "w");
		read_input(filename, buf, n * 100000);
		start = clock();
		qsort(0, n * 100000);
		int i;
		end = clock();
		for (i = 0; i < n * 100000; i++) {
			fprintf(write, "%d ", array[i]);
		}
		timegap = (float)(end - start) / (CLOCKS_PER_SEC);
		fprintf(write, "qsort time is %1f", timegap);
		fclose(write);
	}
	return 0;
}

void read_input(char *filename, char *buf, int size)
{
	FILE *ptr_file;
	ptr_file = fopen(filename, buf);
	int i;
	if (ptr_file==NULL)
	{
		printf("File is not exist.\n");
		exit(1);
	}
	for (i = 0; i < size; i++) {
		fscanf(ptr_file, "%d", &array[i]);
	}
}

void convert_HEAP(char *buf, HEAP heap)
{
       
} 

void swap(int *a, int *b) {
	int tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}
int find(int *array, int size) {
	int i;
	int max = -1;
	for (i = 0; i < size; i++) {
		if (array[i] > max)
			max = array[i];
	}
	return max;
}
void rsort(int size) {
	int i;
	int *tmp;
	int SD = 1;
	int max = find(array, size);
	tmp = (int*)malloc(sizeof(int)*size);

	while (max / SD > 0) {
		int bucket[10] = { 0, };

		for (i = 0; i < size; i++) {
			bucket[(array[i] / SD) % 10]++;
		}

		for (i = 1; i < 10; i++) {
			bucket[i] += bucket[i - 1];
		}

		for (i = size - 1; i >= 0; i--) {
			tmp[bucket[(array[i] / SD) % 10]] = array[i];
			bucket[(array[i] / SD) % 10]--;
		}

		for (i = 0; i < size; i++) {
			array[i] = tmp[i];
		}

		SD *= 10;
	}
	free(tmp);
}
void qsort(int p, int r) {
	if (p < r) {
		int q = Partition(p, r);
		qsort(p, q);
		qsort(q + 1, r);
	}
}
int Partition(int p, int r) {
	int x = array[r];
	int i = p - 1;
	int j;
	for (j = p; j <= r - 1; j++) {
		if (array[j] <= x) {
			i++;
			swap(&array[j], &array[i]);
		}
	}
	swap(&array[i + 1], &array[r]);
	return i + 1;
}
void mergesort(int p, int r) {
	if (p < r) {
		int q = (p + r) / 2;
		mergesort(p, q);
		mergesort(q + 1, r);
		merge(p, q, r);
	}
}
void merge(int p, int q, int r) {
	int n1 = q - p + 1;
	int n2 = r - q;
	int *L, *R;
	int i;
	L = (int*)malloc(sizeof(int)*(n1 + 1));
	R = (int*)malloc(sizeof(int)*(n2 + 1));
	for (i = 0; i < n1; i++) {
		L[i] = array[p + i - 1];
	}
	for (i = 0; i < n2; i++) {
		R[i] = array[q+i];
	}
	L[n1] = 0x7FFFFFFF;
	R[n2] = 0x7FFFFFFF;
	for (i = p; i <= r; i++) {
//		if(L[])
	}
}