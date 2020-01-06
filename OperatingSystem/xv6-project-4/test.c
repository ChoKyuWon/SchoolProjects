#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char **argv){
	int child, parent = getpid();

	//start
	printf(1, "[Case 2. Setnice, Yield] Start\n");
	if((child = fork()) == 0) { /* child */
		//2
		printf(1, "   -...state 2 pid %d, nice %d...\n", getpid(), getnice(getpid()));
		if(fork() == 0) {
			//4
			printf(1, "   -...state 4 pid %d, nice %d...\n", getpid(), getnice(getpid()));
			setnice(parent, 18);
			//10
			printf(1, "   -...state 10 pid %d, nice %d...\n", getpid(), getnice(getpid()));
			yield();
		} else {
			//3
			printf(1, "   -...state 3 pid %d, nice %d...\n", getpid(), getnice(getpid()));
			yield();
			//8
			printf(1, "   -...state 8 pid %d, nice %d...\n",getpid(), getnice(getpid()));
			if(fork() == 0){
				//11
				printf(1, "   -...state 11 pid %d, nice %d...\n", getpid(), getnice(getpid()));
			} else{
				//9
				printf(1, "   -...state 9 pid %d, nice %d...\n", getpid(), getnice(getpid()));
				wait();
			}
			wait();
		}
	}
	else {	/* parent */
		//1
		printf(1, "   -...state 1 pid %d, nice %d...\n", getpid(), getnice(getpid()));
		setnice(child, 19);
		//5
		printf(1, "   -...state 5 pid %d, nice %d...\n", getpid(), getnice(getpid()));
		if(fork() == 0) {
			//7
			printf(1, "   -...state 7 pid %d, nice %d...\n", getpid(), getnice(getpid()));
			if(fork() == 0)	setnice(getpid(), 20);
			else	wait();
		} else{
			//6
			printf(1, "   -...state 6 pid %d, nice %d...\n", getpid(), getnice(getpid()));
			wait();
		}
		wait();
	}

	//end
	exit();
}