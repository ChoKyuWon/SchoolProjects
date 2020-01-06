/*-----------------------------------------------------------
*
* SWE2007: System Software Experiment 2 (Fall 2018)
*
* Skeleton code for PA#1
*
* October ��¥, 2018.
* Sungkyunkwan University
*
* Student Id   : 
* Student Name : 
*
*-----------------------------------------------------------
*/

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void indexPrinter(const char* indexFileNm, const char* outputFileNm)
{
	int fd=open(indexFileNm, O_RDONLY);
	int fd2=open(outputFileNm, O_WRONLY | O_CREAT, 0775);

	while(1){
		char tmp[1]={0};
		if(!read(fd,tmp,1)) break;
		else{
			if(write(fd2,tmp,1)) ;
		}
	}
}