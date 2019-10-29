/*************************************
실습 6-3

스레드 실행중지, 재시작

여러 스레드가 종료하기를 기다리려면 WaitForSingleObject() 함수를 스레드 개수만큼호출해야 함. 
때문에, WaitForMultipleObjects() 함수를 사용하면 호출 한번으로 끝낼 수 있음. 
MAXIMUM_WAIT_OBJECTS = 60
**************************************/

#include <windows.h>
#include <stdio.h>

int sum = 0;

DWORD WINAPI MyThread(LPVOID arg)
{
    int num = (int)arg;
    for (int i = 1; i <= num; i++) sum += i;
    return 0;
}

int main(int argc, char *argv[])
{
    int num = 100;
	HANDLE hThread[2];
	hThread[0] = CreateThread(NULL, 0, MyThread, (LPVOID)num, CREATE_SUSPENDED, NULL);	// CREATE_SUSPENDED: 중지된 상태로 생성
	hThread[1] = CreateThread(NULL, 0, MyThread, (LPVOID)num, CREATE_SUSPENDED, NULL);
    if (hThread[0] == NULL) return 1;
	if (hThread[1] == NULL) return 1;

    printf("스레드 실행 전. 계산 결과 = %d\n", sum);
    ResumeThread(hThread[0]);					// 스레드 재실행
	ResumeThread(hThread[1]);
    //WaitForSingleObject(hThread, INFINITE);
	WaitForMultipleObjects(2, hThread, true, INFINITE);
    printf("스레드 실행 후. 계산 결과 = %d\n", sum);
    CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);

    return 0;
}
