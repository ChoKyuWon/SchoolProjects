//-----------------------------------------------------------
//
// SWE2007: Software Experiment II (Fall, 2018)
//
// Skeleton code for PA#1
//
// CSLab, Sungkyunkwan University
//
//-----------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

void indexBuilder(char* inputFileNm, char* indexFileNm);
void indexPrinter(char* indexFileNm, char* outputFileNm);

int main(int argc, char** argv) {
	char *inputFileNm, *indexFileNm, *outputFileNm;
	int inputFileNmLen, indexFileNmLen, outputFileNmLen;
	int i;
	struct timeval start_t, end_t, result_t;

	if(argc!=2) {
		printf("Usage : %s [bible text file]\n", argv[0]);
		return 1;
	}
	
	// Calculate input file name length
	for(i=0; argv[1][i]!='.'; i++);
	inputFileNmLen=i;
	inputFileNm = (char *)malloc(sizeof(char)*inputFileNmLen+1);

	for(i=0; i<inputFileNmLen; i++)
		inputFileNm[i] = argv[1][i];
	inputFileNm[i] = '\0';

	indexFileNmLen = inputFileNmLen+7;
	outputFileNmLen = inputFileNmLen+8;

	// index file name
	indexFileNm = (char *)malloc(sizeof(char)*indexFileNmLen);
	sprintf(indexFileNm, "%s_index", inputFileNm);
	
	// output file name
	outputFileNm = (char *)malloc(sizeof(char)*outputFileNmLen);
	sprintf(outputFileNm, "%s_output", inputFileNm);
	
	/*************************************
	 * Index Builder
	 *************************************/
	printf("** Index Builder : Start\t**\n");
	gettimeofday(&start_t, NULL);

	// call index bulder
	indexBuilder(argv[1], indexFileNm);
	gettimeofday(&end_t, NULL);
	
	timersub(&end_t, &start_t, &result_t);
	printf("   Elapsed Time  : %ld(usec)\n",
			100000*(result_t.tv_sec)+result_t.tv_usec);
	printf("** Index Builder : End\t\t**\n");
	
	printf("\n");
	/*************************************
	 * Index Printer
	 *************************************/
	printf("** Index Printer : Start\t**\n");
	indexPrinter(indexFileNm, outputFileNm);
	printf("** Index Printer : End\t\t**\n");

	return 0;
}
