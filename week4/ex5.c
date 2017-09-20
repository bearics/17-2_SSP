#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
	int pid;
	int status;
	int spid;
	pid = fork();

	if( pid == 0) {
		sleep(5);
		printf("I will be back %d\n", getpid());
		return 1;
	}else if( pid > 0 ){
		printf("I'm parent %d\n", getpid());
		printf("Press any key and wait\n");
		getchar();

		spid = wait(&status);
		printf("Child process wait success \n");

		printf("PID		: %d\n", spid);
		printf("Exit Value	: %d\n", WEXITSTATUS(status));
		printf("Exit Stat	: %d\n", WIFEXITED(status));
	}else {
		perror("fork error : ");
	}
}


