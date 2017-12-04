#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <syslog.h>
#include <signal.h>


#define MAXLINE 511
#define MAX_SOCK 1024

char *EXIT_STRING = "exit"; //클라리언트 종료 요청 문자열
char *FIN_STRING = "fin to send";
char *next_STRING = "next";
char *START_STRING = "Connected to char_server \n";
char *FAIL_STRING = "Faild to find file in char_server \n";

//클라이언트 환영 메세지
int maxfdp1;
int num_chat = 0;
int clisock_list[MAX_SOCK]; //채팅 참가자 소켓 번호 목록
int listen_sock; //서버의 리슨 소켓

				 //새로운 채팅 참가자 처리
void addClient(int s, struct sockaddr_in *newcliaddr);
int getmax();
void removeClient(int s);
int tcp_listen(int host, int port, int backlog);
void errquit(char *mesg) { perror(mesg); exit(1); }

// socket > bind > listen >accept > read/write > close

int main(int argc, char *argv[]) {
	struct sockaddr_in cliaddr;
	char buf[MAXLINE + 1];
	int i, j, nbyte, accp_sock, addrlen = sizeof(struct sockaddr_in);
	fd_set read_fds; //읽기를 감지할 소켓번호 구조체
	pid_t pid;
	struct sigaction sact;

	if (argc != 2) {
		printf("사용법 : %s port\n", argv[0]);
		exit(0);
	}
	/*
	INADDR_ANY : 다수의접속을가질경우IP주소를지정.
	atoi(argv[1]) : 포트번호
	5 : 최대연결갯수
	*/
	listen_sock = tcp_listen(INADDR_ANY, atoi(argv[1]), 5); //소켓번호를 리턴받는다.

	while (1) {
		FD_ZERO(&read_fds);//읽기를 감지할 소켓의 모든 비트를 지움
		FD_SET(listen_sock, &read_fds); //읽기전용 소켓(read_fds) 중 생성된 소켓에 해당하는 비트값를 1로 지정
										//num_chat = 0 초기치
		for (i = 0; i < num_chat; i++) //참가중인 모든 클라리언트 소켓을 선택
			FD_SET(clisock_list[i], &read_fds); //배열에

		maxfdp1 = getmax() + 1; //생성된 소켓양에 대해서 최대 소켓보다 크면

		puts("wait for client");

		//seletct로 클라이언트를 선택한다.
		//변수명:감지할최대소켓개수+1,읽기I/O변화감지소켓, 쓰기감지소켓,예외상황감지소켓,시스템I/o변화시간
		if (select(maxfdp1, &read_fds, NULL, NULL, NULL) < 0)
			errquit("select fail");

		//읽은 소켓중 클라리언트에서 들어온 비트가 있으면 양수값을 리턴해준다.
		if (FD_ISSET(listen_sock, &read_fds)) {

			/*클라리언트에서 호스트로 보낸 채팅를 메세지수신
			listen_sock : 듣는소켓, cliaddr프로토콜주소,프로토콜크기)
			서버와 클라이언트 악수 기법 클라이언트가 종료되면 자동으로 연결 소켓이 닫힌다.
			*/
			accp_sock = accept(listen_sock, (struct sockaddr *)&cliaddr, &addrlen);

			if (accp_sock == -1)
				errquit("accept fail");
			//클라리언트 추가
			addClient(accp_sock, &cliaddr);

			//메시지를 모든 클라리언트에게 방송
			printf("%d번째 사용자 추가.\n", num_chat);
		}

		// 하나씩 받은게 있는지 확인하고, 모든 사용자에게 한 명씩  보내준다.
		int fd, k, l;
		int nbyte=0;
		char filename[1024];
		char buff[1024]; 	// store file data
		bzero(filename, 30);
		bzero(buff, 1024);
		
		for (i = 0; i<num_chat; i++) {
			if (FD_ISSET(clisock_list[i], &read_fds)) {
				nbyte = recv(clisock_list[i], filename, MAXLINE, 0);
				if (nbyte <= 0) {
					removeClient(i);
					continue;
				}

				if (strstr(filename, EXIT_STRING) != NULL) {
					removeClient(i);
					continue;
				}
				if ( 0 > ( fd = open( filename, O_WRONLY | O_CREAT , 0644)))
				{
					printf("cannot make file\n");
					send(clisock_list[i], next_STRING, strlen(next_STRING), 0);
					continue;
				}
				send(clisock_list[i], next_STRING, strlen(next_STRING), 0);
				while (recv(clisock_list[i], buf, MAXLINE, 0) > 0) {
					if(strstr(buf, FIN_STRING) != NULL )
					{
						printf("finish to save file(%s)\n", filename);
						break;
					}
					printf("recv[%s]\n", buf);
					write(fd, buf, strlen(buf));
					bzero(buf, 1024);
					send(clisock_list[i], next_STRING, strlen(next_STRING), 0);
				}

				close(fd); 
				for (j = 0; j<num_chat; j++)
					send(clisock_list[j], FIN_STRING, strlen(FIN_STRING), 0);	// send fin
				printf("%s\n", buf);
			}
		}
	}

	
return 0;

}





void addClient(int s, struct sockaddr_in *newcliaddr) {
	char buf[20];
	inet_ntop(AF_INET, &newcliaddr->sin_addr, buf, sizeof(buf));
	printf("new client: %s\n", buf);

	clisock_list[num_chat] = s;
	num_chat++;
}

int getmax() {
	int max = listen_sock;
	int i;
	for (i = 0; i < num_chat; i++)
		if (clisock_list[i] > max)
			max = clisock_list[i];
		return max;
	}


	void removeClient(int i) {
		close(clisock_list[i]);
		if (i != num_chat - 1) clisock_list[i] = clisock_list[num_chat - 1];
		num_chat--;
		printf("채팅 참가자 1명 탈퇴. 현재 참가자수 = %d\n", num_chat);
	}

	int tcp_listen(int host, int port, int backlog) {
		int sd;
		struct sockaddr_in servaddr;

	//소켓 생성
		sd = socket(AF_INET, SOCK_STREAM, 0);
		if (sd == -1) {
			perror("socket fail");
			exit(1);
		}
	//소켓생성에 다른 포트와 어드레스 설정
		bzero((char *)&servaddr, sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = htonl(host);
		servaddr.sin_port = htons(port);

	//소켓과 기타정보를 묶어준다.
		if (bind(sd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
			perror("bind fail");
			exit(1);
		}

	//소켓을 쳐다 본다.
		listen(sd, backlog);
		return sd;
	}
