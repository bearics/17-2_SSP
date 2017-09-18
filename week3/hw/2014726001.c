#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	int fd1, fd2;
	struct stat filestat;
	int nread=0;
	char *buf;

	if((fd1 = open(argv[1], O_RDONLY)) > 0) {

		if(stat(argv[1], &filestat) == -1)
		{
			perror("fopen");	
		}
		buf = (char*)malloc((int)filestat.st_size);
		memset(buf, 0x00, filestat.st_size);
		nread = read(fd1, buf, (int)filestat.st_size);
		puts("복사 완료");
		if( (fd2 = open(argv[2], O_WRONLY | O_CREAT, 0644)) > 0) {
			write(fd2, buf, nread);
			close(fd2);
			close(fd1);
		}else {
			close(fd2);
			close(fd1);
			printf("저장하는데 실패했습니다.\n");
		}
	} else {
		close(fd1);
		printf("파일 열기에 실패했습니다.\n");
	}

	return 0;
}
