#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
	int fd;
	ssize_t nread;
	char buf[1024];
	int i=0;
	fd = open("data", O_RDONLY);
	nread = read(fd, buf, 1024);
	for(i=0;i<(int)nread;i++) putc(buf[i], stdout);
	close(fd);
	return 0;
}


