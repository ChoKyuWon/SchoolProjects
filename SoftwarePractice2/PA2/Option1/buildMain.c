#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

void indexBuilder(char* inputFileNm, char* indexFileNm);

int main(int argc, char** argv) {
	char *inputFileNm, *indexFileNm;
	int inputFileNmLen, indexFileNmLen;
	int i;

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

	// index file name
	indexFileNm = (char *)malloc(sizeof(char)*indexFileNmLen);
	sprintf(indexFileNm, "%s_index", inputFileNm);
	
	/*************************************
	 * Index Builder
	 *************************************/
	indexBuilder(argv[1], indexFileNm);
	return 0;
}
