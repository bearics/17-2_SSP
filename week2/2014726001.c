#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define BUF_SIZE 512	
#define PERM 0644		// new file permission

// copy name1 to name2
int combinefile(const char *name1, const char *name2) {
	int infile1, infile2, outfile;
	ssize_t nread;
	char buf[BUF_SIZE];

	// open name1 file
	if((infile1 = open(name1, O_RDONLY)) == -1) {
		close(infile1);
		return(-1);
	}
	// read name1 file BUF_SIZE bytes
	printf("-----%s's data-----\n", name1);
	while((nread = read(infile1, buf, BUF_SIZE)) > 0 ) {
		fputs(buf, stdout);
	}
	printf("\n-----------------------\n");

	// open name2 file
	if((infile2 = open(name2, O_RDONLY)) == -1) {
		close(infile2);
		return(-1);
	}
	
	// read name2 file BUF_SIZE bytes
	printf("-----%s's data-----\n", name2);
	while((nread = read(infile2, buf, BUF_SIZE)) > 0 ) {
		fputs(buf, stdout);
	}
	printf("\n-----------------------\n");

	// open or create test.out file
	printf("Combine two files\n");
	if((outfile = open("test.out", O_WRONLY|O_CREAT|O_TRUNC,PERM)) == -1) {
		close(outfile);
		return(-1);
	}
	lseek(infile1, 0, SEEK_SET);
	while((nread = read(infile1, buf, BUF_SIZE)) > 0) {
		// close when cannot write
		if(write(outfile, buf, nread) < nread) {
			close(infile1);
			close(outfile);
			return (-3);
		}
	}
	// seek end in outfile to attach infile2
	lseek(outfile, -1, SEEK_END);
	lseek(infile2, 0, SEEK_SET);
	while((nread = read(infile2, buf, BUF_SIZE)) > 0) {
		// close when cannot write
		if(write(outfile, buf, nread) < nread) {
			close(infile2);
			close(outfile);
			return (-3);
		}
	}
	printf("New \"test.out\" has been created.\n");


	// read outfile BUF_SIZE bytes
	// seek set offset to read
	if((outfile = open("test.out", O_RDONLY)) == -1) {
		close(outfile);
		return(-1);
	}
	
	printf("-----test.out's data-----\n");

	while((nread = read(outfile, buf, BUF_SIZE)) > 0 ) {
		fputs(buf, stdout);
	}
	printf("\n-----------------------\n");


	close(infile1);
	close(infile2);
	close(outfile);

	if(nread == -1)
		return(-4);		// error at last read
	else
		return(0);
}

int main() {
	printf("Make New file(test.out) \ncombined \"test1.in file\" and \"test2.in file\"\n");
	combinefile("test1.in", "test2.in");
	return 0;
}

