#include <sys/types.h>	// fork, getpid, getppid
#include <sys/wait.h>   // wait
#include <stdlib.h>	// exit
#include <unistd.h>	// fork, getpid, getppid, sleep
#include <time.h>       // time seed for random
#include <errno.h>      // error reporting
#include <stdio.h>	// printf

// @Param processCount will be displayed in debugging messages.

int service(int processCount) {
	pid_t myPid;
	pid_t parentPid;
	int r;
	
	myPid = getpid();
	parentPid = getppid();
	
	printf("service(%.2d): my pid = %d, parent pid = %d\n", processCount, myPid, parentPid);

	srand(time(NULL));
	r = rand()%6+5;

	printf("service(%.2d): waiting %d seconds\n", processCount, r);
	sleep(r);

	//Randomize exit status

	srand(time(NULL));
	r = rand()%2;
	if (r == 0){
	exit(EXIT_SUCCESS);
	}
	if (r == 1){
	exit(EXIT_FAILURE);
	}
	return 1; //something failed
}

int main(int argc, char** argv){
	
	pid_t childPid, returnPid;
	int i, r, status;

	for (i = 0; i < 3; i++) {
	
		childPid = fork();	
		if (childPid == 0) {
			service(i);
			return 0;
		} 
		else if (childPid == -1) {
			perror("Problem while forking");
			return 1;
		} 
		else {
			printf("main: just started process %d. Waiting one second\n", i);
			sleep(1);
		}
	}

	srand(time(NULL));
	r = rand()%6+5;
	
	printf("Parent waiting %d seconds\n", r);
	sleep(r);

	//Check status of children

	for (i = 0; i < 3; i++) {
	
	//do-while loop until response found
		do {
			returnPid = waitpid(-1, &status, WUNTRACED | WCONTINUED );
			if(returnPid == -1){
				printf("Error occured\n");
				perror("waitpid");
				exit(EXIT_FAILURE);
			}	
	
			if (WIFEXITED(status)) {
				printf("Child with PID %d exited successfully with status %d\n", returnPid, WEXITSTATUS(status));
			}
			else if (WIFSIGNALED(status)) {
				printf("Child with PID %d was killed by signal %d\n", returnPid, WTERMSIG(status));
			}
			else if (WIFSTOPPED(status)) {
				printf("Child with PID %d was stopped by signal %d\n", returnPid, WSTOPSIG(status));
			}
			else if (WIFCONTINUED(status)) {
				printf("Child with PID %d continued.\n", returnPid);
			}
		}
		while (!WIFEXITED(status) && !WIFSIGNALED(status));
			;
	}
	return 0;
}
