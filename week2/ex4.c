#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define BUF_SIZE 512	
#define PERM 0644		// new file permission

// Write on name file
int writefile(const char *name) {
	int file;
	char str[20];
	int cnt=0;
	if((file = open(name, O_WRONLY|O_CREAT|O_TRUNC, 0644)) == -1){
		printf("bang");
		return(-1);	// fail to make file
	}
	printf("Input 10 chars : ");
	scanf("%s", str);
	while( str[++cnt] != '\0' ) {}
	if(write(file, str, cnt) < 0 ){
		close(file);
		return(-1);
	}

	close(file);
	return 0;
}

int readfile(const char *name) {
	int file;
	char buf[BUF_SIZE];
	int nread=0;
	if((file = open(name, O_RDONLY)) == -1)
		return(-1);

	if( (nread = read(file, buf, BUF_SIZE)) > 0 ) {
		printf("%s\n", buf);
	}
	else
		return(-2);

	close(file);

	return 0;
}

// copy name1 to name2
int copyfile(const char *name1, const char *name2) {
	int infile, outfile;
	ssize_t nread;
	char buf[BUF_SIZE];
	if((infile = open(name1, O_RDONLY)) == -1)
		return(-1);

	if((outfile = open(name2, O_WRONLY|O_CREAT|O_TRUNC,PERM)) == -1) {
		close(infile);
		return(-1);
	}
	// read name1 file BUF_SIZE bytes
	while((nread = read(infile, buf, BUF_SIZE)) > 0 ) {
		if(write(outfile, buf, nread) < nread ) {
			close(infile);
			close(outfile);
			return(-3);		// error write
		}
	}
	close(infile);
	close(outfile);
	if(nread == -1)
		return(-4);		// error at last read
	else
		return(0);
}

int main() {
	printf("Write in \"test.in file\"\n");
	writefile("test.in");
	printf("Read \"test.in file\"\n");
	readfile("test.in");
	printf("Copy \"test.in file\" to \"test.out file\"\n");
	copyfile("test.in", "test.out");
	return 0;
}

