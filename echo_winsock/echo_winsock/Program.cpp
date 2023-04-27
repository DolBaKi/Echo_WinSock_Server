#include "common.h"
#define PORT_NUM 10200
#define BLOG_SIZE 5
#define MAX_MSG_LEN 256
SOCKET SetTCPServer(short pnum, int blog);//대기 소켓 설정
void AcceptLoop(SOCKET sock);//Accept Loop
void DoIt(SOCKET dosock); //송수신

int main()
{
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);//윈속 초기화	
	SOCKET sock = SetTCPServer(PORT_NUM, BLOG_SIZE);//대기 소켓 설정
	if (sock == -1)
	{
		perror("대기 소켓 오류");
		WSACleanup();
		return 0;
	}
	AcceptLoop(sock);//Accept Loop
	WSACleanup();//윈속 해제화
	return 0;
}

SOCKET SetTCPServer(short pnum, int blog)
{
	SOCKET sock;
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//소켓 생성
	if (sock == -1) { return -1; }

	SOCKADDR_IN servaddr = { 0 };//소켓 주소
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr = GetDefaultMyIP();
	servaddr.sin_port = htons(PORT_NUM);

	int re = 0;
	//소켓 주소와 네트워크 인터페이스 결합
	re = bind(sock, (struct sockaddr*)&servaddr, sizeof(servaddr));
	if (re == -1)
	{
		return -1;
	}
	re = listen(sock, blog);//백 로그 큐 설정
	if (re == -1)
	{
		return -1;
	}
	printf("%s:%d Setup\n", inet_ntoa(servaddr.sin_addr), pnum);
	return sock;
}

void DoIt(SOCKET dosock)
{
	char msg[MAX_MSG_LEN] = "";
	while (recv(dosock, msg, sizeof(msg), 0) > 0)
	{
		printf("Received Packet : %s\n", msg);
		send(dosock, msg, sizeof(msg), 0);
		printf("Send Packet With Name : %s\n", msg);
	}
	closesocket(dosock);
}

void AcceptLoop(SOCKET sock)
{
	SOCKET dosock;
	SOCKADDR_IN cliaddr = { 0 };
	int len = sizeof(cliaddr);
	while (1)
	{
		dosock = accept(sock, (SOCKADDR*)&cliaddr, &len);
		if (dosock == -1)
		{
			perror("accept 실패!");
			break;
		}
		printf("%s:%d의 연결 요청 수락\n",inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));
		DoIt(dosock);
	}
	closesocket(dosock);
}