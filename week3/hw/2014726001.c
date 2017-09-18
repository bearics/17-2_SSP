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
	int size = 0;

	if(fd1 = open(argv[1], O_WRONLY, 0644) > 0) {
		stat("./testimg", &filestat);
		printf("stat ok %d ", (int)filestat.st_size);
		buf = (char*)malloc((int)filestat.st_size);
		nread = read(fd1, buf, (int)filestat.st_size);
		printf("%d", nread);
		/*
		if(fd2 = open(argv[2], O_WRONLY | O_CREAT | O_EXCL, 0644) > 0) {
			write(fd2, buf, nread);
			close(fd2);
			close(fd1);
		}else {
			close(fd2);
			close(fd1);
			printf("저장하는데 실패했습니다.");
		}*/
	} else {
		printf("파일 열기에 실패했습니다.");
	}
	/*
	if((fs1 = fopen("image/test.jpg", "rb")) != NULL ) {
		puts("dddd");
		unsigned char *pImgFile;

		fstat(fileno(fs1), &filestat );
		pImgFile = (unsigned char*)malloc(filestat.st_size);
		memset(pImgFile, 0x00, filestat.st_size);
		nread=read((unsigned char*)pImgFile, 1, filestat.st_size, fs1);
		fclose(fs1);

		if((fs2 = fopen("TestImgData.lsf", "wb")) != NULL ) {
			nRet = fwrite((void*)pImgFile, filestat.st_size, 1, fs2);
			fclose(fs2);
		}

		free(pImgFile);
	} // end if
	*/
	return 0;
}
