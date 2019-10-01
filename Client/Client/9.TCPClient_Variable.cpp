/*****************************************************
실습 5장 2번째


가변 길이 데이터 전송 - 클라이언트 모듈
*******************************************************/


#define _CRT_SECURE_NO_WARNINGS         // 최신 VC++ 컴파일 시 경고 방지
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERIP   "127.0.0.1"	// 접속할 서버의 IP, 로컬 PC.
#define SERVERPORT 9000			// 접속할 서버의 포트
#define BUFSIZE    50			// 50바이트 버퍼를 사용해 데이터를 보냄

// 소켓 함수 오류 출력 후 종료
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

// 소켓 함수 오류 출력
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

    // 윈속 초기화
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

    // 데이터 통신에 사용할 변수
    char buf[BUFSIZE];				// BUFSIZE 크기인 버퍼를 선엄
    char *testdata[] = {			// 전송할 문자열 데이터를 선언
        "안녕하세요",
        "한국산업기술대학교 게임공학과",
        "5장 두번째 예제. 가변길이 데이터 전송방법",
        "확인해보세요",
    };
    int len;				// 문자열 길이를 계산한 결과를 담음

    // 서버와 데이터 통신
    for (int i = 0; i < 4; i++) {
        // 데이터 입력(시뮬레이션)
        len = strlen(testdata[i]);		// 문자열 데이터를 버퍼에 복사하고 맨 끝에 \n을 추가
        strncpy(buf, testdata[i], len);
        buf[len++] = '\n';

        // 데이터 보내기
        retval = send(sock, buf, len, 0);		// len 크기만큼 데이터를 보냄. len은 문자열에 \n을 붙인 데이터의 길이를 나타냄
        if (retval == SOCKET_ERROR) {
            err_display("send()");
            break;
        }
        printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);
    }

    // closesocket()
    closesocket(sock);

    // 윈속 종료
    WSACleanup();
    return 0;
}
