#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	char *ptr = malloc(10);
	char *tmp = malloc(10);

	strcpy(ptr, "text");

	printf("realloc() 호출 전 ptr의 값: %p\n", ptr);
	ptr = realloc(ptr, 50);
	printf("realloc() 호출 후 ptr의 값: %p\n", ptr);

	strcpy(ptr+strlen(ptr), " appended text");
	printf("%s\n", ptr);

	free(ptr);
	free(tmp);


	return 0;
}


