/*****************************************************
�ǽ� 5�� 2��°


���� ���� ������ ���� - Ŭ���̾�Ʈ ���
*******************************************************/


#define _CRT_SECURE_NO_WARNINGS         // �ֽ� VC++ ������ �� ��� ����
#define _WINSOCK_DEPRECATED_NO_WARNINGS // �ֽ� VC++ ������ �� ��� ����
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERIP   "127.0.0.1"	// ������ ������ IP, ���� PC.
#define SERVERPORT 9000			// ������ ������ ��Ʈ
#define BUFSIZE    50			// 50����Ʈ ���۸� ����� �����͸� ����

// ���� �Լ� ���� ��� �� ����
void err_quit(char *msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
    LocalFree(lpMsgBuf);
    exit(1);
}

// ���� �Լ� ���� ���
void err_display(char *msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    printf("[%s] %s", msg, (char *)lpMsgBuf);
    LocalFree(lpMsgBuf);
}

int main(int argc, char *argv[])
{
    int retval;

    // ���� �ʱ�ȭ
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    // socket()
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) err_quit("socket()");

    // connect()
    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("connect()");

    // ������ ��ſ� ����� ����
    char buf[BUFSIZE];				// BUFSIZE ũ���� ���۸� ����
    char *testdata[] = {			// ������ ���ڿ� �����͸� ����
        "�ȳ��ϼ���",
        "�ѱ����������б� ���Ӱ��а�",
        "5�� �ι�° ����. �������� ������ ���۹��",
        "Ȯ���غ�����",
    };
    int len;				// ���ڿ� ���̸� ����� ����� ����

    // ������ ������ ���
    for (int i = 0; i < 4; i++) {
        // ������ �Է�(�ùķ��̼�)
        len = strlen(testdata[i]);		// ���ڿ� �����͸� ���ۿ� �����ϰ� �� ���� \n�� �߰�
        strncpy(buf, testdata[i], len);
        buf[len++] = '\n';

        // ������ ������
        retval = send(sock, buf, len, 0);		// len ũ�⸸ŭ �����͸� ����. len�� ���ڿ��� \n�� ���� �������� ���̸� ��Ÿ��
        if (retval == SOCKET_ERROR) {
            err_display("send()");
            break;
        }
        printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�.\n", retval);
    }

    // closesocket()
    closesocket(sock);

    // ���� ����
    WSACleanup();
    return 0;
}