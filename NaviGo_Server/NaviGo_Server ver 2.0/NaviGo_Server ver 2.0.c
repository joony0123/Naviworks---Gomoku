#include <stdio.h>		
#include <winsock2.h>										// �����
#include <Windows.h>										// Ŀ�� �̵�
#include <conio.h>											// getch�Լ� (Ű�� �ϳ� �޴� �Լ�)
#include "turboc.h"
#include <string.h>
#include <wingdi.h>
#include <stdlib.h>
#include <mmsystem.h>
#include "sendreceive.h"


#pragma comment(lib,"winmm.lib")

#define ARR_SIZE_R 19
#define ARR_SIZE_C 19

int gg33 = 0;
int x, y;
int retry = 0;
int whiteCheck[100][100] = { 0 };
int blackCheck[100][100] = { 0 };
BOOL gameBreaker = FALSE;
BOOL gameReboot = TRUE;
int server = 1;
BOOL spaceSelect = FALSE;
int p1diceNum = 1;
int p2diceNum = 1;
int first= 1;
BOOL click = FALSE;
HANDLE hIn, hOut;
BOOL diffPlace = FALSE;

WSADATA wsaData;
SOCKET hServSock;
SOCKET hClntSock;
SOCKADDR_IN servAddr;
SOCKADDR_IN clntAddr;
int szClntAddr;
int strLen = 0;
int sendX;
int sendY;
int recX = 1;
int recY = 1;
int whiteCount = 0;
int blackCount = 0;
int posNum = 0; // �Ź� �ʱ�ȭ���־��
int tempCount = 0;
BOOL overlap = FALSE;

int msgID;
int error = 0;

int sendPos[5] = { 0 };
int recPos[5] = { 0 };

int servScore = 1; 
int clntScore = 1;
char servName[49] = "gh";
char clntName[50] = "gh";
int count = 0;
void printLose(void);
void printWin(void);
void setPan(void);
void gotoxy(int a, int b);
void MouseClick(int *x, int *y);
void CheckWhiteWins(void);
void CheckBlackWins(void);
void txtColor(int c);
void textbackground(int g);
void CursorVis(void);
void WhiteMsgBox(void);
void gameProcess(void);
void intro(void);
void gameStart(void);
void loading(void);
void BlackMsgBox(void);
void startScreen(void);
void Key_Down(char key);
void enterName(void);
void sendPosition(int a, int b);
void whiteRadarCheck(void);
void black33(void);
void blackExile3(void);
void whiteExile3(int a, int b);
void allBlackExile3(void);

typedef struct point
{
	int pX;
	int pY;
}point;

struct innocent3Club
{
	point pos[3];
};

struct temp3Club
{
	point pos[3];
};

struct innocent3Club innocent3[100] = { -1 };
struct temp3Club temp3[10] = { -1, };


void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}




//���η� �� ���� 0, 2, 4, ... 36 ¦�� (x)
//���η� �� ���� 0, 1, 2... 18 (y)
//����
int main(int argc, char **argv)
{
	memset(innocent3, -1, sizeof(struct innocent3Club) * 100);
	FlushConsoleInputBuffer(hIn);
	//=================================���� ����=========================================
	if (argc != 2)
	{
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	// Load Winsock 2.2 DLL
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup() error!");
	}

	// ���� ���� ����
	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET)
	{
		ErrorHandling("socket() error");
	}
	else
		puts("Socket Initialize...Success");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	// ���Ͽ� �ּ� �Ҵ�
	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("bind() error");
	}
	else
		puts("Socket Binding...Success");

	// ���� ��û ��� ����
	if (listen(hServSock, 5) == SOCKET_ERROR)
	{
		ErrorHandling("listen() error");
	}
	else
		puts("Listening to Client...Success");

	// ���� ��û ����
	szClntAddr = sizeof(clntAddr);
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
	if (hClntSock == INVALID_SOCKET)
	{
		ErrorHandling("accept() error");
	}
	else
		puts("Accepting Client...Success");

	//=================================���� ����=========================================
	system("cls");

	PlaySound(TEXT("Crazy Arcade Login Sound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);

	//Ŀ�� �����
	CursorVis();

	// ������
	startScreen();

	//���Ӽ��� ����
	if (server == 1)
	{
		//�ε�
		loading();

		//����  ����
		while (gameReboot)
		{
			gameStart();
		}
	}

	return 0;
} // ���� ��

//�ε�ȭ��
void loading(void)
{
	gotoxy(33, 8);
	txtColor(6);
	puts("�� Now Loading��\n\n\n\n\n\n\n\n\n\n                       COPYRIGHT 2016 Joon Kim at Naviworks��");
	Sleep(650);
	gotoxy(33, 8);
	txtColor(10);
	puts("�� Now Loading  �� \n\n\n\n\n\n\n\n\n\n                       COPYRIGHT 2016 Joon Kim at Naviworks��");
	Sleep(650);
	gotoxy(33, 8);
	txtColor(14);
	puts("�� Now Loading    �� \n\n\n\n\n\n\n\n\n\n                       COPYRIGHT 2016 Joon Kim at Naviworks��");
	Sleep(650);
	system("cls");
}

//���� ���� ���� ��
void gameStart(void)
{
	if (gameBreaker == FALSE && gameReboot == TRUE)
		enterName();

	gameBreaker = FALSE;
	gameReboot = FALSE;

	gotoxy(33, 8);
	txtColor(6);
	puts("�� Now Loading��\n\n\n\n\n\n                                   Please Wait....");
	Sleep(700);
	gotoxy(33, 8);
	txtColor(10);
	puts("�� Now Loading  �� \n\n\n\n\n\n                                   Please Wait....");
	Sleep(700);
	gotoxy(33, 8);
	txtColor(14);
	puts("�� Now Loading    �� \n\n\n\n\n\n                                   Please Wait....");
	Sleep(700);
	system("cls");


	//�ٵ��� ����
	setPan();
	//���� ����
	intro();

	//���� ����
	gameProcess();

	return;
}

//�̸� �Է�
void enterName(void)
{
	txtColor(14);
	gotoxy(20, 8);
	puts("����Ͻ� �̸��̳� �г����� �Է����ּ���~");
	gotoxy(20, 11);
	txtColor(15);
	printf("������ : ");
	gets(servName);
	//send name of the server 
	send(hClntSock, servName, sizeof(servName), 0);
	
	gotoxy(20, 15);
	puts("Waiting for other player....");
	// receive name of the client
	strLen = recv(hClntSock, clntName, sizeof(clntName) - 1, 0);
	clntName[strLen] = 0;
	
	while (1)
	{

		system("cls");
		txtColor(14);
		puts("\n\n   ���� �ֽŽ� �ζ� ��÷�⸦ ���� ���� ������ ���� ���� �̴ϴ�.\n   0���� 99�߿� �� ���� ���ڸ� ������ ���� �鵹�� ������ �� ���Դϴ�. ");
		txtColor(15);
		printf("\n\n   %s�� Enter�� ���� �ζ� ��÷��踦 �����ּ���~\n", servName);
		getch();
		randomize();
		p1diceNum = rand()%100;

		//�ζ� ������
		txtColor(10);
		gotoxy(33, 15);
		printf("�� ");
		txtColor(15);
		printf("Rolling.");
		Sleep(300);
		txtColor(11);
		gotoxy(33, 15);
		printf("�� ");
		txtColor(15);
		printf("Rolling . ");
		Sleep(300);
		txtColor(12);
		gotoxy(33, 15);
		printf("�� ");
		txtColor(15);
		printf("Rolling  .");
		Sleep(300);
		txtColor(13);
		gotoxy(33, 15);
		printf("�� ");
		txtColor(15);
		printf("Rolling   . ");
		Sleep(300);
		txtColor(14);
		gotoxy(33, 15);
		printf("�� ");
		txtColor(15);
		printf("Rolling    . ");
		Sleep(300);

		system("cls");
		txtColor(14);
		puts("\n\n   ���� �ֽŽ� �ζ� ��÷�⸦ ���� ���� ������ ���� ���� �̴ϴ�.\n   0���� 99�߿� �� ���� ���ڸ� ������ ���� �鵹�� ������ �� ���Դϴ�. ");

		txtColor(15);
		printf("\n   %s���� ���ڴ� ", servName);
		txtColor(14);
		printf("%d", p1diceNum);
		txtColor(15);
		printf("�Դϴ�\n");

		//server p1���� ������/ client ���� �ޱ�
		servScore = p1diceNum;
		send(hClntSock, (char*)&servScore, sizeof(servScore), 0);

		gotoxy(20, 15);
		puts("Waiting for other player....");

		recv(hClntSock, (char*)&clntScore, sizeof(clntScore), 0);


		system("cls");
		txtColor(14);
		puts("\n\n   ���� �ֽŽ� �ζ� ��÷�⸦ ���� ���� ������ ���� ���� �̴ϴ�.\n   0���� 99�߿� �� ���� ���ڸ� ������ ���� �鵹�� ������ �� ���Դϴ�. ");

		txtColor(15);
		printf("\n   %s���� ���ڴ� ", servName);
		txtColor(14);
		printf("%d", p1diceNum);
		txtColor(15);
		printf("�Դϴ�\n");


		txtColor(15);
		printf("\n   %s���� ���ڴ� ", clntName);
		txtColor(14);
		printf("%d", clntScore);
		txtColor(15);
		printf("�Դϴ�\n");

		Sleep(2000);

		if (servScore > clntScore)
		{
			printf("\n\n\n\n                               Nice~ %s���� �����Դϴ�!!", servName);
			first = 1;
			break;
		}
		else if (clntScore > servScore)
		{
			printf("\n\n\n\n                               %s���� �����Դϴ� �Ф�", clntName);
			first = 2;
			break;
		}
		if (servScore == clntScore)
		{
			printf("\n\n\n\n                                �����ϴ�. �ٽ� �����ּ���.");
			Sleep(2000);
		}
	}
	
	Sleep(3300);
	system("cls");
}

//���� ����
void intro(void)
{
	gotoxy(43, 2);
	txtColor(13);
	printf("��5���� ���� ���� �������� ");
	gotoxy(43, 3);
	printf("���γ� ����, �Ǵ� �밢�� �������� ");
	gotoxy(43, 4);
	printf("���ƾ� �¸��մϴ١�");
}

// �ٵ��� �����
void setPan(void)
{

	gotoxy(0, 0);
	txtColor(0 | 96);

	for (int i = 0; i <= ARR_SIZE_R; i++)
	{
		for (int j = 0; j < ARR_SIZE_C; j++)
		{
			if (i == 0)
			{
				if (j == 0)
					printf("��");
				else if (j == ARR_SIZE_C - 1)
					printf("��\n");
				else
					printf("��");
			}
			else if (i != 1 && i != ARR_SIZE_R)
			{
				if (j == 0)
					printf("��");
				else if (j == ARR_SIZE_C - 1)
					printf("��\n");
				else
					printf("��");
			}
			else if (i == ARR_SIZE_R)
			{
				if (j == 0)
					printf("��");
				else if (j == ARR_SIZE_C - 1)
					printf("��\n");
				else
					printf("��");
			}
		}
	}
} // �ٵ��� ����� ��


//���콺 Ŭ�� �Լ�``
void MouseClick(int *x, int *y)
{
		DWORD        dwNOER;
		INPUT_RECORD rec;

		hIn = GetStdHandle(STD_INPUT_HANDLE);
		hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleMode(hIn, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);

		while (TRUE)
		{
			ReadConsoleInput(hIn, &rec, 1, &dwNOER);
		

			if (rec.EventType == MOUSE_EVENT)
			{
				if (rec.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
				{
					*x = rec.Event.MouseEvent.dwMousePosition.X;
					*y = rec.Event.MouseEvent.dwMousePosition.Y;
					return;
				}
			}
		}
}

void gameProcess(void)
{
	
	// ���� ���������� ����
	PlaySound(TEXT("���� ���� - �ﱹ�� 7 Summer OST.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
	// check �迭 �ʱ�ȭ(�ߺ��Ǹ� �ȵǴ�)
	if (gameBreaker == FALSE && gameReboot == FALSE)
	{
		for (int i = 0; i <= 100; i++)
		{
			for (int j = 0; j <= 100; j++)
			{
				whiteCheck[i][j] = 0;
				blackCheck[i][j] = 0;
			}
		}
	}

	//������ �ߴܵ��� ���� ��� ��� ������
	while (gameBreaker == FALSE)
	{
		if (first == 1)
		{
			while (gameBreaker == FALSE)
			{
				 //���� �α�
				txtColor(15);
				gotoxy(43, 7);
				printf("%s���� �����Դϴ�", servName);

				 //�ٽýõ� ���� ���, �ƴ� �� ��ŵ
				if (retry == 1)
				{
					
					send(hClntSock, (char*)&retry, sizeof(retry), 0);
					retry = 0;
					gotoxy(0, 22);
					puts("Waiting for other player....");

					recv(hClntSock, (char*)&retry, sizeof(retry), 0);
					gotoxy(0, 22);
					if (retry != 1)
					{
						txtColor(15);
						puts("Connection Lost! Please Restart");
						gameBreaker = TRUE;
						gameReboot = FALSE;
						return;
					}
					else
					{
						txtColor(0);
						puts("Waiting for other player....");
						
					}
				}
				//	retry �ʱ�ȭ
				retry = 0;
				



				diffPlace = FALSE;

				while (diffPlace == FALSE)
				{
					//Ŭ��
					FlushConsoleInputBuffer(hIn);
					MouseClick(&x, &y);

					if (0 <= x && x <= 37 && 0 <= y && y <= 18)
					{
						if (x % 2 == 1)
							x = x - 1;
						if (whiteCheck[x][y] == 0 && blackCheck[x][y] == 0)
						{
							blackExile3();
							black33();
							
							//�浹 ��ǥ�� �����ֱ�
							sendPos[0] = x;
							sendPos[1] = y;
							send(hClntSock, (char*)&sendPos, sizeof(sendPos), 0);
							if (error == 1)
							{
								gg33 = 1;
								send(hClntSock, (char*)&gg33, sizeof(gg33), 0);
								printLose();
								msgID = MessageBoxA(NULL, "�ءء�  3-3 �ݼ��� �浹�� �����ϴ�!  �ءء�", "���� ��������!", MB_RETRYCANCEL);

							switch (msgID)
							{
							case IDRETRY:
								txtColor(0);

								retry = 1;
								gameBreaker = TRUE;
								gameReboot = TRUE;
								diffPlace = TRUE;
								system("cls");
								break;
							case IDCANCEL:
								gameBreaker = TRUE;
								gameReboot = FALSE;
								retry = 0;
								send(hClntSock, (char*)&retry, sizeof(retry), 0);
								exit(1);
								break;
							}
							}
							else
							{
								gg33 = 0;
								send(hClntSock, (char*)&gg33, sizeof(gg33), 0);
								diffPlace = TRUE;
								//check if White wins
								CheckBlackWins();
							}
						}

					}
				} //���� ���� �� �ΰ� ��

				if (retry == 0)
				{
					txtColor(15);
				gotoxy(43, 7);
				printf("%s���� �����Դϴ�", clntName);

				// �鵹 ��ǥ�� �ް� ����, ���
				recv(hClntSock, (char*)&recPos, sizeof(recPos), 0);
				x = recPos[0];
				y = recPos[1];
				whiteExile3(x, y);
				gotoxy(x, y);
				txtColor(15 | 96);
				printf("��");
				whiteCheck[x][y] = 1;
			//	check if White wins
				CheckWhiteWins();
				}
			
			} // blackturn while ��
		} // if first = 1 ��

		else if (first == 2)
		{
			while (gameBreaker == FALSE)
			{
				// Client ����
				txtColor(15);
				gotoxy(43, 7);
				printf("%s���� �����Դϴ�", clntName);

				//�ٽýõ��� ����, �ƴ� �� ��ŵ
				if (retry == 1)
				{
					
					send(hClntSock, (char*)&retry, sizeof(retry), 0);
					retry = 0;
					gotoxy(0, 22);
					puts("Waiting for other player....");

					recv(hClntSock, (char*)&retry, sizeof(retry), 0);
					gotoxy(0, 22);
					if (retry != 1)
					{
						txtColor(15);
						puts("Connection Lost! Please Restart");
						gameBreaker = TRUE;
						gameReboot = FALSE;
						return;
					}
					else
					{
						txtColor(0);
						puts("Waiting for other player....");
						

					}
				}
				//retry �ٽ� �ʱ�ȭ
				retry = 0;
			


				// �浹 ��ǥ�� �ް� ����, ���
				recv(hClntSock, (char*)&recPos, sizeof(recPos), 0);
				
				x = recPos[0];
				y = recPos[1]; 
				gotoxy(x, y);
				txtColor(0 | 96);
				printf("��");
				blackCheck[x][y] = 1;
				recv(hClntSock, (char*)&gg33, sizeof(gg33), 0);
				if (gg33 == 1)
				{
					printWin();
					msgID = MessageBoxA(NULL, "�١١�  �鵹�� �̰���ϴ�!  �١١�", "�����մϴ�!", MB_RETRYCANCEL);

				switch (msgID)
				{
				case IDRETRY:
					txtColor(0);

					retry = 1;
					gameBreaker = TRUE;
					gameReboot = TRUE;

					system("cls");
					break;
				case IDCANCEL:
					gameBreaker = TRUE;
					gameReboot = FALSE;
					retry = 0;
					send(hClntSock, (char*)&retry, sizeof(retry), 0);
					exit(1);
					break;
				}
				}
				//check if Black wins
				CheckBlackWins();

				if (retry == 0)
				{
					//���� ����
					txtColor(15);
					gotoxy(43, 7);
					printf("%s���� �����Դϴ�", servName);

					diffPlace = FALSE;
					while (diffPlace == FALSE)
					{
						FlushConsoleInputBuffer(hIn);
						MouseClick(&x, &y);

						if (0 <= x && x <= 37 && 0 <= y && y <= 18)
						{
							if (x % 2 == 1)
								x = x - 1;
							if (whiteCheck[x][y] == 0 && blackCheck[x][y] == 0)
							{
								gotoxy(x, y);
								txtColor(15 | 96);
								printf("��");
								whiteCheck[x][y] = 1;

								sendPos[0] = x;
								sendPos[1] = y;
								send(hClntSock, (char*)&sendPos, sizeof(sendPos), 0);
								diffPlace = TRUE;
								//check if Black wins
								CheckWhiteWins();
							}
						}
					} // ���� ���� �� �ΰ� �ϱ�
				}

			}// blackturn while ��
		} // if first = 2 ��
	} // while ��, ������ �ߴܵ��� ���� ��� ��� ������

	return;
}

//�Ͼ���� �̰���� �Ǻ�
void CheckWhiteWins(void)
{
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			
			if ((whiteCheck[i][y] == 1 && whiteCheck[i + 2][y] == 1 && whiteCheck[i + 4][y] == 1 && whiteCheck[i + 6][y] == 1 && whiteCheck[i + 8][y] == 1) || 
				(whiteCheck[x][i] == 1 && whiteCheck[x][i + 1] == 1 && whiteCheck[x][i + 2] == 1 && whiteCheck[x][i + 3] == 1 && whiteCheck[x][i + 4] == 1) || 
				(whiteCheck[j][i] == 1 && whiteCheck[j + 2][i + 1] == 1 && whiteCheck[j + 4][i + 2] == 1 && whiteCheck[j + 6][i + 3] == 1 && whiteCheck[j + 8][i + 4] == 1) || 
				(whiteCheck[j][i] == 1 && whiteCheck[j - 2][i + 1] == 1 && whiteCheck[j - 4][i + 2] == 1 && whiteCheck[j - 6][i + 3] == 1 && whiteCheck[j - 8][i + 4] == 1))
			{
				if (first == 2)
				{
					printWin();
					WhiteMsgBox();
				}
				else
				{
					printLose();
					WhiteMsgBox();
				}
				return;
			}
		}
	}
}

//�������� �̰���� �Ǻ�
void CheckBlackWins(void)
{
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			if ((blackCheck[i][y] == 1 && blackCheck[i + 2][y] == 1 && blackCheck[i + 4][y] == 1 && blackCheck[i + 6][y] == 1 && blackCheck[i + 8][y] == 1 && blackCheck[i + 10][y] == 1) ||
				(blackCheck[x][i] == 1 && blackCheck[x][i + 1] == 1 && blackCheck[x][i + 2] == 1 && blackCheck[x][i + 3] == 1 && blackCheck[x][i + 4] == 1 && blackCheck[x][i + 5] == 1) ||
				(blackCheck[j][i] == 1 && blackCheck[j + 2][i + 1] == 1 && blackCheck[j + 4][i + 2] == 1 && blackCheck[j + 6][i + 3] == 1 && blackCheck[j + 8][i + 4] == 1 && blackCheck[j + 10][i + 5] == 1) ||
				(blackCheck[j][i] == 1 && blackCheck[j - 2][i + 1] == 1 && blackCheck[j - 4][i + 2] == 1 && blackCheck[j - 6][i + 3] == 1 && blackCheck[j - 8][i + 4] == 1 && blackCheck[j - 10][i + 5] == 1))
			{
				if (first == 1)
				{
					printLose();
					msgID = MessageBoxA(NULL, "�ءء�  6�� �ݼ��Դϴ�! �浹�� �����ϴ�! �ءء�", "���� ��������!", MB_RETRYCANCEL);
				}
				else
				{
					printWin();
					msgID = MessageBoxA(NULL, "�١١�  ��밡 6�� �ݼ��� �׽��ϴ�! �鵹�� �̰���ϴ�!�١١�", "�����մϴ�!", MB_RETRYCANCEL);
				}
				return;
			}

			//check row
			 else if ((blackCheck[i][y] == 1 && blackCheck[i + 2][y] == 1 && blackCheck[i + 4][y] == 1 && blackCheck[i + 6][y] == 1 && blackCheck[i + 8][y] == 1) || 
				(blackCheck[x][i] == 1 && blackCheck[x][i + 1] == 1 && blackCheck[x][i + 2] == 1 && blackCheck[x][i + 3] == 1 && blackCheck[x][i + 4] == 1) || 
				(blackCheck[j][i] == 1 && blackCheck[j + 2][i + 1] == 1 && blackCheck[j + 4][i + 2] == 1 && blackCheck[j + 6][i + 3] == 1 && blackCheck[j + 8][i + 4] == 1) || 
				(blackCheck[j][i] == 1 && blackCheck[j - 2][i + 1] == 1 && blackCheck[j - 4][i + 2] == 1 && blackCheck[j - 6][i + 3] == 1 && blackCheck[j - 8][i + 4] == 1))
			{
				if (first == 1)
				{
					printWin();
					BlackMsgBox();
				}
				else
				{
					printLose();
					BlackMsgBox();
				}
				return;
			}
		}
	}
}
		
		
	
//�� ��ȯ
void txtColor(int color_number)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_number);
}


//Ŀ�� ���翩��
void CursorVis(void)
{
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.dwSize = 1;
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
	setcursortype(NOCURSOR);
}

void WhiteMsgBox(void)
{
	int msgID;

	if (first ==  2)
	msgID = MessageBoxA(NULL, "�١١�  �鵹�� �̰���ϴ�!  �١١�", "�����մϴ�!", MB_RETRYCANCEL);
	else
		msgID = MessageBoxA(NULL, "�ءء�  �浹�� �����ϴ�!  �ءء�", "�й� �ϼ���!", MB_RETRYCANCEL);

	switch (msgID)
	{
	case IDRETRY:
		txtColor(0);
		
		retry = 1;
		gameBreaker = TRUE;
		gameReboot = TRUE;

		system("cls");
		break;
	case IDCANCEL:
		gameBreaker = TRUE;
		gameReboot = FALSE;
		retry = 0;
		send(hClntSock, (char*)&retry, sizeof(retry), 0);
		exit(1);
		break;
	}

}




void BlackMsgBox(void)
{
	int msgID;
	
		if (first == 1)
			msgID = MessageBoxA(NULL, "�ڡڡ�  �浹�� �̰���ϴ�! �ڡڡ�", "�����մϴ�!", MB_RETRYCANCEL);
		else
			msgID = MessageBoxA(NULL, "�ءء�  �鵹�� �����ϴ�!  �ءء�", "�й� �ϼ���!", MB_RETRYCANCEL);
			
	switch (msgID)
	{
	case IDRETRY:
		txtColor(0);
		retry = 1;
		gameBreaker = TRUE;
		gameReboot = TRUE;
		system("cls");
		break;
	case IDCANCEL:
		gameBreaker = TRUE;
		gameReboot = FALSE;
		retry = 0;
		send(hClntSock, (char*)&retry, sizeof(retry), 0);
		exit(1);
		break;
	}
	return;
}

//������
void startScreen(void)
{
	spaceSelect = FALSE;
	char typed, typed2;
	gotoxy(7, 4);
	txtColor(3);
	printf("W");
	Sleep(100);
	printf("E");
	Sleep(100);
	printf("L");
	Sleep(100);
	printf("C");
	Sleep(100);
	printf("O");
	Sleep(100);
	printf("M");
	Sleep(100);
	printf("E ");
	Sleep(100);
	printf("T");
	Sleep(100);
	printf("O");
	Sleep(100);
	txtColor(14);
	gotoxy(7, 5);
	printf("��     ��      ��    ��        ��   �̢̢�   �̢̢�      �̢Ǣ�   ��\n");
	txtColor(14);
	gotoxy(7, 6);
	printf("�� ��  ��    ��  ��   ��      ��      ��    ��         ��      ��\n");
	txtColor(14);
	gotoxy(7, 7);
	printf("��  �� ��   �� �� ��   ��    ��       ��   ��  �ǢǢ�  ��      ��\n");
	txtColor(14);
	gotoxy(7, 8);
	printf("��   �ʢ�  ��      ��   ��  ��        ��    ��     ��  ��      ��\n");
	txtColor(14);
	gotoxy(7, 9);
	printf("��     �� ��        ��   �̢�       �̢̢�   �̢̢̢�    �̢Ǣ�\n");

	gotoxy(37, 13);
	txtColor(13);
	puts("Ver 1.0");
	gotoxy(33, 14);
	txtColor(3);
	puts("Made by Joon Kim");



	txtColor(10);
	gotoxy(6, 19);
	puts("������ ����                   ���� ���                   ũ����");
	// spacebar  ������ ������ �Դ� ���� ����
	while (spaceSelect == FALSE)
	{
		typed = _getch();
		Key_Down(typed);
	}

	if (server == 1)
		system("cls");
	else if (server == 2)
	{
		system("cls");
		gotoxy(20, 6);
		printf("��5���� ���� ���� �������� ���γ� ����, \n");
		gotoxy(22, 7);
		printf("�Ǵ� �밢�� �������� ���ƾ� �¸�~\n");
		gotoxy(32, 8);
		printf("�� �޸��ڰ�~��");
		txtColor(10);
		gotoxy(37, 17);
		puts("��BACK");

		typed = _getch();
		if (typed == 80);
		{
			gotoxy(37, 17);
			puts("��BACK");
		}

		typed2 = _getch();
		if (typed2 == 32);
		{
			system("cls");
			server = 1;
			startScreen();
		}

	}
	else if (server == 3)
	{
		system("cls");
		txtColor(15);
		puts("\n\n                            ������   CREDITS   ������ \n\n\n �� HEE HUN PARK ���ӿ�������\n\n �� TAE GEUN KIM ���ӿ�������\n\n �� JOHN UHM ��������\n\n �� JI WON KIM ��������\n\n �� NAVIWORKS NATIONAL DEFENSE TECHNOLOGY DEVELOPMENT DIVISION(MARINE SAFETY)\n\n COPYRIGHT 2016 JOON KIM NAVIWORKS");
		txtColor(10);
		gotoxy(37, 17);
		puts("��BACK");

		typed = _getch();
		if (typed == 80);
		{
			gotoxy(37, 17);
			puts("��BACK");
		}

		typed2 = _getch();
		if (typed2 == 32);
		{
			system("cls");
			server = 1;
			startScreen();
		}
	}

}

void Key_Down(char key)
{
	switch (key)
	{
	case 75:												// key���� ASCII�ڵ尡 75���� �� (���� ����Ű)
		if (server == 2)
		{
			gotoxy(6, 19);
			puts("������ ����                   ���� ���                   ũ����");
			server--;
		}
		else if (server == 3)
		{
			gotoxy(6, 19);
			puts("  ���� ����                 ������ ���                   ũ����");
			server--;
		}
		break;
	case 77:												// key���� ASCII�ڵ尡 77���� �� (������ ����Ű)
		if (server == 1)
		{
			gotoxy(6, 19);
			puts("  ���� ����                 ������ ���                   ũ����");
			server++;
		}
		else if (server == 2)
		{
			gotoxy(6, 19);
			puts("  ���� ����                   ���� ���                 ��ũ����");
			server++;
		}
		break;
	case 32:												// key���� ASCII�ڵ尡 32���� �� (�����̽���)
		spaceSelect = TRUE;
		break;
	}


}

//�̰��� ���
void printWin(void)
{
	PlaySound(TEXT("victory sound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
	txtColor(14);
	gotoxy(49, 12);
	printf("%s���� �¸��ϼ̽��ϴ�!", servName);
	txtColor(11);
	gotoxy(40, 13);
	puts(" ��          ��  �ܡܡ�  ��      ��  ��");
	Sleep(80);
	txtColor(12);
	gotoxy(40, 14);
	puts("  ��   ��   ��     ��    ��  ��  ��  ��");
	Sleep(80);
	txtColor(13);
	gotoxy(40, 15);
	puts("   �� �ܡ� ��      ��    ��   �� ��  ��");
	Sleep(80);
	txtColor(14);
	gotoxy(40, 16);
	puts("    ��    ��     �ܡܡ�  ��      ��  ��");
	Sleep(80);
	txtColor(14);
	gotoxy(0, 22);
	
}
//���� ���
void printLose(void)
{
	PlaySound(TEXT("Wha-Wha.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
	txtColor(14);
	gotoxy(49, 12);
	printf("%s���� ���̽��ϴ� �Ф�", servName);
	txtColor(8);
	gotoxy(40, 13);
	puts(" ��       �� ��       �� ��   �ܡܡ�   ");
	Sleep(80);
	gotoxy(40, 14);
	puts(" ��     ��     ��   �� ��     �ܡܡ�   ");
	Sleep(80);
	gotoxy(40, 15);
	puts(" ��     ��     ��      �� ��  ��      ");
	Sleep(80);
	gotoxy(40, 16);
	puts(" �ܡܡ�   �� ��      �� ��    �ܡܡ�  ");
	Sleep(80);
	gotoxy(0, 22);
	

}
 // ������ 33üũ
void black33(void)
{
	//�ϴ� �� ���� 1�̶� �ϰ� ����
	blackCheck[x][y] = 1;
	whiteCount = 0;
	blackCount = 0;
	error = 0;
	BOOL overlap = FALSE;
	tempCount = 0;

	
	memset(temp3, -1, sizeof(struct temp3Club) * 10);

	
	
	// // ==========������ ���� ���� ===========
	for (int i = 0; i < 8; i++)
	{
		if (blackCheck[x + i][y] == 1) // ��� ������ ī��Ʈ
			blackCount++;
		if (whiteCheck[x + i][y] == 1) // ��� ������ ī��Ʈ
			whiteCount++;
	}
	if (blackCheck[x + 8][y] == 1) // ���� �鵹�� ������
		if (blackCount == 4) // ���� �� 4�� ���� 5�� ������ �¸�
			return;
	if (blackCount == 3 && whiteCount == 0)
		if (!(blackCheck[x + 2][y] == 1 && blackCheck[x + 4][y] == 1 && whiteCheck[x-4][y] == 0)) // ���ʿ��� �˻����� ����3 ����
		if (whiteCheck[x-2][y] == 0 && blackCheck[x-2][y] == 0) // ���� �� �ٷ� �ڿ� ���̳� �������� ���� �� ��
		if (whiteCheck[x + 8][y] == 0 || (whiteCheck[x + 8][y] == 1 && whiteCheck[x + 6][y] == 0 && blackCheck[x + 6][y] == 0))// ���� 3�̸�
			{
				tempCount++; // �ӽ�3 �� �͵� ����
				for (int k = 0; k < 10; k++)
				{
					if (temp3[k].pos[0].pX == -1)
					{
						for (int i = 0; i < 8; i++)
						{
							if (blackCheck[x + i][y] == 1) // ��� ������ ī��Ʈ
							{
								temp3[k].pos[posNum].pX = x+i;
								temp3[k].pos[posNum].pY = y;
								posNum++;
							}
							
						}
						posNum = 0; // posNum �ʱ�ȭ
						break; // �� �� ã�Ƽ� ��ġ�� ���������� �ٸ� �� �� �� �鰡�� ���� �극��ũ
					} // �� �� ã�� ��
				} //��� ã�� ��
			} // ���� 3 üũ & �ӽð��� ��


	blackCount = 0;
	whiteCount = 0;
	//================������ ���� �� =========

	// =============���� ���� ���� ===========
	for (int i = 0; i < 8; i++)
	{
		if (blackCheck[x - i][y] == 1) // ��� ������ ī��Ʈ
			blackCount++;
		if (whiteCheck[x - i][y] == 1) // ��� ������ ī��Ʈ
			whiteCount++;
	}
	if (blackCheck[x - 8][y] == 1) // ���� �鵹�� ������
		if (blackCount == 4) // ���� �� 4�� ���� 5�� ������ �¸�
			return;
	if (blackCount == 3 && whiteCount == 0)
		if (!(blackCheck[x - 2][y] == 1 && blackCheck[x - 4][y] == 1 && whiteCheck[x + 4][y] == 0)) // ���ʿ��� �˻����� ����3 ����
		if (whiteCheck[x + 2][y] == 0 && blackCheck[x + 2][y] == 0) // ���� �� �ٷ� �ڿ� ���̳� �������� ���� �� ��
			if (whiteCheck[x - 8][y] == 0 || (whiteCheck[x - 8][y] == 1 && whiteCheck[x - 6][y] == 0 && blackCheck[x - 6][y] == 0))// ���� 3�̸�
			{
				tempCount++; // �ӽ�3 �� �͵� ����
				for (int k = 0; k < 10; k++)
				{
					if (temp3[k].pos[0].pX == -1)
					{
						for (int i = 0; i < 8; i++)
						{
							if (blackCheck[x - i][y] == 1) // ��� ������ ī��Ʈ
							{
								temp3[k].pos[posNum].pX = x-i;
								temp3[k].pos[posNum].pY = y;
								posNum++;
							}

						}
						posNum = 0; // posNum �ʱ�ȭ
						break; // �� �� ã�Ƽ� ��ġ�� ���������� �ٸ� �� �� �� �鰡�� ���� �극��ũ
					} // �� �� ã�� ��
				} //��� ã�� ��
			} // ���� 3 üũ & �ӽð��� ��

	blackCount = 0;
	whiteCount = 0;

	// ============�Ʒ����� ����=============
	for (int i = 0; i < 4; i++)
	{
		if (blackCheck[x][y + i] == 1) // ��� ������ ī��Ʈ
			blackCount++;
		if (whiteCheck[x][y + i] == 1) // ��� ������ ī��Ʈ
			whiteCount++;
	}
	if (blackCheck[x][y + 4] == 1) // ���� �鵹�� ������
		if (blackCount == 4) // ���� �� 4�� ���� 5�� ������ �¸�
			return;
	if (blackCount == 3 && whiteCount == 0)
		if (!(blackCheck[x][y + 1] == 1 && blackCheck[x][y + 2] == 1 && whiteCheck[x][y - 2] == 0)) // ���ʿ��� �˻����� ����3 ����
		if (whiteCheck[x][y - 1] == 0 && blackCheck[x][y - 1] == 0) // ���� �� �ٷ� �ڿ� ���̳� �������� ���� �� ��
			if (whiteCheck[x][y+4] == 0 || (whiteCheck[x][y+4] == 1 && whiteCheck[x][y+3] == 0 && blackCheck[x][y+3] == 0))// ���� 3�̸�
			{
				tempCount++; // �ӽ�3 �� �͵� ����
				for (int k = 0; k < 10; k++)
				{
					if (temp3[k].pos[0].pX == -1)
					{
						for (int i = 0; i < 4; i++)
						{
							if (blackCheck[x ][y+i] == 1) // ��� ������ ī��Ʈ
							{
								temp3[k].pos[posNum].pX = x;
								temp3[k].pos[posNum].pY = y + i;
								posNum++;
							}

						}
						posNum = 0; // posNum �ʱ�ȭ
						break; // �� �� ã�Ƽ� ��ġ�� ���������� �ٸ� �� �� �� �鰡�� ���� �극��ũ
					} // �� �� ã�� ��
				} //��� ã�� ��
			} // ���� 3 üũ & �ӽð��� ��
	blackCount = 0;
	whiteCount = 0;
	// ================ �Ʒ� ���� �� ====

	// =============���� ���� ���� =====
	for (int i = 0; i < 4; i++)
	{
		if (blackCheck[x][y - i] == 1) // ��� ������ ī��Ʈ
			blackCount++;
		if (whiteCheck[x][y - i] == 1) // ��� ������ ī��Ʈ
			whiteCount++;
	}
	if (blackCheck[x][y - 4] == 1) // ���� �鵹�� ������
		if (blackCount == 4) // ���� �� 4�� ���� 5�� ������ �¸�
			return;
	if (blackCount == 3 && whiteCount == 0)
		if (!(blackCheck[x][y - 1] == 1 && blackCheck[x][y - 2] == 1 && whiteCheck[x][y + 2] == 0)) // ���ʿ��� �˻����� ����3 ����
		if (whiteCheck[x][y + 1] == 0 && blackCheck[x][y + 1] == 0) // ���� �� �ٷ� �ڿ� ���̳� �������� ���� �� ��
			if (whiteCheck[x][y - 4] == 0 || (whiteCheck[x][y - 4] == 1 && whiteCheck[x][y - 3] == 0 && blackCheck[x][y - 3] == 0))// ���� 3�̸�
			{
				tempCount++; // �ӽ�3 �� �͵� ����
				for (int k = 0; k < 10; k++)
				{
					if (temp3[k].pos[0].pX == -1)
					{
						for (int i = 0; i < 4; i++)
						{
							if (blackCheck[x][y - i] == 1) // ��� ������ ī��Ʈ
							{
								temp3[k].pos[posNum].pX = x;
								temp3[k].pos[posNum].pY = y - i;
								posNum++;
							}

						}
						posNum = 0; // posNum �ʱ�ȭ
						break; // �� �� ã�Ƽ� ��ġ�� ���������� �ٸ� �� �� �� �鰡�� ���� �극��ũ
					} // �� �� ã�� ��
				} //��� ã�� ��
			} // ���� 3 üũ & �ӽð��� ��
	blackCount = 0;
	whiteCount = 0;
	//========���� ���� ��===========

	// ========���� �밢�� ����========
	for (int i = 0; i < 8; i += 2)
	{
		if (blackCheck[x + i][y + (i / 2)] == 1) // ��� ������ ī��Ʈ
			blackCount++;
		if (whiteCheck[x + i][y + (i / 2)] == 1) // ��� ������ ī��Ʈ
			whiteCount++;
	}
	if (blackCheck[x + 8][y + 4] == 1) // ���� �鵹�� ������
		if (blackCount == 4) // ���� �� 4�� ���� 5�� ������ �¸�
			return;
	if (blackCount == 3 && whiteCount == 0)
		if (!(blackCheck[x + 2][y + 1] == 1 && blackCheck[x + 4][y + 2] == 1 && whiteCheck[x-4][y - 2] == 0)) // ���ʿ��� �˻����� ����3 ����
		if (whiteCheck[x - 2][y - 1] == 0 && blackCheck[x-2][y - 1] == 0) // ���� �� �ٷ� �ڿ� ���̳� �������� ���� �� ��
			if (whiteCheck[x+8][y + 4] == 0 || (whiteCheck[x+8][y + 4] == 1 && whiteCheck[x+6][y + 3] == 0 && blackCheck[x+6][y + 3] == 0))// ���� 3�̸�
			{
				tempCount++; // �ӽ�3 �� �͵� ����
				for (int k = 0; k < 10; k++)
				{
					if (temp3[k].pos[0].pX == -1)
					{
						for (int i = 0; i < 8; i+=2)
						{
							if (blackCheck[x + i][y + (i / 2)] == 1) // ��� ������ ī��Ʈ
							{
								temp3[k].pos[posNum].pX = x + i;
								temp3[k].pos[posNum].pY = y + (i / 2);
								posNum++;
							}

						}
						posNum = 0; // posNum �ʱ�ȭ
						break; // �� �� ã�Ƽ� ��ġ�� ���������� �ٸ� �� �� �� �鰡�� ���� �극��ũ
					} // �� �� ã�� ��
				} //��� ã�� ��
			} // ���� 3 üũ & �ӽð��� ��
	blackCount = 0;
	whiteCount = 0;
	//====================���� �밢�� ��===============

	// ==============�»� �밢�� ����==============
	for (int i = 0; i < 8; i += 2)
	{
		if (blackCheck[x - i][y - (i / 2)] == 1) // ��� ������ ī��Ʈ
			blackCount++;
		if (whiteCheck[x - i][y - (i / 2)] == 1) // ��� ������ ī��Ʈ
			whiteCount++;
	}
	if (blackCheck[x - 8][y - 4] == 1) // ���� �鵹�� ������
		if (blackCount == 4) // ���� �� 4�� ���� 5�� ������ �¸�
			return;
	if (blackCount == 3 && whiteCount == 0)
		if (!(blackCheck[x - 2][y - 1] == 1 && blackCheck[x - 4][y - 2] == 1 && whiteCheck[x + 4][y + 2] == 0)) // ���ʿ��� �˻����� ����3 ����
		if (whiteCheck[x + 2][y + 1] == 0 && blackCheck[x + 2][y + 1] == 0) // ���� �� �ٷ� �ڿ� ���̳� �������� ���� �� ��
			if (whiteCheck[x - 8][y - 4] == 0 || (whiteCheck[x - 8][y - 4] == 1 && whiteCheck[x - 6][y - 3] == 0 && blackCheck[x - 6][y - 3] == 0))// ���� 3�̸�
			{
				tempCount++; // �ӽ�3 �� �͵� ����
				for (int k = 0; k < 10; k++)
				{
					if (temp3[k].pos[0].pX == -1)
					{
						for (int i = 0; i < 8; i += 2)
						{
							if (blackCheck[x - i][y - (i / 2)] == 1) // ��� ������ ī��Ʈ
							{
								temp3[k].pos[posNum].pX = x - i;
								temp3[k].pos[posNum].pY = y - (i / 2);
								posNum++;
							}

						}
						posNum = 0; // posNum �ʱ�ȭ
						break; // �� �� ã�Ƽ� ��ġ�� ���������� �ٸ� �� �� �� �鰡�� ���� �극��ũ
					} // �� �� ã�� ��
				} //��� ã�� ��
			} // ���� 3 üũ & �ӽð��� ��
	blackCount = 0;
	whiteCount = 0;
	// ==========�»� �밢�� ��===========

	// ==============��� �밢�� ����============
	for (int i = 0; i < 8; i += 2)
	{
		if (blackCheck[x + i][y - (i / 2)] == 1) // ��� ������ ī��Ʈ
			blackCount++;
		if (whiteCheck[x + i][y - (i / 2)] == 1) // ��� ������ ī��Ʈ
			whiteCount++;
	}
	if (blackCheck[x + 8][y - 4] == 1) // ���� �鵹�� ������
		if (blackCount == 4) // ���� �� 4�� ���� 5�� ������ �¸�
			return;
	if (blackCount == 3 && whiteCount == 0)
		if (!(blackCheck[x + 2][y - 1] == 1 && blackCheck[x + 4][y - 2] == 1 && whiteCheck[x - 4][y + 2] == 0)) // ���ʿ��� �˻����� ����3 ����
		if (whiteCheck[x - 2][y + 1] == 0 && blackCheck[x - 2][y + 1] == 0) // ���� �� �ٷ� �ڿ� ���̳� �������� ���� �� ��
			if (whiteCheck[x + 8][y - 4] == 0 || (whiteCheck[x+ 8][y - 4] == 1 && whiteCheck[x + 6][y - 3] == 0 && blackCheck[x + 6][y - 3] == 0))// ���� 3�̸�
			{
				tempCount++; // �ӽ�3 �� �͵� ����
				for (int k = 0; k < 10; k++)
				{
					if (temp3[k].pos[0].pX == -1)
					{
						for (int i = 0; i < 8; i += 2)
						{
							if (blackCheck[x + i][y - (i / 2)] == 1) // ��� ������ ī��Ʈ
							{
								temp3[k].pos[posNum].pX = x + i;
								temp3[k].pos[posNum].pY = y - (i / 2);
								posNum++;
							}

						}
						posNum = 0; // posNum �ʱ�ȭ
						break; // �� �� ã�Ƽ� ��ġ�� ���������� �ٸ� �� �� �� �鰡�� ���� �극��ũ
					} // �� �� ã�� ��
				} //��� ã�� ��
			} // ���� 3 üũ & �ӽð��� ��
	blackCount = 0;
	whiteCount = 0;

	//=================��� �밢�� ��====================

	//=====================���� �밢�� ����===================
	for (int i = 0; i < 8; i += 2)
	{
		if (blackCheck[x - i][y + (i / 2)] == 1) // ��� ������ ī��Ʈ
			blackCount++;
		if (whiteCheck[x - i][y + (i / 2)] == 1) // ��� ������ ī��Ʈ
			whiteCount++;
	}
	if (blackCheck[x - 8][y + 4] == 1) // ���� �鵹�� ������
		if (blackCount == 4) // ���� �� 4�� ���� 5�� ������ �¸�
			return;
	if (blackCount == 3 && whiteCount == 0)
		if (!(blackCheck[x - 2][y + 1] == 1 && blackCheck[x - 4][y + 2] == 1 && whiteCheck[x + 4][y - 2] == 0)) // ���ʿ��� �˻����� ����3 ����
		if (whiteCheck[x + 2][y - 1] == 0 && blackCheck[x + 2][y - 1] == 0) // ���� �� �ٷ� �ڿ� ���̳� �������� ���� �� ��
			if (whiteCheck[x - 8][y + 4] == 0 || (whiteCheck[x - 8][y + 4] == 1 && whiteCheck[x - 6][y + 3] == 0 && blackCheck[x - 6][y + 3] == 0))// ���� 3�̸�
			{
				tempCount++; // �ӽ�3 �� �͵� ����
				for (int k = 0; k < 10; k++)
				{
					if (temp3[k].pos[0].pX == -1)
					{
						for (int i = 0; i < 8; i += 2)
						{
							if (blackCheck[x - i][y + (i / 2)]) // ��� ������ ī��Ʈ
							{
								temp3[k].pos[posNum].pX = x - i;
								temp3[k].pos[posNum].pY = y + (i / 2);
								posNum++;
							}

						}
						posNum = 0; // posNum �ʱ�ȭ
						break; // �� �� ã�Ƽ� ��ġ�� ���������� �ٸ� �� �� �� �鰡�� ���� �극��ũ
					} // �� �� ã�� ��
				} //��� ã�� ��
			} // ���� 3 üũ & �ӽð��� ��
	blackCount = 0;
	whiteCount = 0;
	//======================���� �밢�� ��=========================
	//�� �� �߽����� 5ĭ�� ��� ��

	// �� �� �� ������ �� �� ���� ����
	//========���� ���� 5ĭ ����======
	for (int i = -4; i < 5; i += 2)
	{
		if (blackCheck[x + i][y] == 1) // ��� ������ ī��Ʈ
			blackCount++;
		if (whiteCheck[x + i][y] == 1) // ��� ������ ī��Ʈ
			whiteCount++;
	}
	if (blackCount == 5)
		return;

	if (blackCount == 3 && whiteCount == 0)
	{
		if (!(blackCheck[x + 4][y] == 1 && blackCheck[x - 4][y] == 1)) // 0  0  0 ����
		{
			if (blackCheck[x + 6][y] == 0 && blackCheck[x - 6][y] == 0) // ���ʳ� �ʸӿ��� �鵹 ������ ���� (����3 �ƴ�)
			{
				if (blackCheck[x + 4][y] == 1) // ������ ������ �� �ְ�
				{
					if (blackCheck[x + 6][y] == 0 && whiteCheck[x + 6][y] == 0) // �� �ڿ� �ƹ��͵� ������ ����3
					{
						tempCount++; // �ӽ�3 �� �͵� ����
						for (int k = 0; k < 10; k++)
						{
							if (temp3[k].pos[0].pX == -1)
							{
								for (int i = -4; i < 5; i += 2)
								{
									if (blackCheck[x + i][y]) // ��� ������ ī��Ʈ
									{
										temp3[k].pos[posNum].pX = x + i;
										temp3[k].pos[posNum].pY = y;
										posNum++;
									}

								}
								posNum = 0; // posNum �ʱ�ȭ
								break; // �� �� ã�Ƽ� ��ġ�� ���������� �ٸ� �� �� �� �鰡�� ���� �극��ũ
							} // �� �� ã�� ��
						} //��� ã�� ��
					}
				} // ������ �� ���� ��
				else if (blackCheck[x - 4][y] == 1) // ���� ������ �� �ְ�
				{
					if (blackCheck[x - 6][y] == 0 && whiteCheck[x - 6][y] == 0) // �� �ڿ� �ƹ��͵� ������ 
					{
						tempCount++; // �ӽ�3 �� �͵� ����
						for (int k = 0; k < 10; k++)
						{
							if (temp3[k].pos[0].pX == -1)
							{
								for (int i = -4; i < 5; i += 2)
								{
									if (blackCheck[x + i][y]) // ��� ������ ī��Ʈ
									{
										temp3[k].pos[posNum].pX = x + i;
										temp3[k].pos[posNum].pY = y;
										posNum++;
									}

								}
								posNum = 0; // posNum �ʱ�ȭ
								break; // �� �� ã�Ƽ� ��ġ�� ���������� �ٸ� �� �� �� �鰡�� ���� �극��ũ
							} // �� �� ã�� ��
						} //��� ã�� ��
					}
				} // ���� �� ���� ��
				else // �߾� 3�� ���� 3
				{
					tempCount++; // �ӽ�3 �� �͵� ����
					for (int k = 0; k < 10; k++)
					{
						if (temp3[k].pos[0].pX == -1)
						{
							for (int i = -4; i < 5; i += 2)
							{
								if (blackCheck[x + i][y]) // ��� ������ ī��Ʈ
								{
									temp3[k].pos[posNum].pX = x+i;
									temp3[k].pos[posNum].pY = y;
									posNum++;
								}

							}
							posNum = 0; // posNum �ʱ�ȭ
							break; // �� �� ã�Ƽ� ��ġ�� ���������� �ٸ� �� �� �� �鰡�� ���� �극��ũ
						} // �� �� ã�� ��
					} //��� ã�� ��
				} // �߾� 3�� ��� ��


			} // ���� ���� ���� �ִ°� �ƴ� �� ��
		}
	} // (blackCount == 3 && whiteCount == 0) ��
	blackCount = 0;
	whiteCount = 0;
	//===============���� ���� 5ĭ ���� ��================

	//========���� ���� 5ĭ ����======
	for (int i = -2; i < 3; i ++)
	{
		if (blackCheck[x][y + i] == 1) // ��� ������ ī��Ʈ
			blackCount++;
		if (whiteCheck[x][y + i] == 1) // ��� ������ ī��Ʈ
			whiteCount++;
	}
	if (blackCount == 5)
		return;

	if (blackCount == 3 && whiteCount == 0)
	{
		if (!(blackCheck[x][y + 2] == 1 && blackCheck[x][y - 2] == 1)) // 0  0  0 ����
		{
			if (blackCheck[x][y + 3] == 0 && blackCheck[x][y - 3] == 0) // ���ʳ� �ʸӿ��� �鵹 ������ ���� (����3 �ƴ�)
			{
				if (blackCheck[x][y + 2] == 1) // ������ ������ �� �ְ�
				{
					if (blackCheck[x][y + 3] == 0 && whiteCheck[x][y + 3] == 0) // �� �ڿ� �ƹ��͵� ������ 
					{
						tempCount++; // �ӽ�3 �� �͵� ����
						for (int k = 0; k < 10; k++)
						{
							if (temp3[k].pos[0].pX == -1)
							{
								for (int i = -2; i < 3; i ++)
								{
									if (blackCheck[x ][y+i]) // ��� ������ ī��Ʈ
									{
										temp3[k].pos[posNum].pX = x;
										temp3[k].pos[posNum].pY = y + i;
										posNum++;
									}

								}
								posNum = 0; // posNum �ʱ�ȭ
								break; // �� �� ã�Ƽ� ��ġ�� ���������� �ٸ� �� �� �� �鰡�� ���� �극��ũ
							} // �� �� ã�� ��
						} //��� ã�� ��
					}
				} // �Ʒ� �� ���� ��
				else if (blackCheck[x][y - 2] == 1) // ���� ������ �� �ְ�
				{
					if (blackCheck[x][y - 3] == 0 && whiteCheck[x][y-3] == 0) // �� �ڿ� �ƹ��͵� ������ 
					{
						tempCount++; // �ӽ�3 �� �͵� ����
						for (int k = 0; k < 10; k++)
						{
							if (temp3[k].pos[0].pX == -1)
							{
								for (int i = -2; i < 3; i++)
								{
									if (blackCheck[x][y + i]) // ��� ������ ī��Ʈ
									{
										temp3[k].pos[posNum].pX = x;
										temp3[k].pos[posNum].pY = y + i;
										posNum++;
									}

								}
								posNum = 0; // posNum �ʱ�ȭ
								break; // �� �� ã�Ƽ� ��ġ�� ���������� �ٸ� �� �� �� �鰡�� ���� �극��ũ
							} // �� �� ã�� ��
						} //��� ã�� ��
					}
				} // ���� �� ���� ��
				else // �߾� 3�� ���� 3
				{
					tempCount++; // �ӽ�3 �� �͵� ����
					for (int k = 0; k < 10; k++)
					{
						if (temp3[k].pos[0].pX == -1)
						{
							for (int i = -2; i < 3; i++)
							{
								if (blackCheck[x][y + i]) // ��� ������ ī��Ʈ
								{
									temp3[k].pos[posNum].pX = x;
									temp3[k].pos[posNum].pY = y + i;
									posNum++;
								}

							}
							posNum = 0; // posNum �ʱ�ȭ
							break; // �� �� ã�Ƽ� ��ġ�� ���������� �ٸ� �� �� �� �鰡�� ���� �극��ũ
						} // �� �� ã�� ��
					} //��� ã�� ��
				} // �߾� 3�� ��� ��


			} // ���� ���� ���� �ִ°� �ƴ� �� ��
		}
	} // (blackCount == 3 && whiteCount == 0) ��	
	blackCount = 0;
	whiteCount = 0;
	//===============���� ���� 5ĭ ���� ��================

	//===============���� �»���� 5ĭ ����================
	for (int i = -4; i < 5; i+=2)
	{
		if (blackCheck[x+i][y + (i/2)] == 1) // ��� ������ ī��Ʈ
			blackCount++;
		if (whiteCheck[x+i][y + (i/2)] == 1) // ��� ������ ī��Ʈ
			whiteCount++;
	}
	if (blackCount == 5)
		return;

	if (blackCount == 3 && whiteCount == 0)
	{
		if (!(blackCheck[x - 4][y - 2] == 1 && blackCheck[x+4][y + 2] == 1)) // 0  0  0 ����
		{
			if (blackCheck[x + 6][y + 3] == 0 && blackCheck[x-6][y - 3] == 0) // ���ʳ� �ʸӿ��� �鵹 ������ ���� (����3 �ƴ�)
			{
				if (blackCheck[x+4][y + 2] == 1) // ������ ������ �� �ְ�
				{
					if (blackCheck[x+6][y + 3] == 0 && whiteCheck[x+6][y + 3] == 0) // �� �ڿ� �ƹ��͵� ������ 
					{
						tempCount++; // �ӽ�3 �� �͵� ����
						for (int k = 0; k < 10; k++)
						{
							if (temp3[k].pos[0].pX == -1)
							{
								for (int i = -4; i < 5; i += 2)
								{
									if (blackCheck[x + i][y + (i / 2)] == 1) // ��� ������ ī��Ʈ
									{
										temp3[k].pos[posNum].pX = x + i;
										temp3[k].pos[posNum].pY = y + (i / 2);
										posNum++;
									}

								}
								posNum = 0; // posNum �ʱ�ȭ
								break; // �� �� ã�Ƽ� ��ġ�� ���������� �ٸ� �� �� �� �鰡�� ���� �극��ũ
							} // �� �� ã�� ��
						} //��� ã�� ��
					}
				} // �Ʒ� �� ���� ��
				else if (blackCheck[x-4][y - 2] == 1) // ���� ������ �� �ְ�
				{
					if (blackCheck[x-6][y - 3] == 0 && whiteCheck[x-6][y - 3] == 0) // �� �ڿ� �ƹ��͵� ������ 
					{
						tempCount++; // �ӽ�3 �� �͵� ����
						for (int k = 0; k < 10; k++)
						{
							if (temp3[k].pos[0].pX == -1)
							{
								for (int i = -4; i < 5; i += 2)
								{
									if (blackCheck[x + i][y + (i / 2)] == 1) // ��� ������ ī��Ʈ
									{
										temp3[k].pos[posNum].pX = x + i;
										temp3[k].pos[posNum].pY = y + (i / 2);
										posNum++;
									}

								}
								posNum = 0; // posNum �ʱ�ȭ
								break; // �� �� ã�Ƽ� ��ġ�� ���������� �ٸ� �� �� �� �鰡�� ���� �극��ũ
							} // �� �� ã�� ��
						} //��� ã�� ��
					}
				} // ���� �� ���� ��
				else // �߾� 3�� ���� 3
				{
					tempCount++; // �ӽ�3 �� �͵� ����
					for (int k = 0; k < 10; k++)
					{
						if (temp3[k].pos[0].pX == -1)
						{
							
							for (int i = -4; i < 5; i += 2)
							{
								if (blackCheck[x + i][y + (i / 2)] == 1) // ��� ������ ī��Ʈ
								{
									temp3[k].pos[posNum].pX = x + i;
									temp3[k].pos[posNum].pY = y + (i / 2);
									posNum++;
								}

							}
							posNum = 0; // posNum �ʱ�ȭ
							break; // �� �� ã�Ƽ� ��ġ�� ���������� �ٸ� �� �� �� �鰡�� ���� �극��ũ
						} // �� �� ã�� ��
					} //��� ã�� ��
				} // �߾� 3�� ��� ��


			} // ���� ���� ���� �ִ°� �ƴ� �� ��
		}
	} // (blackCount == 3 && whiteCount == 0) ��	
	blackCount = 0;
	whiteCount = 0;
	// =====================�»���� �밢�� ���� ���� ��=================
	

	//===============���� ���Ͽ�� 5ĭ ����================
	for (int i = -4; i < 5; i += 2)
	{
		if (blackCheck[x + i][y - (i / 2)] == 1) // ��� ������ ī��Ʈ
			blackCount++;
		if (whiteCheck[x + i][y - (i / 2)] == 1) // ��� ������ ī��Ʈ
			whiteCount++;
	}
	if (blackCount == 5)
		return;

	if (blackCount == 3 && whiteCount == 0)
	{
		if (!(blackCheck[x - 4][y + 2] == 1 && blackCheck[x + 4][y - 2] == 1)) // 0  0  0 ����
		{
			if (blackCheck[x - 6][y + 3] == 0 && blackCheck[x + 6][y - 3] == 0) // ���ʳ� �ʸӿ��� �鵹 ������ ���� (����3 �ƴ�)
			{
				if (blackCheck[x + 4][y - 2] == 1) // ������ ������ �� �ְ�
				{
					if (blackCheck[x + 6][y - 3] == 0 && whiteCheck[x + 6][y - 3] == 0) // �� �ڿ� �ƹ��͵� ������ 
					{
						tempCount++; // �ӽ�3 �� �͵� ����
						for (int k = 0; k < 10; k++)
						{
							if (temp3[k].pos[0].pX == -1)
							{
								for (int i = -4; i < 5; i += 2)
								{
									if (blackCheck[x + i][y - (i / 2)] == 1) // ��� ������ ī��Ʈ
									{
										temp3[k].pos[posNum].pX = x + i;
										temp3[k].pos[posNum].pY = y - (i / 2);
										posNum++;
									}

								}
								posNum = 0; // posNum �ʱ�ȭ
								break; // �� �� ã�Ƽ� ��ġ�� ���������� �ٸ� �� �� �� �鰡�� ���� �극��ũ
							} // �� �� ã�� ��
						} //��� ã�� ��
					}
				} // ��� �� ���� ��
				else if (blackCheck[x - 4][y + 2] == 1) // ���� ������ �� �ְ�
				{
					if (blackCheck[x - 6][y + 3] == 0 && whiteCheck[x - 6][y + 3] == 0) // �� �ڿ� �ƹ��͵� ������ 
					{
						tempCount++; // �ӽ�3 �� �͵� ����
						for (int k = 0; k < 10; k++)
						{
							if (temp3[k].pos[0].pX == -1)
							{
								for (int i = -4; i < 5; i += 2)
								{
									if (blackCheck[x + i][y - (i / 2)] == 1) // ��� ������ ī��Ʈ
									{
										temp3[k].pos[posNum].pX = x + i;
										temp3[k].pos[posNum].pY = y - (i / 2);
										posNum++;
									}

								}
								posNum = 0; // posNum �ʱ�ȭ
								break; // �� �� ã�Ƽ� ��ġ�� ���������� �ٸ� �� �� �� �鰡�� ���� �극��ũ
							} // �� �� ã�� ��
						} //��� ã�� ��
					}
				} // ���� �� ���� ��
				else // �߾� 3�� ���� 3
				{
					for (int k = 0; k < 10; k++)
					{
						tempCount++; // �ӽ�3 �� �͵� ����
						if (temp3[k].pos[0].pX == -1)
						{
							for (int i = -4; i < 5; i += 2)
							{
								if (blackCheck[x + i][y - (i / 2)] == 1) // ��� ������ ī��Ʈ
								{
									temp3[k].pos[posNum].pX = x + i;
									temp3[k].pos[posNum].pY = y - (i / 2);
									posNum++;
								}

							}
							posNum = 0; // posNum �ʱ�ȭ
							break; // �� �� ã�Ƽ� ��ġ�� ���������� �ٸ� �� �� �� �鰡�� ���� �극��ũ
						} // �� �� ã�� ��
					} //��� ã�� ��
				} // �߾� 3�� ��� ��


			} // ���� ���� ���� �ִ°� �ƴ� �� ��
		}
	} // (blackCount == 3 && whiteCount == 0) ��	
	blackCount = 0;
	whiteCount = 0;
	// =====================�»���� �밢�� ���� ���� ��=================
	// ====================���� 5ĭ ���� ��=========================

	//if (tempCount == 1) // �� ���� 33�������� �ƴ϶��
	//{
	//	//�̹� ����3 Ŭ�� ȸ�� �߿� ��ġ�� ��ġ�� �ִ��� Ȯ��
	//	for (int r = 0; r < 100; r++)
	//	{
	//		for (int e = 0; e < 3; e++)
	//		{
	//			for (int w = 0; w < 3; w++)
	//			{
	//				if (innocent3[r].pos[e].pX == temp3[0].pos[w].pX && innocent3[r].pos[e].pY == temp3[0].pos[w].pY)
	//				{
	//					overlap = TRUE;
	//					gotoxy(x, y);
	//					txtColor(0 | 96);
	//					printf("��");
	//					blackCheck[x][y] = 1;
	//					error = 1;
	//					break;
	//				}
	//			}	
	//			if (overlap) // �ϳ��� �߰ߵǸ� �� ������
	//				break;
	//		}
	//		if (overlap)// �ϳ��� �߰ߵǸ� �� ������
	//			break;
	//	}

	//	//����3 ȸ���� �߿� ��ġ�� ȸ�� ������ ���� ����
	//	if (overlap == FALSE)
	//	for (int k = 0; k < 100; k++)
	//	{
	//		if (innocent3[k].pos[0].pX == -1)
	//		{
	//			innocent3[k].pos[0].pX = temp3[0].pos[0].pX; // ����3Ŭ�� ����
	//			innocent3[k].pos[0].pY = temp3[0].pos[0].pY;
	//			innocent3[k].pos[1].pX = temp3[0].pos[1].pX;
	//			innocent3[k].pos[1].pY = temp3[0].pos[1].pY;
	//			innocent3[k].pos[2].pX = temp3[0].pos[2].pX;
	//			innocent3[k].pos[2].pY = temp3[0].pos[2].pY;
	//			break;
	//		}

	//	}
	//} // ����3Ŭ�� ���ԿϷ�
	/*else*/ if (tempCount > 1) // �������� ���(33)
	{
		gotoxy(x, y);
		txtColor(0 | 96);
		printf("��");
		blackCheck[x][y] = 1;
		error = 1;
	}
				
	if (overlap == FALSE && tempCount < 2)
	{
		gotoxy(x, y);
		txtColor(0 | 96);
		printf("��");
		blackCheck[x][y] = 1;
	}

	overlap = FALSE;
		//��� �� ���ٰ� �ʱ�ȭ
	
}

//=================white Exile====================
//�鵹�� �ξ��� �� ����3 Ż�� ��
void whiteExile3(int a, int b)
{
	count = 0;
	blackCount = 0;
	whiteCount = 0;
	// =====���ʿ� �ΰ� ���� ��====
	for (int i = -2; i < 3; i+=2)
	{
		for (int j = -1; j < 2; j++)
		{
			if (blackCheck[a + i][b+j] == 1 && blackCheck[a - i][b - j] == 1)
			{
				for (int k = 0; k < 100; k++)
				{
					for (int h = 0; h < 3; h++)
					{
						if (innocent3[k].pos[h].pX == (a + i) && innocent3[k].pos[h].pY == (b+j))
							count++;
						if (innocent3[k].pos[h].pX == (a - i) && innocent3[k].pos[h].pY == (b-j))
							count++;
					}
					if (count == 2)
					{
						memset(&innocent3[k], -1, sizeof(struct innocent3Club));
						count = 0;
						break;
					}
					count = 0;// reset
				}
			}
		}
	} // ========�� ���� �ΰ� ���� �� ��=========

	// =========���� �Ѱ� ���� ��===========
	if (blackCheck[a + 2][b] == 1) // ������ ���� �Ѱ� ���� ��
	{
		for (int i = 2; i < 10; i += 2) // 4ĭ �������� �Ͼ�3 ��0���� üũ
		{
			if (blackCheck[a + i][b] == 1)
				blackCount++;
			if (whiteCheck[a + i][b] == 1)
				whiteCount++;
		}
		if (blackCount == 3 && whiteCount == 0)
			for (int k = 0; k < 100; k++)
			{
				for (int h = 0; h < 3; h++)
				{
					for (int i = 2; i < 10; i += 2)
					{
						if (innocent3[k].pos[h].pX == (a + i) && innocent3[k].pos[h].pY == (b))
							count++;
					}
				}
				if (count > 0)
				{
					memset(&innocent3[k], -1, sizeof(struct innocent3Club));
					count = 0;
					break;
				}
				count = 0;// reset
			}

	} // ������ ���� �Ѱ� ��
	blackCount = 0;
	whiteCount = 0;

	if (blackCheck[a - 2][b] == 1) // ���� ���� �Ѱ� ���� ��
	{
		for (int i = 2; i < 10; i+=2) // 4ĭ �������� �Ͼ�3 ��0���� üũ
		{
			if (blackCheck[a - i][b] == 1)
				blackCount++;
			if (whiteCheck[a - i][b] == 1)
				whiteCount++;
		}
		if (blackCount == 3 && whiteCount == 0)
			for (int k = 0; k < 100; k++)
			{
				for (int h = 0; h < 3; h++)
				{
					for (int i = 2; i < 10; i += 2)
					{
						if (innocent3[k].pos[h].pX == (a - i) && innocent3[k].pos[h].pY == (b))
							count++;
					}
				}
				if (count > 0)
				{
					memset(&innocent3[k], -1, sizeof(struct innocent3Club));
					count = 0;
					break;
				}
				count = 0;// reset
			}

	}
	blackCount = 0;
	whiteCount = 0;

	if (blackCheck[a][b-1] == 1) // ���� ���� �Ѱ� ���� ��
	{
		for (int i = 1; i < 5; i++) // 4ĭ �������� �Ͼ�3 ��0���� üũ
		{
			if (blackCheck[a][b-i] == 1)
				blackCount++;
			if (whiteCheck[a][b-i] == 1)
				whiteCount++;
		}
		if (blackCount == 3 && whiteCount == 0)
			for (int k = 0; k < 100; k++)
			{
				for (int h = 0; h < 3; h++)
				{
					for (int i = 1; i < 5; i++)
					{
						if (innocent3[k].pos[h].pX == (a) && innocent3[k].pos[h].pY == (b-i))
							count++;
					}
				}
				if (count > 0)
				{
					memset(&innocent3[k], -1, sizeof(struct innocent3Club));
					count = 0;
					break;
				}
				count = 0;// reset
			}

	}
	blackCount = 0;
	whiteCount = 0;

	if (blackCheck[a][b + 1] == 1) // �Ʒ��� ���� �Ѱ� ���� ��
	{
		for (int i = 1; i < 5; i++) // 4ĭ �������� �Ͼ�3 ��0���� üũ
		{
			if (blackCheck[a][b + i] == 1)
				blackCount++;
			if (whiteCheck[a][b + i] == 1)
				whiteCount++;
		}
		if (blackCount == 3 && whiteCount == 0)
			for (int k = 0; k < 100; k++)
			{
				for (int h = 0; h < 3; h++)
				{
					for (int i = 1; i < 5; i++)
					{
						if (innocent3[k].pos[h].pX == (a) && innocent3[k].pos[h].pY == (b + i))
							count++;
					}
				}
				if (count > 0)
				{
					memset(&innocent3[k], -1, sizeof(struct innocent3Club));
					count = 0;
					break;
				}
				count = 0;// reset
			}

	}
	blackCount = 0;
	whiteCount = 0;
	
	if (blackCheck[a+2][b + 1] == 1) // ���ϴ밢�� �Ѱ� ���� ��
	{
		for (int i = 2; i < 10; i+=2) // 4ĭ �������� �Ͼ�3 ��0���� üũ
		{
			if (blackCheck[a+i][b + (i/2)] == 1)
				blackCount++;
			if (whiteCheck[a+i][b + (i/2)] == 1)
				whiteCount++;
		}
		if (blackCount == 3 && whiteCount == 0)
			for (int k = 0; k < 100; k++)
			{
				for (int h = 0; h < 3; h++)
				{
					for (int i = 2; i < 10; i += 2)
					{
						if (innocent3[k].pos[h].pX == (a + i) && innocent3[k].pos[h].pY == (b + (i / 2)))
							count++;
					}
				}
				if (count > 0)
				{
					memset(&innocent3[k], -1, sizeof(struct innocent3Club));
					count = 0;
					break;
				}
				count = 0;// reset
			}

	}
	blackCount = 0;
	whiteCount = 0;

	if (blackCheck[a - 2][b - 1] == 1) // �»�밢�� �Ѱ� ���� ��
	{
		for (int i = 2; i < 10; i += 2) // 4ĭ �������� �Ͼ�3 ��0���� üũ
		{
			if (blackCheck[a - i][b - (i / 2)] == 1)
				blackCount++;
			if (whiteCheck[a - i][b - (i / 2)] == 1)
				whiteCount++;
		}
		if (blackCount == 3 && whiteCount == 0)
			for (int k = 0; k < 100; k++)
			{
				for (int h = 0; h < 3; h++)
				{
					for (int i = 2; i < 10; i += 2)
					{
						if (innocent3[k].pos[h].pX == (a - i) && innocent3[k].pos[h].pY == (b - (i / 2)))
							count++;
					}
				}
				if (count > 0)
				{
					memset(&innocent3[k], -1, sizeof(struct innocent3Club));
					count = 0;
					break;
				}
				count = 0;// reset
			}

	}
	blackCount = 0;
	whiteCount = 0;

	if (blackCheck[a - 2][b + 1] == 1) // ���ϴ밢�� �Ѱ� ���� ��
	{
		for (int i = 2; i < 10; i += 2) // 4ĭ �������� �Ͼ�3 ��0���� üũ
		{
			if (blackCheck[a - i][b + (i / 2)] == 1)
				blackCount++;
			if (whiteCheck[a - i][b + (i / 2)] == 1)
				whiteCount++;
		}
		if (blackCount == 3 && whiteCount == 0)
			for (int k = 0; k < 100; k++)
			{
				for (int h = 0; h < 3; h++)
				{
					for (int i = 2; i < 10; i += 2)
					{
						if (innocent3[k].pos[h].pX == (a - i) && innocent3[k].pos[h].pY == (b + (i / 2)))
							count++;
					}
				}
				if (count > 0)
				{
					memset(&innocent3[k], -1, sizeof(struct innocent3Club));
					count = 0;
					break;
				}
				count = 0;// reset
			}

	}
	blackCount = 0;
	whiteCount = 0;

	if (blackCheck[a + 2][b - 1] == 1) // ���밢�� �Ѱ� ���� ��
	{
		for (int i = 2; i < 10; i += 2) // 4ĭ �������� �Ͼ�3 ��0���� üũ
		{
			if (blackCheck[a + i][b - (i / 2)] == 1)
				blackCount++;
			if (whiteCheck[a + i][b - (i / 2)] == 1)
				whiteCount++;
		}
		if (blackCount == 3 && whiteCount == 0)
			for (int k = 0; k < 100; k++)
			{
				for (int h = 0; h < 3; h++)
				{
					for (int i = 2; i < 10; i += 2)
					{
						if (innocent3[k].pos[h].pX == (a + i) && innocent3[k].pos[h].pY == (b - (i / 2)))
							count++;
					}
				}
				if (count > 0)
				{
					memset(&innocent3[k], -1, sizeof(struct innocent3Club));
					count = 0;
					break;
				}
				count = 0;// reset
			}

	}
	blackCount = 0;
	whiteCount = 0;
	 // ===========���� ������ ��============

	} // ==================exileWhite ��================== //���� �Ϸ�

	//�� ���� ��� ������ �ֺ� ������ üũ
	/*void allBlackExile3(void)
	{
		for (int i = 0; i < 100; i++)
		{
			for (int j = 0; j < 100; j++)
			{
				if (blackCheck[i][j] == 1)
					blackExile3(i, j);
			}
		}
	}*/

	//====================Black Exile============================
	 // ���� �� ������ �ֺ� ����3 ������ üũ
	void blackExile3(void)
{
	count = 0;
	blackCount = 0;
	whiteCount = 0;
	// =====���ʿ� �ΰ� ���� ��====
	for (int i = -2; i < 3; i += 2)
	{
		for (int j = -1; j < 2; j++)
		{
			if (blackCheck[x + i][y + j] == 1 && blackCheck[x - i][y - j] == 1)
			{
				for (int k = 0; k < 100; k++)
				{
					for (int h = 0; h < 3; h++)
					{
						if (innocent3[k].pos[h].pX == (x + i) && innocent3[k].pos[h].pY == (y + j))
							count++;
						if (innocent3[k].pos[h].pX == (x - i) && innocent3[k].pos[h].pY == (y - j))
							count++;
					}
					if (count == 2)
					{
						memset(&innocent3[k], -1, sizeof(struct innocent3Club));
						count = 0;
						break;
					}
					count = 0;// reset
				}
			}
		}
	} // ========�� ���� �ΰ� ���� �� ��=========

	// ===========������ �Ͼᵹ üũ========
	//������ ����
	for (int i = 2; i < 10; i += 2) // 4ĭ �������� �Ͼ�3 ��0���� üũ
	{
		if (blackCheck[x + i][y] == 1)
			blackCount++;
		if (whiteCheck[x + i][y] == 1)
			whiteCount++;
	}
	if (blackCount == 3 && whiteCount == 0)
		for (int k = 0; k < 100; k++)
		{
			for (int h = 0; h < 3; h++)
			{
				for (int i = 2; i < 10; i += 2)
				{
					if (innocent3[k].pos[h].pX == (x + i) && innocent3[k].pos[h].pY == y)
						count++;
				}
			}
			if (count > 0)
			{
				memset(&innocent3[k], -1, sizeof(struct innocent3Club));
				count = 0;
				break;
			}
			count = 0;// reset
		}
	blackCount = 0;
	whiteCount = 0;

	//���� ����
	for (int i = 2; i < 10; i += 2) // 4ĭ �������� �Ͼ�3 ��0���� üũ
	{
		if (blackCheck[x - i][y] == 1)
			blackCount++;
		if (whiteCheck[x - i][y] == 1)
			whiteCount++;
	}
	if (blackCount == 3 && whiteCount == 0)
		for (int k = 0; k < 100; k++)
		{
			for (int h = 0; h < 3; h++)
			{
				for (int i = 2; i < 10; i += 2)
				{
					if (innocent3[k].pos[h].pX == (x - i) && innocent3[k].pos[h].pY == y)
						count++;
				}
			}
			if (count > 0)
			{
				memset(&innocent3[k], -1, sizeof(struct innocent3Club));
				count = 0;
				break;
			}
			count = 0;// reset
		}
	blackCount = 0;
	whiteCount = 0;

	//���� ���� 
		for (int i = 1; i < 5; i++) // 4ĭ �������� �Ͼ�3 ��0���� üũ
		{
			if (blackCheck[x][y - i] == 1)
				blackCount++;
			if (whiteCheck[x][y - i] == 1)
				whiteCount++;
		}
		if (blackCount == 3 && whiteCount == 0)
			for (int k = 0; k < 100; k++)
			{
				for (int h = 0; h < 3; h++)
				{
					for (int i = 1; i < 5; i++)
					{
						if (innocent3[k].pos[h].pX == (x) && innocent3[k].pos[h].pY == (y - i))
							count++;
					}
				}
				if (count > 0)
				{
					memset(&innocent3[k], -1, sizeof(struct innocent3Club));
					count = 0;
					break;
				}
				count = 0;// reset
			}
		blackCount = 0;
		whiteCount = 0;

	

	//�Ʒ��� ����
		for (int i = 1; i < 5; i++) // 4ĭ �������� �Ͼ�3 ��0���� üũ
		{
			if (blackCheck[x][y + i] == 1)
				blackCount++;
			if (whiteCheck[x][y + i] == 1)
				whiteCount++;
		}
		if (blackCount == 3 && whiteCount == 0)
			for (int k = 0; k < 100; k++)
			{
				for (int h = 0; h < 3; h++)
				{
					for (int i = 1; i < 5; i++)
					{
						if (innocent3[k].pos[h].pX == (x) && innocent3[k].pos[h].pY == (y + i))
							count++;
					}
				}
				if (count > 0)
				{
					memset(&innocent3[k], -1, sizeof(struct innocent3Club));
					count = 0;
					break;
				}
				count = 0;// reset
			}
		blackCount = 0;
		whiteCount = 0;

	

	//���ϴ밢��
		for (int i = 2; i < 10; i += 2) // 4ĭ �������� �Ͼ�3 ��0���� üũ
		{
			if (blackCheck[x + i][y + (i / 2)] == 1)
				blackCount++;
			if (whiteCheck[x + i][y + (i / 2)] == 1)
				whiteCount++;
		}
		if (blackCount == 3 && whiteCount == 0)
			for (int k = 0; k < 100; k++)
			{
				for (int h = 0; h < 3; h++)
				{
					for (int i = 2; i < 10; i += 2)
					{
						if (innocent3[k].pos[h].pX == (x + i) && innocent3[k].pos[h].pY == (y + (i / 2)))
							count++;
					}
				}
				if (count > 0)
				{
					memset(&innocent3[k], -1, sizeof(struct innocent3Club));
					count = 0;
					break;
				}
				count = 0;// reset
			}
		blackCount = 0;
		whiteCount = 0;

	

	//�»�밢��
		for (int i = 2; i < 10; i += 2) // 4ĭ �������� �Ͼ�3 ��0���� üũ
		{
			if (blackCheck[x - i][y - (i / 2)] == 1)
				blackCount++;
			if (whiteCheck[x - i][y - (i / 2)] == 1)
				whiteCount++;
		}
		if (blackCount == 3 && whiteCount == 0)
			for (int k = 0; k < 100; k++)
			{
				for (int h = 0; h < 3; h++)
				{
					for (int i = 2; i < 10; i += 2)
					{
						if (innocent3[k].pos[h].pX == (x - i) && innocent3[k].pos[h].pY == (y - (i / 2)))
							count++;
					}
				}
				if (count > 0)
				{
					memset(&innocent3[k], -1, sizeof(struct innocent3Club));
					count = 0;
					break;
				}
				count = 0;// reset
			}
		blackCount = 0;
		whiteCount = 0;

	

	//���ϴ밢��
		for (int i = 2; i < 10; i += 2) // 4ĭ �������� �Ͼ�3 ��0���� üũ
		{
			if (blackCheck[x - i][y + (i / 2)] == 1)
				blackCount++;
			if (whiteCheck[x - i][y + (i / 2)] == 1)
				whiteCount++;
		}
		if (blackCount == 3 && whiteCount == 0)
			for (int k = 0; k < 100; k++)
			{
				for (int h = 0; h < 3; h++)
				{
					for (int i = 2; i < 10; i += 2)
					{
						if (innocent3[k].pos[h].pX == (x - i) && innocent3[k].pos[h].pY == (y + (i / 2)))
							count++;
					}
				}
				if (count > 0)
				{
					memset(&innocent3[k], -1, sizeof(struct innocent3Club));
					count = 0;
					break;
				}
				count = 0;// reset
			}
		blackCount = 0;
		whiteCount = 0;

	

	//���ϴ밢��
		for (int i = 2; i < 10; i += 2) // 4ĭ �������� �Ͼ�3 ��0���� üũ
		{
			if (blackCheck[x + i][y - (i / 2)] == 1)
				blackCount++;
			if (whiteCheck[x + i][y - (i / 2)] == 1)
				whiteCount++;
		}
		if (blackCount == 3 && whiteCount == 0)
			for (int k = 0; k < 100; k++)
			{
				for (int h = 0; h < 3; h++)
				{
					for (int i = 2; i < 10; i += 2)
					{
						if (innocent3[k].pos[h].pX == (x + i) && innocent3[k].pos[h].pY == (y - (i / 2)))
							count++;
					}
				}
				if (count > 0)
				{
					memset(&innocent3[k], -1, sizeof(struct innocent3Club));
					count = 0;
					break;
				}
				count = 0;// reset
			}
		blackCount = 0;
		whiteCount = 0;

	
	

} // whiteExile3 ��

//void black44(void)
//{
//	blackCheck[x][y] = 1;
//	whiteCount = 0;
//	blackCount = 0;
//	count = 0;
//
//
//
//}