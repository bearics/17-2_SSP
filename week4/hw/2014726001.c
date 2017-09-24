#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
	pid_t par_pid = (int)getpid(); // 부모내에서 프로세스 식별 번호 저장
	pid_t *chi_pid = (pid_t*)malloc(sizeof(pid_t)*atoi(argv[1])); // 부모내에서 프로세스 식별 번호 저장
	int status;
	int count=0;
	int i;
	for(i=0; i<atoi(argv[1]); i++) { // 입력한 개수 만큼 자식 프로세스 생성
		chi_pid[i] = fork();	// 자식 프로세스 생성하고 chi_pid[i]에 pid 저장 
		if(chi_pid[i] == 0) break;	// 자시프로세스는 for구문 탈출 
	}
	if(i == atoi(argv[1])) i--;	// for구문을 나오면 i가 증가해 있으므로 감소
	
	if(chi_pid[i] != 0) { // 부모 프로세스만 실행
		while(count < atoi(argv[1])) { // 해당 개수의 자식 프로세스가 모두 죽을 때 까지 실행 
			for(int j=0; j<atoi(argv[1]); j++) { // 해당 자식 프로세스가 죽었는지 확인 
				if(waitpid(chi_pid[j], &status, WNOHANG) == chi_pid[j]) {
					// 죽은 자식 프로세스 정보를 출력
					printf("[%d] Parent : %d / Child : %d\n", j, (int)par_pid, (int)chi_pid[j]);
					count++;
					break;
				}
			}
		}
		printf("Parent %d is running\n", par_pid); // Parent 프로세스 아이디 출력
	}


	return 0;
}