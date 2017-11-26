#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define BUFSIZE 256
#define FIFO_MODES O_RDONLY

int main(int argc, char *argv[]) {

	char buf[BUFSIZE];
	int fd, n;

	if( (fd = open(argv[1], FIFO_MODES) ) < 0 ) {	// fifo파일을 오픈한다.
		fprintf(stderr, "failed to open named pipe %s for write : %s \n", argv[1], strerror(errno));
		return 1;
	}else {
		printf("Connected. \n");
	}

	while(1) {
		memset( buf, 0x00, BUFSIZE);	// 버퍼를 초기화한다.
		if(read(fd,buf,BUFSIZE) < 0 ) {	// fifo파일에서 문자열을 읽어와 buf에 저장한다.
			break;
		}
		printf("[read] : ");
		puts(buf);		// 버퍼에 저장된 문자열을 출력한다.
		fflush(stdout);	// stdout 버퍼에 남아 있는 찌꺼기를 제거한다.
		if(strstr(buf, "quit")) {	// quit이 있으면 종료 한다.
			printf("Finishing... \n");
			break;
		}
	}
	close( fd);	// 프로그램 종료 전에 fd와 fifo파일을 제거한다.
	unlink(argv[1]);

	return 0;
}
