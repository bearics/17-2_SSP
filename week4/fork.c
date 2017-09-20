#include <stdio.h>
#include <unistd.h>

int main()
{
	fork();
	printf("%d\n", getpid() );

	fork();
	printf("%d\n", getpid() );
	fork();
	printf("%d\n", getpid() );
	return 0;
}
