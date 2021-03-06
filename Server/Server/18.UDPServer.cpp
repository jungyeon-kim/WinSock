/*****************************************
실습 7-1 

UDP Server

비연결형 프로토콜
오류제어, 흐름제어 미지원 -> 신뢰성 없는 데이터 전송

통신을 위한 소켓 하나만 존재
일대다 통신 가능. -> 멀티스레딩 기법 불필요
연결설정을 하지않기 때문에 TCP와 달리 listen(), connect(), accept() 불필요
send(), recv()는 주소를 지정할 수 없음. -> sendto(), recvfrom() 사용

TCP에서 connect()는 IP와 Port번호를 할당받고, 서버에 연결요청을 하지만 UDP는 첫번째만 수행한다.
connect()를 명시적으로 호출하여 주소의 복사과정을 생략해 퍼포먼스를 높일 수 있다. -> 클라이언트는 send(), recv() 사용
******************************************/

#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

#define SERVERPORT 9000	// server port 설정
#define BUFSIZE    512

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
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);		// udp 소켓 생성 
    if (sock == INVALID_SOCKET) err_quit("socket()");

    // bind()
    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);		// 모든 ip로부터 수신
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));	// udp 서비스 완료
    if (retval == SOCKET_ERROR) err_quit("bind()");

    // 데이터 통신에 사용할 변수
    SOCKADDR_IN clientaddr;
    int addrlen;
    char buf[BUFSIZE + 1];

    // 클라이언트와 데이터 통신
    while (1) {
        // 데이터 받기
        addrlen = sizeof(clientaddr);
        retval = recvfrom(sock, buf, BUFSIZE, 0,
            (SOCKADDR *)&clientaddr, &addrlen);
        if (retval == SOCKET_ERROR) {
            err_display("recvfrom()");
            continue;
        }

        // 받은 데이터 출력
        buf[retval] = '\0';
        printf("[UDP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
            ntohs(clientaddr.sin_port), buf);

        // 데이터 보내기
        retval = sendto(sock, buf, retval, 0,
            (SOCKADDR *)&clientaddr, sizeof(clientaddr));
        if (retval == SOCKET_ERROR) {
            err_display("sendto()");
            continue;
        }
    }

    // closesocket()
    closesocket(sock);

    // 윈속 종료
    WSACleanup();
    return 0;
}
