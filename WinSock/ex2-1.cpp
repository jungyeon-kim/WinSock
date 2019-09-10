#pragma comment(lib, "ws2_32")
#include <winsock2.h>

// 소켓 함수 오류 출력 후 종료
void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

int main(int argc, char *argv[])
{
	// 윈속 초기화
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)	// MAKEWORD(2,2)버전이 아니면 프로그램 종료
		return 1;
	MessageBox(NULL, "윈속 초기화 성공", "알림", MB_OK);

	// socket()
	SOCKET udp_sock = socket(AF_INET6, SOCK_DGRAM, 0);
	if(udp_sock == INVALID_SOCKET) err_quit("socket()");
	MessageBox(NULL, "UDP 소켓(IPv6) 생성 성공", "알림", MB_OK);

	// closesocket()
	closesocket(udp_sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}