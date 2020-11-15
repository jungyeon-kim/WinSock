/**********************************************
�ǽ� 6-1 

�ֽ�����(�θ𽺷���)�� ������ 2�� ����� ����
***********************************************/

#include <windows.h>
#include <stdio.h>

struct Point3D
{
    int x, y, z;
};

DWORD WINAPI MyThread(LPVOID arg)
{
    Point3D *pt = (Point3D *)arg;
    while (1) {
        printf("Running MyThread() %d : %d, %d, %d\n",
            GetCurrentThreadId(), pt->x, pt->y, pt->z);
        Sleep(1000);
    }
    return 0;
}

int main(int argc, char *argv[])
{
	Point3D pt[5]{};

	for (int i = 0; i < 5; ++i)
	{
		pt[i] = { 10 * i, 20 * i, 30 * i };
		HANDLE hThread = CreateThread(NULL, 0, MyThread, &pt[i], 0, NULL);
		if (hThread == NULL) return 1;
		CloseHandle(hThread);
	}

    //// ù ��° ������ ����
    //Point3D pt1 = { 10, 20, 30 };
    //HANDLE hThread1 = CreateThread(NULL, 0, MyThread, &pt1, 0, NULL);
    //if (hThread1 == NULL) return 1;
    //CloseHandle(hThread1);

    //// �� ��° ������ ����
    //Point3D pt2 = { 40, 50, 60 };
    //HANDLE hThread2 = CreateThread(NULL, 0, MyThread, &pt2, 0, NULL);
    //if (hThread2 == NULL) return 1;
    //CloseHandle(hThread2);

    while (1) {
        printf("Running main() %d\n", GetCurrentThreadId());
        Sleep(1000);
    }

    return 0;
}
