#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define BUFSIZE 256
#define FIFO_PERM (S_IRUSR | S_IWUSR)
#define FIFO_FILE "/home/bearics/univ/17-2_SSP/week9/hw/fifo"

int main(int argc, char *argv[]) {

	char buf[BUFSIZE];
	int fd, nread;

	if(argc != 2) { 	// 인자값이 있는지 확인하고 없으면 오류 출력 
		fprintf(stderr, "No, fiflname. Usage:%s pipename\n", argv[0]);
		return 1;
	}

	if(mkfifo(argv[1], 0666 ) == -1) {	// fifo파일 생성
		if(errno != EEXIST) {
			fprintf(stderr, "[%ld]:failed to ccreate named pipe %s: %s\n", (long)getpid(), argv[1], strerror(errno));
			return 1;
		}
	}

	if(( fd = open(argv[1], O_RDWR)) == -1 ) { // fifo파일 오픈한다
		fprintf(stderr, "failed to open named pipe %s for write : %s \n", argv[1], strerror(errno));
		return 1;
	}

	while(1) {
		memset(buf, 0x00, BUFSIZE);	// 버퍼를 모두 초기화 한다
		printf("[send] : ");	
		scanf("%s",buf);			// 보낼 문자열을 버퍼에 저장한다.
		write( fd, buf, strlen(buf)+1);	// 문자열을 파이프에 입력한다.
		if(strstr(buf, "quit")) {	// quit 문자열이 있으면 종료 한다.
			printf("Finishing... \n");
			sleep(1);
			break;
		}
	}
	close( fd);	// open했던 fd를 종료 한다.

	return 0;
}
