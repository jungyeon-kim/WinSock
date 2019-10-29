/*************************************
실습 6-6

CS Event 실습

사건 발생(어떤 스레드의 작업 완료)을 알려(신호) 대기상태(비신호)인 다른 스레드를 깨움 -> 이벤트간 순서 보장
자동 리셋 이벤트:		이벤트가 신호 상태가 되면, 대기상태 스레드 "하나"를 깨우고 비신호 상태가 됨.
수동 리셋 이벤트:		이벤트가 신호 상태가 되면, 대기상태 스레드 "모두"를 깨우고 비신호 상태가 됨.
**************************************/

#include <windows.h>
#include <stdio.h>

#define BUFSIZE 10

HANDLE hReadEvent;
HANDLE hWriteEvent;
int buf[BUFSIZE];

DWORD WINAPI WriteThread(LPVOID arg)
{
    DWORD retval;

    for (int k = 1; k <= 500; k++) {
        // 읽기 완료 대기
        retval = WaitForSingleObject(hReadEvent, INFINITE);
        if (retval != WAIT_OBJECT_0) break;

        // 공유 버퍼에 데이터 저장
		printf("Thread %4d: \n", GetCurrentThreadId());
        for (int i = 0; i < BUFSIZE; i++)
            buf[i] = k;

        // 쓰기 완료 알림
        SetEvent(hWriteEvent);
    }

    return 0;
}

DWORD WINAPI ReadThread(LPVOID arg)
{
    DWORD retval;

    while (1) {
        // 쓰기 완료 대기
        retval = WaitForSingleObject(hWriteEvent, INFINITE);
        if (retval != WAIT_OBJECT_0) break;

        // 읽은 데이터 출력
        printf("Thread %4d: ", GetCurrentThreadId());
        for (int i = 0; i < BUFSIZE; i++)
            printf("%3d ", buf[i]);
        printf("\n");

        // 버퍼 초기화
        ZeroMemory(buf, sizeof(buf));

        // 읽기 완료 알림
        SetEvent(hReadEvent);
    }

    return 0;
}

int main(int argc, char *argv[])
{
    // 두 개의 자동 리셋 이벤트 생성(각각 비신호, 신호 상태)
    hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);	// NULL, 자동리셋, 비신호, NULL
    if (hWriteEvent == NULL) return 1;
    hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);		// NULL, 자동리셋, 신호, NULL
    if (hReadEvent == NULL) return 1;

    // 세 개의 스레드 생성
    HANDLE hThread[3];
    hThread[0] = CreateThread(NULL, 0, WriteThread, NULL, 0, NULL);
    hThread[1] = CreateThread(NULL, 0, ReadThread, NULL, 0, NULL);
    hThread[2] = CreateThread(NULL, 0, ReadThread, NULL, 0, NULL);

    // 세 개의 스레드 종료 대기
    WaitForMultipleObjects(3, hThread, TRUE, INFINITE);

    // 이벤트 제거
    CloseHandle(hWriteEvent);
    CloseHandle(hReadEvent);

    return 0;
}
