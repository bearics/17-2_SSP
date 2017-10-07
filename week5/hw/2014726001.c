#include <stdio.h>
#include <stdlib.h>

void print_arr(int** arr, int x, int y);

int main(void)
{
	int x=0, y=0;
	int** arr;

	printf("size of matrix : ");
	scanf("%d %d", &y, &x);

	// 입력 받은 수 만큼 동적 배열 생성 
	arr = (int**)malloc(sizeof(int*)*y);
	for(int i=0; i<y; i++) {
		arr[i] = (int*)malloc(sizeof(int)*x);
	}

	// 배열에 1로 초기화
	for(int i=0; i<y; i++){
		for(int j=0; j<x; j++){
			arr[i][j] = 1;
		}
	}
	while(1){
		int row=0, col=0;
		print_arr(arr, x, y);
		printf("input i, j : ");
		scanf("%d %d", &row, &col);
		if(row < 0 || col < 0 || row >= y || col >= x ) {
			puts("Bye bye");	// 입력 값이 범위를 벗어나면 메모리 해제후 프로그램 종료
			for(int i=0; i<y; i++)
				free(arr[i]);
			free(arr);
			return 0;
		}
		// 입력 받은 해당 정수 증가 및 2보다 크면 -2로 초기화
		if(arr[row][col] != 2)
			arr[row][col]++;
		else
			arr[row][col] = -2;
	}
}

void print_arr(int** arr, int x, int y)
{	// 배열을 출력해주는 함수
	for(int i=0; i<y; i++){
		for(int j=0; j<x; j++){
			printf("%+3d", arr[i][j]);
		}
		puts("");
	}
}