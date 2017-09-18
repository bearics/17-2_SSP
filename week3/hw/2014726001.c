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

	if((fd1 = open(argv[1], O_RDONLY)) > 0) { // 복사할 파일 오픈
		if(stat(argv[1], &filestat) == -1) // 복사할 파일 정보 filestat에 저장
			perror("open"); // 안되면 오류 출력
		buf = (char*)malloc((int)filestat.st_size); // 저장할 buf 할당
		memset(buf, 0x00, filestat.st_size); // buf 더미 값 모두 초기화
		nread = read(fd1, buf, (int)filestat.st_size); // buf에 데이터 저장
		puts("복사 완료");
		if( (fd2 = open(argv[2], O_WRONLY | O_CREAT, 0644)) > 0) { // 새로운 파일 생성
			write(fd2, buf, nread); // 복사할 데이터를 새로운 파일에 저장
			close(fd2);
			close(fd1);
		}else {	// 새로운 파일 생성 실패 시에 오류 출력
			close(fd2);
			close(fd1);
			printf("저장하는데 실패했습니다.\n");
		}
	} else { // 복사할 파일 오픈 실패 시에 오류 출력
		close(fd1);
		printf("파일 열기에 실패했습니다.\n");
	}
	return 0;
}
