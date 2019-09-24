/********************************
실습 3-1 

바이트 정렬 함수	-> 호스트와 네트워크의 주소저장방식이 다를 수 있기 때문에 적절히 수정해주어야함.
					-> ex. 호스트(LE -> BE) -> 네트워크(BE -> LE) -> 호스트

빅엔디안:	주소를 순서대로 읽어옴
리틀엔디안:	주소를 반대부터 읽어옴
라우터:		서로 다른 네트워크를 연결해주는 장치
*********************************/


#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	if(s == SOCKET_ERROR)
		return 1;

	u_short x1 = 0x1234;
	u_long  y1 = 0x12345678;
	u_short x2, x3;
	u_long  y2, y3;

	// 호스트 바이트 -> 네트워크 바이트
	printf("[호스트 바이트 -> 네트워크 바이트]\n");
	WSAHtons(s, x1, &x2);
	printf("0x%x -> 0x%x\n", x1, x2);
	WSAHtonl(s, y1, &y2);
	printf("0x%x -> 0x%x\n", y1, y2);

	// 네트워크 바이트 -> 호스트 바이트
	printf("\n[네트워크 바이트 -> 호스트 바이트]\n");
	WSANtohs(s, x2, &x3);
	printf("0x%x -> 0x%x\n", x2, x3);
	WSANtohl(s, y2, &y3);
	printf("0x%x -> 0x%x\n", y2, y3);

	// 잘못된 사용 예
	printf("\n[부적절한 사용 예]\n");
	WSAHtonl(s, x1, &y3);	// short형을 long형 함수로 이용
	printf("0x%x -> 0x%x\n", x1, y3);

	WSACleanup();
	return 0;
}