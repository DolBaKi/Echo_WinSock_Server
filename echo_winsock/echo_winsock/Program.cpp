#include "common.h"
#define PORT_NUM 10200
#define BLOG_SIZE 5
#define MAX_MSG_LEN 256
SOCKET SetTCPServer(short pnum, int blog);//��� ���� ����
void AcceptLoop(SOCKET sock);//Accept Loop
void DoIt(SOCKET dosock); //�ۼ���

int main()
{
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);//���� �ʱ�ȭ	
	SOCKET sock = SetTCPServer(PORT_NUM, BLOG_SIZE);//��� ���� ����
	if (sock == -1)
	{
		perror("��� ���� ����");
		WSACleanup();
		return 0;
	}
	AcceptLoop(sock);//Accept Loop
	WSACleanup();//���� ����ȭ
	return 0;
}

SOCKET SetTCPServer(short pnum, int blog)
{
	SOCKET sock;
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//���� ����
	if (sock == -1) { return -1; }

	SOCKADDR_IN servaddr = { 0 };//���� �ּ�
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr = GetDefaultMyIP();
	servaddr.sin_port = htons(PORT_NUM);

	int re = 0;
	//���� �ּҿ� ��Ʈ��ũ �������̽� ����
	re = bind(sock, (struct sockaddr*)&servaddr, sizeof(servaddr));
	if (re == -1)
	{
		return -1;
	}
	re = listen(sock, blog);//�� �α� ť ����
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
			perror("accept ����!");
			break;
		}
		printf("%s:%d�� ���� ��û ����\n",inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));
		DoIt(dosock);
	}
	closesocket(dosock);
}