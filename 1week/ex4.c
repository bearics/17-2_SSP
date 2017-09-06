#include <unistd.h>
#include <fcntl.h>

#define BUF_SIZE 512
#define PERM 0644

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

	while((nread = read(infile, buf, BUF_SIZE)) > 0 ) {
		if(write(outfile, buf, nread) < nread ) {
			close(infile);
			close(outfile);
			return(-3);
		}
	}
	close(infile);
	close(outfile);
	if(nread == -1)
		return(-4);
	else
		return(0);
}

int main() {
	copyfile("test.in", "test.out");
	return 0;
}

