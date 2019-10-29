/**************************************
실습 예제 3-2

IP 주소 변환 함수

컴퓨터는 이진 데이터만 해석 가능
인터넷 주소(문자열) -> 이진 데이터 (네트워크 바이트) 로 변경
혹은 이진 데이터 (네트워크 바이트) -> 인터넷 주소(문자열)로 변경할 필요가 있음
***************************************/

#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    /*----------------*/
    /* IPv4 변환 연습 */
    /*----------------*/
    // 원래의 IPv4 주소 출력
    char *ipv4test = "147.46.114.70";
    printf("IPv4 주소(변환 전) = %s\n", ipv4test);

    // inet_addr() 함수 연습
    printf("IPv4 주소(변환 후) = 0x%x\n", inet_addr(ipv4test));	// 16진수지만 OS가 자체적으로 2진수로 변환

    // inet_ntoa() 함수 연습
    IN_ADDR ipv4num;
    ipv4num.s_addr = inet_addr(ipv4test);	// 소켓주소 구조체에 변환된 주소를 대입
    printf("IPv4 주소(다시 변환 후) = %s\n", inet_ntoa(ipv4num));

    printf("\n");

    /*----------------*/
    /* IPv6 변환 연습 */
    /*----------------*/
    // 원래의 IPv6 주소 출력
    char *ipv6test = "2001:0230:abcd:ffab:0023:eb00:ffff:1111";
    printf("IPv6 주소(변환 전) = %s\n", ipv6test);

    // WSAStringToAddress() 함수 연습
    SOCKADDR_IN6 ipv6num;
    int addrlen = sizeof(ipv6num);
    WSAStringToAddress(ipv6test, AF_INET6, NULL,
        (SOCKADDR *)&ipv6num, &addrlen);
    printf("IPv6 주소(변환 후) = 0x");
    for (int i = 0; i < 16; i++)
        printf("%02x", ipv6num.sin6_addr.u.Byte[i]);
    printf("\n");

    // WSAAddressToString() 함수 연습
    char ipaddr[50];
    DWORD ipaddrlen = sizeof(ipaddr);
    WSAAddressToString((SOCKADDR *)&ipv6num, sizeof(ipv6num),
        NULL, ipaddr, &ipaddrlen);
    printf("IPv6 주소(다시 변환 후) = %s\n", ipaddr);

    WSACleanup();
    return 0;
}
