#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

int main() {
	FILE *fs1, *fs2;
	struct stat filestat;
	int nRet=0;

	if((fs1 = fopen("image/test.jpg", "rb")) != NULL ) {
		puts("dddd");
		unsigned char *pImgFile;

		fstat(fileno(fs1), &filestat );
		pImgFile = (unsigned char*)malloc(filestat.st_size);
		memset(pImgFile, 0x00, filestat.st_size);
		nRet=fread((unsigned char*)pImgFile, 1, filestat.st_size, fs1);
		fclose(fs1);

		if((fs2 = fopen("TestImgData.lsf", "wb")) != NULL ) {
			nRet = fwrite((void*)pImgFile, filestat.st_size, 1, fs2);
			fclose(fs2);
		}

		free(pImgFile);
	} // end if
	return 0;
}
