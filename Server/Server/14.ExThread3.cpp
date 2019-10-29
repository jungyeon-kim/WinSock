/*************************************
�ǽ� 6-3

������ ��������, �����

���� �����尡 �����ϱ⸦ ��ٸ����� WaitForSingleObject() �Լ��� ������ ������ŭȣ���ؾ� ��. 
������, WaitForMultipleObjects() �Լ��� ����ϸ� ȣ�� �ѹ����� ���� �� ����. 
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
	hThread[0] = CreateThread(NULL, 0, MyThread, (LPVOID)num, CREATE_SUSPENDED, NULL);	// CREATE_SUSPENDED: ������ ���·� ����
	hThread[1] = CreateThread(NULL, 0, MyThread, (LPVOID)num, CREATE_SUSPENDED, NULL);
    if (hThread[0] == NULL) return 1;
	if (hThread[1] == NULL) return 1;

    printf("������ ���� ��. ��� ��� = %d\n", sum);
    ResumeThread(hThread[0]);					// ������ �����
	ResumeThread(hThread[1]);
    //WaitForSingleObject(hThread, INFINITE);
	WaitForMultipleObjects(2, hThread, true, INFINITE);
    printf("������ ���� ��. ��� ��� = %d\n", sum);
    CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);

    return 0;
}
