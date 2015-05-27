// A simple forking example.

#include <sys/types.h>	// fork, getpid, getppid
#include <sys/wait.h>   // wait
#include <stdlib.h>     // abort
#include <unistd.h>	// fork, getpid, getppid, sleep
#include <time.h>       // time seed for random
#include <errno.h>      // error reporting
#include <stdio.h>	// printf

// our global variable
static int globalData = 0;

// Display PID-data, modify global data, sleep and redisplay

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

	return;
}

int main(int argc, char** argv)
{
	pid_t childPid;
	pid_t childState;
	pid_t returnPid;
	int i, status, r;

	for (i = 0; i < 1; i++) {
	
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
	//r = rand()%6+5;
	r = rand()%1+1;
	
	printf("Parent waiting %d seconds\n", r);
	sleep(r);

	//Check status of children

do{

	returnPid = waitpid(childPid, &status, WUNTRACED | WCONTINUED );
	if(returnPid == -1){
		printf("Error occured\n");
		perror("waitpid");
		exit(EXIT_FAILURE);
	}
	
	if (WIFEXITED(status)) {
		printf("Child exited with status %d\n", WEXITSTATUS(status));
	}
	else if (WIFSIGNALED(status)) {
		printf("killed by signal %d\n", WTERMSIG(status));
	}
	else if (WIFSTOPPED(status)) {
		printf("stopped by signal %d\n", WSTOPSIG(status));
	}
	else if (WIFCONTINUED(status)) {
		printf("continued\n");
	}
	}
while (!WIFEXITED(status) && !WIFSIGNALED(status)); {
	exit(EXIT_SUCCESS);
	}
}


