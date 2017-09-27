#include <stdio.h>
#include <stdlib.h>


int main(void)
{
	int *trash;
	int count = 0;
	
	while(1){
		trash = (int*)malloc(100000000 * sizeof(int));
		printf("%d\n", count++);
	}

	free(trash);

	
	return 0;
}
