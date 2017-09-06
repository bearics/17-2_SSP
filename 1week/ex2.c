#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
char *workfile="junk"; //define the name work file
int main(){
	int fd; 
	/* open using O_RDWR defined <fcntl.h> */
	/* open to read/write the file */
	if ((fd = open (workfile, O_RDWR)) ==-1) {
		 printf ("Couldn't open %s\n", workfile);
		 exit (1); // exit cause error 
	}
	 /* 프로그램의 나머지 부분을 작성 뒤따른다 */
	return  0; // ordinary exit
}
