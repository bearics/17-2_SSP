#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAXLINE 1000
#define NAME_LEN 20
char *EXIT_STRING = "exit";
char *next_STRING = "next";
char *FIN_STRING = "fin to send";
char *FAIL_STRING = "Faild to find file in char_server \n";

//소켓 생성 및 서버 연결 생성된 소켓 리턴
int tcp_connect(int af, char *servip, unsigned short port);
void errquit(char *mesg) { perror(mesg); exit(1); }

int main(int argc, char *argv[]) {
 //	이름 + 메세지를 위한 버퍼, bufall에서 메세지 부분의 포인터
	char bufall[MAXLINE+NAME_LEN], *bufmsg;
	char buff[1024];
	int make=0;
//최대 소켓 디스크립터, 소켓, 이름의길이
	int maxfdp1,s,namelen;
	fd_set read_fds;

	if(argc != 4) {
		printf("사용법 : %s server_ip port name \n");
		exit(0);
	}

	sprintf(bufall, "[%s] :", argv[3]); //bufall의 앞부분에 이름을 저장
	namelen = strlen(bufall);
	bufmsg = bufall + namelen;//메세지 시작 부분 지정
	//호스트에 접속 IP주소, 포트번호 (리턴:감지할소켓수)
	s = tcp_connect(AF_INET, argv[1], atoi(argv[2]));
	if(s == -1)
		errquit("tcp_connect fail");
	
	puts("서버에 접속 되었습니다");
	maxfdp1 = s + 1;
	FD_ZERO(&read_fds); //소켓에 메세지를 받기 위해 모든 비트를 삭제 해둠

	while (1) {
		FD_SET(0, &read_fds); //모든 비트를 0으로 세팅
		FD_SET(s, &read_fds); //s로 세팅?
							  //소켓의 I/O 감지
		if (select(maxfdp1, &read_fds, NULL, NULL, NULL) < 0)
			errquit("select fail");
		if (FD_ISSET(s, &read_fds)) {
			
		}

		if (FD_ISSET(0, &read_fds)) {
			int fd;
			bzero(bufmsg, 1020);
			bzero(buff, 1024);
			//키보드 입력
			if (fgets(bufmsg, MAXLINE, stdin)) {
				bufmsg[strlen(bufmsg)-1]=0;
				printf("Sending file(%s)\n", bufmsg);
				if ( 0 > ( fd = open( bufmsg, O_RDONLY)))
				{
					printf("cannot open file\n");
					continue;
				}
				send(s, bufmsg, strlen(bufmsg), 0);
				recv(s, buff, MAXLINE, 0);
				int i = 0;
				while( strstr(buff, next_STRING) == NULL || i>3)
					sleep(1);

				while( (read(fd, buff, 1024) > 0) )
				{
					buff[strlen(buff)]=0x00;
					send(s, buff, strlen(buff), 0);
					bzero(buff, 1024);
					recv(s, buff, MAXLINE, 0);
				}
				send(s, FIN_STRING, strlen(FIN_STRING), 0);
				close(fd);
				printf("Fin to send file(%s)\n", bufmsg);
			}
		}
	}
}

int tcp_connect(int af, char *servip, unsigned short port) {
	struct sockaddr_in servaddr;
	int s;
	//소켓 생성
	if((s = socket(af,SOCK_STREAM,0)) < 0)
		return -1;

	//채팅 서버의 소켓 주소 구조체 servaddr 초기화
	bzero((char *)&servaddr, sizeof(servaddr));
	servaddr.sin_family = af;
	inet_pton(AF_INET, servip, &servaddr.sin_addr);
	servaddr.sin_port = htons(port);

	//연결 요청
	if(connect(s, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
		return -1;
	return s;
}
