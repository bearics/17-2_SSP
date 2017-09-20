#include <unistd.h>
#include <stdio.h>

int main() {
	int i;
	for(i=0; i<3; i++) {
		printf("%d\n",i);
	}
	printf("execl return : %d", execl("/bin/ls", "ls", "-l", NULL));

	for(i=0; i<3; i++) {
		printf("%d\n", i);
	}

	return 0;
}
