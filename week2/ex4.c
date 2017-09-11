#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define BUF_SIZE 512	
#define PERM 0644		// new file permission

// copy name1 to name2
int attachfile(const char *name1, const char *name2) {
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
	printf("Attach \"test.in file\" to \"test.out file\"\n");
	copyfile("test.in", "test.out");
	return 0;
}

