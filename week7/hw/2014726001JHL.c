
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#define TIMER_MSG "Received Timer Interrupt\n"

static timer_t timerid;
static struct itimerspec value;

static void interrupt()
{
	printf("%s", TIMER_MSG);
}

static int setinterrupt()
{
	struct sigaction act;
	
	act.sa_flags = 0;
	act.sa_sigaction = interrupt;
	if( sigaction(SIGALRM, &act, NULL) == -1 )
		return -1;
	return 0;
}

static int setperiodic(int sec)
{
	if(timer_create(CLOCK_REALTIME, NULL, &timerid)==-1)
		return -1;
	
	value.it_interval.tv_sec = (long)sec;
	value.it_interval.tv_nsec = 0;

	value.it_value = value.it_interval;
	return timer_settime(timerid, 0, &value, NULL);
}

static void change()
{	
	timer_delete(timerid);
	if(value.it_interval.tv_sec == (long)3)
		setperiodic(5);
	else 
		setperiodic(3);
}

static int setchange()
{
	struct sigaction act;
	
	act.sa_flags = 0;
	act.sa_sigaction = change;
	if( sigaction(SIGINT, &act, NULL) == -1 )
		return -1;
	return 0;
}

int main()
{
	if(setinterrupt() == -1)
	{
		perror("Failed to setup SIGALRM handler");
		return 1;
	}
	if(setperiodic(3) == -1)
	{
		perror("Failed to setup periodic handler");
		return 1;
	}
	if(setchange() == -1)
	{
		perror("Failed to Change Sec");
		return 1;
	}
	int i=0;
	for(;;)
	{
		printf("%d\n", i++);
		sleep(1);
		//	pause();
	}
	return 0;
}
