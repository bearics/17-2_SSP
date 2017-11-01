#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	struct timeval tpend;
	struct timeval tpstart;
	if(gettimeofday(&tpstart, NULL)){
		fprintf(stderr, "Failed to get");
		return 1;
	}
	printf("%ld\n", tpstart.tv_usec);
	if(gettimeofday(&tpend, NULL)){
		fprintf(stderr, "Failed to get");
		return 1;
	}
	printf("%ld\n", tpend.tv_usec);
}
	
