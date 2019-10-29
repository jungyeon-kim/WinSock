/*************************************
실습 6-5

임계영역(Critical Section) 실습

공유 자원에 대해 오직 한 스레드의 접근만 허용 -> 순서가 미보장
**************************************/

#include <windows.h>
#include <stdio.h>

#define MAXCNT 10
int g_count = 0;
CRITICAL_SECTION cs;						// 임계영역 구조체 변수

DWORD WINAPI MyThread1(LPVOID arg)
{
    for (int i = 0; i < MAXCNT; ++i) {
        EnterCriticalSection(&cs);			// 공유자원(g_count)을 사용하는 스레드가 없다면, 리턴 후 다음 줄로 진행
											// 반대로 있다면, 리턴하지 못하고 해당 스레드는 대기상태로 변경
		printf("thread 1 \n");
        g_count += 2;
        LeaveCriticalSection(&cs);			// 공유자원 사용(g_count += 2;)을 마치면 임계영역을 떠남
    }

    return 0;
}

DWORD WINAPI MyThread2(LPVOID arg)
{
	for (int i = 0; i < MAXCNT; ++i) {
		EnterCriticalSection(&cs);
		printf("thread 2 \n");
		g_count -= 2;
		LeaveCriticalSection(&cs);
	}

    return 0;
}

int main(int argc, char *argv[])
{
    // 임계 영역 초기화
    InitializeCriticalSection(&cs);
    // 두 개의 스레드 생성
    HANDLE hThread[2];
    hThread[0] = CreateThread(NULL, 0, MyThread1, NULL, 0, NULL);
    hThread[1] = CreateThread(NULL, 0, MyThread2, NULL, 0, NULL);
    // 두 개의 스레드 종료 대기
    WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
    // 임계 영역 삭제
    DeleteCriticalSection(&cs);
    // 결과 출력
    printf("g_count = %d\n", g_count);
    return 0;
}
