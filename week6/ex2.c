#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>

void show_mask(){
	sigset_t set;
	if(sigprocmask(0, NULL, &set)<0)
		fprintf(stderr, "sigprocmask error");
	if(sigismember(&set, SIGINT))
		printf("SIGINT");
	if(sigismember(&set, SIGQUIT))
		printf("SIGQUIT");
	if(sigismember(&set, SIGALRM))
		printf("SIGALRM");
	if(sigismember(&set, SIGUSR1))
		printf("SIGUSR1");
	printf("\n");
}

int main(){
	sigset_t newmask, oldmask;
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGQUIT);

	if(sigprocmask(SIG_BLOCK, &newmask, &oldmask)<0)
		fprintf(stderr,  "sigprocmask error");
	show_mask();
	if(sigprocmask(SIG_SETMASK, &oldmask, NULL)<0)
		fprintf(stderr,  "sigprocmask error");
	exit(0);
}
