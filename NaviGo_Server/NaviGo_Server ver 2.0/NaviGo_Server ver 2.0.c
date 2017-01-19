#include <stdio.h>		
#include <winsock2.h>										// 입출력
#include <Windows.h>										// 커서 이동
#include <conio.h>											// getch함수 (키를 하나 받는 함수)
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
int posNum = 0; // 매번 초기화해주어야
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




//가로로 갈 때는 0, 2, 4, ... 36 짝수 (x)
//세로로 갈 때는 0, 1, 2... 18 (y)
//메인
int main(int argc, char **argv)
{
	memset(innocent3, -1, sizeof(struct innocent3Club) * 100);
	FlushConsoleInputBuffer(hIn);
	//=================================소켓 관련=========================================
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

	// 서버 소켓 생성
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

	// 소켓에 주소 할당
	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("bind() error");
	}
	else
		puts("Socket Binding...Success");

	// 연결 요청 대기 상태
	if (listen(hServSock, 5) == SOCKET_ERROR)
	{
		ErrorHandling("listen() error");
	}
	else
		puts("Listening to Client...Success");

	// 연결 요청 수락
	szClntAddr = sizeof(clntAddr);
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
	if (hClntSock == INVALID_SOCKET)
	{
		ErrorHandling("accept() error");
	}
	else
		puts("Accepting Client...Success");

	//=================================소켓 관련=========================================
	system("cls");

	PlaySound(TEXT("Crazy Arcade Login Sound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);

	//커서 지우기
	CursorVis();

	// 오프닝
	startScreen();

	//게임서버 접속
	if (server == 1)
	{
		//로딩
		loading();

		//게임  시작
		while (gameReboot)
		{
			gameStart();
		}
	}

	return 0;
} // 메인 끝

//로딩화면
void loading(void)
{
	gotoxy(33, 8);
	txtColor(6);
	puts("● Now Loading♪\n\n\n\n\n\n\n\n\n\n                       COPYRIGHT 2016 Joon Kim at Naviworks®");
	Sleep(650);
	gotoxy(33, 8);
	txtColor(10);
	puts("◑ Now Loading  ♪ \n\n\n\n\n\n\n\n\n\n                       COPYRIGHT 2016 Joon Kim at Naviworks®");
	Sleep(650);
	gotoxy(33, 8);
	txtColor(14);
	puts("○ Now Loading    ♪ \n\n\n\n\n\n\n\n\n\n                       COPYRIGHT 2016 Joon Kim at Naviworks®");
	Sleep(650);
	system("cls");
}

//게임 시작 누를 시
void gameStart(void)
{
	if (gameBreaker == FALSE && gameReboot == TRUE)
		enterName();

	gameBreaker = FALSE;
	gameReboot = FALSE;

	gotoxy(33, 8);
	txtColor(6);
	puts("● Now Loading♪\n\n\n\n\n\n                                   Please Wait....");
	Sleep(700);
	gotoxy(33, 8);
	txtColor(10);
	puts("◑ Now Loading  ♪ \n\n\n\n\n\n                                   Please Wait....");
	Sleep(700);
	gotoxy(33, 8);
	txtColor(14);
	puts("○ Now Loading    ♪ \n\n\n\n\n\n                                   Please Wait....");
	Sleep(700);
	system("cls");


	//바둑판 세팅
	setPan();
	//게임 설명
	intro();

	//게임 진행
	gameProcess();

	return;
}

//이름 입력
void enterName(void)
{
	txtColor(14);
	gotoxy(20, 8);
	puts("사용하실 이름이나 닉네임을 입력해주세요~");
	gotoxy(20, 11);
	txtColor(15);
	printf("참가자 : ");
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
		puts("\n\n   이제 최신식 로또 추첨기를 돌려 누가 선공을 할지 정할 겁니다.\n   0부터 99중에 더 높은 숫자를 뽑으신 분이 백돌로 선공을 할 것입니다. ");
		txtColor(15);
		printf("\n\n   %s님 Enter를 눌러 로또 추첨기계를 돌려주세요~\n", servName);
		getch();
		randomize();
		p1diceNum = rand()%100;

		//로또 돌리기
		txtColor(10);
		gotoxy(33, 15);
		printf("● ");
		txtColor(15);
		printf("Rolling.");
		Sleep(300);
		txtColor(11);
		gotoxy(33, 15);
		printf("◑ ");
		txtColor(15);
		printf("Rolling . ");
		Sleep(300);
		txtColor(12);
		gotoxy(33, 15);
		printf("○ ");
		txtColor(15);
		printf("Rolling  .");
		Sleep(300);
		txtColor(13);
		gotoxy(33, 15);
		printf("◑ ");
		txtColor(15);
		printf("Rolling   . ");
		Sleep(300);
		txtColor(14);
		gotoxy(33, 15);
		printf("● ");
		txtColor(15);
		printf("Rolling    . ");
		Sleep(300);

		system("cls");
		txtColor(14);
		puts("\n\n   이제 최신식 로또 추첨기를 돌려 누가 선공을 할지 정할 겁니다.\n   0부터 99중에 더 높은 숫자를 뽑으신 분이 백돌로 선공을 할 것입니다. ");

		txtColor(15);
		printf("\n   %s님의 숫자는 ", servName);
		txtColor(14);
		printf("%d", p1diceNum);
		txtColor(15);
		printf("입니다\n");

		//server p1점수 보내기/ client 점수 받기
		servScore = p1diceNum;
		send(hClntSock, (char*)&servScore, sizeof(servScore), 0);

		gotoxy(20, 15);
		puts("Waiting for other player....");

		recv(hClntSock, (char*)&clntScore, sizeof(clntScore), 0);


		system("cls");
		txtColor(14);
		puts("\n\n   이제 최신식 로또 추첨기를 돌려 누가 선공을 할지 정할 겁니다.\n   0부터 99중에 더 높은 숫자를 뽑으신 분이 백돌로 선공을 할 것입니다. ");

		txtColor(15);
		printf("\n   %s님의 숫자는 ", servName);
		txtColor(14);
		printf("%d", p1diceNum);
		txtColor(15);
		printf("입니다\n");


		txtColor(15);
		printf("\n   %s님의 숫자는 ", clntName);
		txtColor(14);
		printf("%d", clntScore);
		txtColor(15);
		printf("입니다\n");

		Sleep(2000);

		if (servScore > clntScore)
		{
			printf("\n\n\n\n                               Nice~ %s님의 선공입니다!!", servName);
			first = 1;
			break;
		}
		else if (clntScore > servScore)
		{
			printf("\n\n\n\n                               %s님의 선공입니다 ㅠㅠ", clntName);
			first = 2;
			break;
		}
		if (servScore == clntScore)
		{
			printf("\n\n\n\n                                비겼습니다. 다시 돌려주세요.");
			Sleep(2000);
		}
	}
	
	Sleep(3300);
	system("cls");
}

//게임 설명
void intro(void)
{
	gotoxy(43, 2);
	txtColor(13);
	printf("★5개의 오목 돌을 연속으로 ");
	gotoxy(43, 3);
	printf("가로나 세로, 또는 대각선 방향으로 ");
	gotoxy(43, 4);
	printf("놓아야 승리합니다★");
}

// 바둑판 만들기
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
					printf("┏");
				else if (j == ARR_SIZE_C - 1)
					printf("┓\n");
				else
					printf("┳");
			}
			else if (i != 1 && i != ARR_SIZE_R)
			{
				if (j == 0)
					printf("┣");
				else if (j == ARR_SIZE_C - 1)
					printf("┫\n");
				else
					printf("╋");
			}
			else if (i == ARR_SIZE_R)
			{
				if (j == 0)
					printf("┗");
				else if (j == ARR_SIZE_C - 1)
					printf("┛\n");
				else
					printf("┻");
			}
		}
	}
} // 바둑판 만들기 끝


//마우스 클릭 함수``
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
	
	// 게임 본격적으로 시작
	PlaySound(TEXT("게임 진행 - 삼국지 7 Summer OST.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NODEFAULT);
	// check 배열 초기화(중복되면 안되니)
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

	//게임이 중단되지 않을 경우 계속 돌리기
	while (gameBreaker == FALSE)
	{
		if (first == 1)
		{
			while (gameBreaker == FALSE)
			{
				 //먼저 두기
				txtColor(15);
				gotoxy(43, 7);
				printf("%s님의 차례입니다", servName);

				 //다시시도 했을 경우, 아닐 시 스킵
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
				//	retry 초기화
				retry = 0;
				



				diffPlace = FALSE;

				while (diffPlace == FALSE)
				{
					//클릭
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
							
							//흑돌 좌표값 보내주기
							sendPos[0] = x;
							sendPos[1] = y;
							send(hClntSock, (char*)&sendPos, sizeof(sendPos), 0);
							if (error == 1)
							{
								gg33 = 1;
								send(hClntSock, (char*)&gg33, sizeof(gg33), 0);
								printLose();
								msgID = MessageBoxA(NULL, "※※※  3-3 금수로 흑돌이 졌습니다!  ※※※", "룰을 익히세요!", MB_RETRYCANCEL);

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
				} //같은 곳에 못 두게 함

				if (retry == 0)
				{
					txtColor(15);
				gotoxy(43, 7);
				printf("%s님의 차례입니다", clntName);

				// 백돌 좌표값 받고 저장, 출력
				recv(hClntSock, (char*)&recPos, sizeof(recPos), 0);
				x = recPos[0];
				y = recPos[1];
				whiteExile3(x, y);
				gotoxy(x, y);
				txtColor(15 | 96);
				printf("●");
				whiteCheck[x][y] = 1;
			//	check if White wins
				CheckWhiteWins();
				}
			
			} // blackturn while 끝
		} // if first = 1 끝

		else if (first == 2)
		{
			while (gameBreaker == FALSE)
			{
				// Client 차례
				txtColor(15);
				gotoxy(43, 7);
				printf("%s님의 차례입니다", clntName);

				//다시시도시 실행, 아닐 시 스킵
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
				//retry 다시 초기화
				retry = 0;
			


				// 흑돌 좌표값 받고 저장, 출력
				recv(hClntSock, (char*)&recPos, sizeof(recPos), 0);
				
				x = recPos[0];
				y = recPos[1]; 
				gotoxy(x, y);
				txtColor(0 | 96);
				printf("●");
				blackCheck[x][y] = 1;
				recv(hClntSock, (char*)&gg33, sizeof(gg33), 0);
				if (gg33 == 1)
				{
					printWin();
					msgID = MessageBoxA(NULL, "☆☆☆  백돌이 이겼습니다!  ☆☆☆", "축하합니다!", MB_RETRYCANCEL);

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
					//서버 차례
					txtColor(15);
					gotoxy(43, 7);
					printf("%s님의 차례입니다", servName);

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
								printf("●");
								whiteCheck[x][y] = 1;

								sendPos[0] = x;
								sendPos[1] = y;
								send(hClntSock, (char*)&sendPos, sizeof(sendPos), 0);
								diffPlace = TRUE;
								//check if Black wins
								CheckWhiteWins();
							}
						}
					} // 같은 곳에 못 두게 하기
				}

			}// blackturn while 끝
		} // if first = 2 끝
	} // while 끝, 게임이 중단되지 않을 경우 계속 돌리기

	return;
}

//하얀색이 이겼는지 판별
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

//검은색이 이겼는지 판별
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
					msgID = MessageBoxA(NULL, "※※※  6목 금수입니다! 흑돌이 졌습니다! ※※※", "룰을 익히세요!", MB_RETRYCANCEL);
				}
				else
				{
					printWin();
					msgID = MessageBoxA(NULL, "☆☆☆  상대가 6목 금수를 뒀습니다! 백돌이 이겼습니다!☆☆☆", "축하합니다!", MB_RETRYCANCEL);
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
		
		
	
//색 변환
void txtColor(int color_number)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_number);
}


//커서 존재여부
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
	msgID = MessageBoxA(NULL, "☆☆☆  백돌이 이겼습니다!  ☆☆☆", "축하합니다!", MB_RETRYCANCEL);
	else
		msgID = MessageBoxA(NULL, "※※※  흑돌이 졌습니다!  ※※※", "분발 하세요!", MB_RETRYCANCEL);

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
			msgID = MessageBoxA(NULL, "★★★  흑돌이 이겼습니다! ★★★", "축하합니다!", MB_RETRYCANCEL);
		else
			msgID = MessageBoxA(NULL, "※※※  백돌이 졌습니다!  ※※※", "분발 하세요!", MB_RETRYCANCEL);
			
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

//오프닝
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
	printf("▩     ▩      ▩    ▩        ▩   ▩▩▩   ▩▩▩      ▩▤▩   ™\n");
	txtColor(14);
	gotoxy(7, 6);
	printf("▥ ▧  ▥    ▨  ▧   ▧      ▨      ▥    ▨         ▨      ▧\n");
	txtColor(14);
	gotoxy(7, 7);
	printf("▥  ▧ ▥   ▨ ★ ▧   ▧    ▨       ▥   ▥  ▤▤▤  ▥      ▥\n");
	txtColor(14);
	gotoxy(7, 8);
	printf("▥   ▧▥  ▨      ▧   ▧  ▨        ▥    ▧     ▥  ▧      ▨\n");
	txtColor(14);
	gotoxy(7, 9);
	printf("▩     ▩ ▩        ▩   ▩▩       ▩▩▩   ▩▩▩▩    ▩▤▩\n");

	gotoxy(37, 13);
	txtColor(13);
	puts("Ver 1.0");
	gotoxy(33, 14);
	txtColor(3);
	puts("Made by Joon Kim");



	txtColor(10);
	gotoxy(6, 19);
	puts("▶게임 시작                   게임 방법                   크레딧");
	// spacebar  눌리기 전까지 왔다 갔다 가능
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
		printf("★5개의 오목 돌을 연속으로 가로나 세로, \n");
		gotoxy(22, 7);
		printf("또는 대각선 방향으로 놓아야 승리~\n");
		gotoxy(32, 8);
		printf("자 달리자고~★");
		txtColor(10);
		gotoxy(37, 17);
		puts("▷BACK");

		typed = _getch();
		if (typed == 80);
		{
			gotoxy(37, 17);
			puts("▶BACK");
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
		puts("\n\n                            ♣♧♣   CREDITS   ♣♧♣ \n\n\n ★ HEE HUN PARK 선임연구원님\n\n ★ TAE GEUN KIM 선임연구원님\n\n ★ JOHN UHM 연구원님\n\n ★ JI WON KIM 연구원님\n\n ★ NAVIWORKS NATIONAL DEFENSE TECHNOLOGY DEVELOPMENT DIVISION(MARINE SAFETY)\n\n COPYRIGHT 2016 JOON KIM NAVIWORKS");
		txtColor(10);
		gotoxy(37, 17);
		puts("▷BACK");

		typed = _getch();
		if (typed == 80);
		{
			gotoxy(37, 17);
			puts("▶BACK");
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
	case 75:												// key값의 ASCII코드가 75번일 때 (왼쪽 방향키)
		if (server == 2)
		{
			gotoxy(6, 19);
			puts("▶게임 시작                   게임 방법                   크레딧");
			server--;
		}
		else if (server == 3)
		{
			gotoxy(6, 19);
			puts("  게임 시작                 ▶게임 방법                   크레딧");
			server--;
		}
		break;
	case 77:												// key값의 ASCII코드가 77번일 때 (오른쪽 방향키)
		if (server == 1)
		{
			gotoxy(6, 19);
			puts("  게임 시작                 ▶게임 방법                   크레딧");
			server++;
		}
		else if (server == 2)
		{
			gotoxy(6, 19);
			puts("  게임 시작                   게임 방법                 ▶크레딧");
			server++;
		}
		break;
	case 32:												// key값의 ASCII코드가 32번일 때 (스페이스바)
		spaceSelect = TRUE;
		break;
	}


}

//이겼을 경우
void printWin(void)
{
	PlaySound(TEXT("victory sound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
	txtColor(14);
	gotoxy(49, 12);
	printf("%s님이 승리하셨습니다!", servName);
	txtColor(11);
	gotoxy(40, 13);
	puts(" ●          ●  ●●●  ●      ●  ▥");
	Sleep(80);
	txtColor(12);
	gotoxy(40, 14);
	puts("  ●   ★   ●     ●    ●  ●  ●  ▥");
	Sleep(80);
	txtColor(13);
	gotoxy(40, 15);
	puts("   ● ●● ●      ●    ●   ● ●  ▥");
	Sleep(80);
	txtColor(14);
	gotoxy(40, 16);
	puts("    ●    ●     ●●●  ●      ●  ▦");
	Sleep(80);
	txtColor(14);
	gotoxy(0, 22);
	
}
//졌을 경우
void printLose(void)
{
	PlaySound(TEXT("Wha-Wha.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
	txtColor(14);
	gotoxy(49, 12);
	printf("%s님이 지셨습니다 ㅠㅠ", servName);
	txtColor(8);
	gotoxy(40, 13);
	puts(" ●       ● ●       ● ●   ●●●   ");
	Sleep(80);
	gotoxy(40, 14);
	puts(" ●     ●     ●   ● ●     ●●●   ");
	Sleep(80);
	gotoxy(40, 15);
	puts(" ●     ●     ●      ● ●  ●      ");
	Sleep(80);
	gotoxy(40, 16);
	puts(" ●●●   ● ●      ● ●    ●●●  ");
	Sleep(80);
	gotoxy(0, 22);
	

}
 // 검은돌 33체크
void black33(void)
{
	//일단 둔 곳도 1이라 하고 시작
	blackCheck[x][y] = 1;
	whiteCount = 0;
	blackCount = 0;
	error = 0;
	BOOL overlap = FALSE;
	tempCount = 0;

	
	memset(temp3, -1, sizeof(struct temp3Club) * 10);

	
	
	// // ==========오른쪽 가로 시작 ===========
	for (int i = 0; i < 8; i++)
	{
		if (blackCheck[x + i][y] == 1) // 백색 있으면 카운트
			blackCount++;
		if (whiteCheck[x + i][y] == 1) // 흑색 있으면 카운트
			whiteCount++;
	}
	if (blackCheck[x + 8][y] == 1) // 끝에 백돌이 있으면
		if (blackCount == 4) // 안쪽 돌 4개 포함 5개 있으면 승리
			return;
	if (blackCount == 3 && whiteCount == 0)
		if (!(blackCheck[x + 2][y] == 1 && blackCheck[x + 4][y] == 1 && whiteCheck[x-4][y] == 0)) // 안쪽에서 검사해줄 순수3 빼기
		if (whiteCheck[x-2][y] == 0 && blackCheck[x-2][y] == 0) // 높은 곳 바로 뒤에 흑이나 백있으면 순수 안 됌
		if (whiteCheck[x + 8][y] == 0 || (whiteCheck[x + 8][y] == 1 && whiteCheck[x + 6][y] == 0 && blackCheck[x + 6][y] == 0))// 순수 3이면
			{
				tempCount++; // 임시3 인 것들 세기
				for (int k = 0; k < 10; k++)
				{
					if (temp3[k].pos[0].pX == -1)
					{
						for (int i = 0; i < 8; i++)
						{
							if (blackCheck[x + i][y] == 1) // 백색 있으면 카운트
							{
								temp3[k].pos[posNum].pX = x+i;
								temp3[k].pos[posNum].pY = y;
								posNum++;
							}
							
						}
						posNum = 0; // posNum 초기화
						break; // 빈 곳 찾아서 위치값 저장했으면 다른 빈 곳 또 들가지 말고 브레이크
					} // 빈 곳 찾기 끝
				} //빈곳 찾기 끝
			} // 순수 3 체크 & 임시가입 끝


	blackCount = 0;
	whiteCount = 0;
	//================오른쪽 가로 끝 =========

	// =============왼쪽 가로 시작 ===========
	for (int i = 0; i < 8; i++)
	{
		if (blackCheck[x - i][y] == 1) // 백색 있으면 카운트
			blackCount++;
		if (whiteCheck[x - i][y] == 1) // 흑색 있으면 카운트
			whiteCount++;
	}
	if (blackCheck[x - 8][y] == 1) // 끝에 백돌이 있으면
		if (blackCount == 4) // 안쪽 돌 4개 포함 5개 있으면 승리
			return;
	if (blackCount == 3 && whiteCount == 0)
		if (!(blackCheck[x - 2][y] == 1 && blackCheck[x - 4][y] == 1 && whiteCheck[x + 4][y] == 0)) // 안쪽에서 검사해줄 순수3 빼기
		if (whiteCheck[x + 2][y] == 0 && blackCheck[x + 2][y] == 0) // 높은 곳 바로 뒤에 흑이나 백있으면 순수 안 됌
			if (whiteCheck[x - 8][y] == 0 || (whiteCheck[x - 8][y] == 1 && whiteCheck[x - 6][y] == 0 && blackCheck[x - 6][y] == 0))// 순수 3이면
			{
				tempCount++; // 임시3 인 것들 세기
				for (int k = 0; k < 10; k++)
				{
					if (temp3[k].pos[0].pX == -1)
					{
						for (int i = 0; i < 8; i++)
						{
							if (blackCheck[x - i][y] == 1) // 백색 있으면 카운트
							{
								temp3[k].pos[posNum].pX = x-i;
								temp3[k].pos[posNum].pY = y;
								posNum++;
							}

						}
						posNum = 0; // posNum 초기화
						break; // 빈 곳 찾아서 위치값 저장했으면 다른 빈 곳 또 들가지 말고 브레이크
					} // 빈 곳 찾기 끝
				} //빈곳 찾기 끝
			} // 순수 3 체크 & 임시가입 끝

	blackCount = 0;
	whiteCount = 0;

	// ============아래세로 시작=============
	for (int i = 0; i < 4; i++)
	{
		if (blackCheck[x][y + i] == 1) // 백색 있으면 카운트
			blackCount++;
		if (whiteCheck[x][y + i] == 1) // 흑색 있으면 카운트
			whiteCount++;
	}
	if (blackCheck[x][y + 4] == 1) // 끝에 백돌이 있으면
		if (blackCount == 4) // 안쪽 돌 4개 포함 5개 있으면 승리
			return;
	if (blackCount == 3 && whiteCount == 0)
		if (!(blackCheck[x][y + 1] == 1 && blackCheck[x][y + 2] == 1 && whiteCheck[x][y - 2] == 0)) // 안쪽에서 검사해줄 순수3 빼기
		if (whiteCheck[x][y - 1] == 0 && blackCheck[x][y - 1] == 0) // 높은 곳 바로 뒤에 흑이나 백있으면 순수 안 됌
			if (whiteCheck[x][y+4] == 0 || (whiteCheck[x][y+4] == 1 && whiteCheck[x][y+3] == 0 && blackCheck[x][y+3] == 0))// 순수 3이면
			{
				tempCount++; // 임시3 인 것들 세기
				for (int k = 0; k < 10; k++)
				{
					if (temp3[k].pos[0].pX == -1)
					{
						for (int i = 0; i < 4; i++)
						{
							if (blackCheck[x ][y+i] == 1) // 백색 있으면 카운트
							{
								temp3[k].pos[posNum].pX = x;
								temp3[k].pos[posNum].pY = y + i;
								posNum++;
							}

						}
						posNum = 0; // posNum 초기화
						break; // 빈 곳 찾아서 위치값 저장했으면 다른 빈 곳 또 들가지 말고 브레이크
					} // 빈 곳 찾기 끝
				} //빈곳 찾기 끝
			} // 순수 3 체크 & 임시가입 끝
	blackCount = 0;
	whiteCount = 0;
	// ================ 아래 세로 끝 ====

	// =============위로 세로 시작 =====
	for (int i = 0; i < 4; i++)
	{
		if (blackCheck[x][y - i] == 1) // 백색 있으면 카운트
			blackCount++;
		if (whiteCheck[x][y - i] == 1) // 흑색 있으면 카운트
			whiteCount++;
	}
	if (blackCheck[x][y - 4] == 1) // 끝에 백돌이 있으면
		if (blackCount == 4) // 안쪽 돌 4개 포함 5개 있으면 승리
			return;
	if (blackCount == 3 && whiteCount == 0)
		if (!(blackCheck[x][y - 1] == 1 && blackCheck[x][y - 2] == 1 && whiteCheck[x][y + 2] == 0)) // 안쪽에서 검사해줄 순수3 빼기
		if (whiteCheck[x][y + 1] == 0 && blackCheck[x][y + 1] == 0) // 높은 곳 바로 뒤에 흑이나 백있으면 순수 안 됌
			if (whiteCheck[x][y - 4] == 0 || (whiteCheck[x][y - 4] == 1 && whiteCheck[x][y - 3] == 0 && blackCheck[x][y - 3] == 0))// 순수 3이면
			{
				tempCount++; // 임시3 인 것들 세기
				for (int k = 0; k < 10; k++)
				{
					if (temp3[k].pos[0].pX == -1)
					{
						for (int i = 0; i < 4; i++)
						{
							if (blackCheck[x][y - i] == 1) // 백색 있으면 카운트
							{
								temp3[k].pos[posNum].pX = x;
								temp3[k].pos[posNum].pY = y - i;
								posNum++;
							}

						}
						posNum = 0; // posNum 초기화
						break; // 빈 곳 찾아서 위치값 저장했으면 다른 빈 곳 또 들가지 말고 브레이크
					} // 빈 곳 찾기 끝
				} //빈곳 찾기 끝
			} // 순수 3 체크 & 임시가입 끝
	blackCount = 0;
	whiteCount = 0;
	//========위로 세로 끝===========

	// ========우하 대각선 시작========
	for (int i = 0; i < 8; i += 2)
	{
		if (blackCheck[x + i][y + (i / 2)] == 1) // 백색 있으면 카운트
			blackCount++;
		if (whiteCheck[x + i][y + (i / 2)] == 1) // 흑색 있으면 카운트
			whiteCount++;
	}
	if (blackCheck[x + 8][y + 4] == 1) // 끝에 백돌이 있으면
		if (blackCount == 4) // 안쪽 돌 4개 포함 5개 있으면 승리
			return;
	if (blackCount == 3 && whiteCount == 0)
		if (!(blackCheck[x + 2][y + 1] == 1 && blackCheck[x + 4][y + 2] == 1 && whiteCheck[x-4][y - 2] == 0)) // 안쪽에서 검사해줄 순수3 빼기
		if (whiteCheck[x - 2][y - 1] == 0 && blackCheck[x-2][y - 1] == 0) // 높은 곳 바로 뒤에 흑이나 백있으면 순수 안 됌
			if (whiteCheck[x+8][y + 4] == 0 || (whiteCheck[x+8][y + 4] == 1 && whiteCheck[x+6][y + 3] == 0 && blackCheck[x+6][y + 3] == 0))// 순수 3이면
			{
				tempCount++; // 임시3 인 것들 세기
				for (int k = 0; k < 10; k++)
				{
					if (temp3[k].pos[0].pX == -1)
					{
						for (int i = 0; i < 8; i+=2)
						{
							if (blackCheck[x + i][y + (i / 2)] == 1) // 백색 있으면 카운트
							{
								temp3[k].pos[posNum].pX = x + i;
								temp3[k].pos[posNum].pY = y + (i / 2);
								posNum++;
							}

						}
						posNum = 0; // posNum 초기화
						break; // 빈 곳 찾아서 위치값 저장했으면 다른 빈 곳 또 들가지 말고 브레이크
					} // 빈 곳 찾기 끝
				} //빈곳 찾기 끝
			} // 순수 3 체크 & 임시가입 끝
	blackCount = 0;
	whiteCount = 0;
	//====================우하 대각선 끝===============

	// ==============좌상 대각선 시작==============
	for (int i = 0; i < 8; i += 2)
	{
		if (blackCheck[x - i][y - (i / 2)] == 1) // 백색 있으면 카운트
			blackCount++;
		if (whiteCheck[x - i][y - (i / 2)] == 1) // 흑색 있으면 카운트
			whiteCount++;
	}
	if (blackCheck[x - 8][y - 4] == 1) // 끝에 백돌이 있으면
		if (blackCount == 4) // 안쪽 돌 4개 포함 5개 있으면 승리
			return;
	if (blackCount == 3 && whiteCount == 0)
		if (!(blackCheck[x - 2][y - 1] == 1 && blackCheck[x - 4][y - 2] == 1 && whiteCheck[x + 4][y + 2] == 0)) // 안쪽에서 검사해줄 순수3 빼기
		if (whiteCheck[x + 2][y + 1] == 0 && blackCheck[x + 2][y + 1] == 0) // 높은 곳 바로 뒤에 흑이나 백있으면 순수 안 됌
			if (whiteCheck[x - 8][y - 4] == 0 || (whiteCheck[x - 8][y - 4] == 1 && whiteCheck[x - 6][y - 3] == 0 && blackCheck[x - 6][y - 3] == 0))// 순수 3이면
			{
				tempCount++; // 임시3 인 것들 세기
				for (int k = 0; k < 10; k++)
				{
					if (temp3[k].pos[0].pX == -1)
					{
						for (int i = 0; i < 8; i += 2)
						{
							if (blackCheck[x - i][y - (i / 2)] == 1) // 백색 있으면 카운트
							{
								temp3[k].pos[posNum].pX = x - i;
								temp3[k].pos[posNum].pY = y - (i / 2);
								posNum++;
							}

						}
						posNum = 0; // posNum 초기화
						break; // 빈 곳 찾아서 위치값 저장했으면 다른 빈 곳 또 들가지 말고 브레이크
					} // 빈 곳 찾기 끝
				} //빈곳 찾기 끝
			} // 순수 3 체크 & 임시가입 끝
	blackCount = 0;
	whiteCount = 0;
	// ==========좌상 대각선 끝===========

	// ==============우상 대각선 시작============
	for (int i = 0; i < 8; i += 2)
	{
		if (blackCheck[x + i][y - (i / 2)] == 1) // 백색 있으면 카운트
			blackCount++;
		if (whiteCheck[x + i][y - (i / 2)] == 1) // 흑색 있으면 카운트
			whiteCount++;
	}
	if (blackCheck[x + 8][y - 4] == 1) // 끝에 백돌이 있으면
		if (blackCount == 4) // 안쪽 돌 4개 포함 5개 있으면 승리
			return;
	if (blackCount == 3 && whiteCount == 0)
		if (!(blackCheck[x + 2][y - 1] == 1 && blackCheck[x + 4][y - 2] == 1 && whiteCheck[x - 4][y + 2] == 0)) // 안쪽에서 검사해줄 순수3 빼기
		if (whiteCheck[x - 2][y + 1] == 0 && blackCheck[x - 2][y + 1] == 0) // 높은 곳 바로 뒤에 흑이나 백있으면 순수 안 됌
			if (whiteCheck[x + 8][y - 4] == 0 || (whiteCheck[x+ 8][y - 4] == 1 && whiteCheck[x + 6][y - 3] == 0 && blackCheck[x + 6][y - 3] == 0))// 순수 3이면
			{
				tempCount++; // 임시3 인 것들 세기
				for (int k = 0; k < 10; k++)
				{
					if (temp3[k].pos[0].pX == -1)
					{
						for (int i = 0; i < 8; i += 2)
						{
							if (blackCheck[x + i][y - (i / 2)] == 1) // 백색 있으면 카운트
							{
								temp3[k].pos[posNum].pX = x + i;
								temp3[k].pos[posNum].pY = y - (i / 2);
								posNum++;
							}

						}
						posNum = 0; // posNum 초기화
						break; // 빈 곳 찾아서 위치값 저장했으면 다른 빈 곳 또 들가지 말고 브레이크
					} // 빈 곳 찾기 끝
				} //빈곳 찾기 끝
			} // 순수 3 체크 & 임시가입 끝
	blackCount = 0;
	whiteCount = 0;

	//=================우상 대각선 끝====================

	//=====================좌하 대각선 시작===================
	for (int i = 0; i < 8; i += 2)
	{
		if (blackCheck[x - i][y + (i / 2)] == 1) // 백색 있으면 카운트
			blackCount++;
		if (whiteCheck[x - i][y + (i / 2)] == 1) // 흑색 있으면 카운트
			whiteCount++;
	}
	if (blackCheck[x - 8][y + 4] == 1) // 끝에 백돌이 있으면
		if (blackCount == 4) // 안쪽 돌 4개 포함 5개 있으면 승리
			return;
	if (blackCount == 3 && whiteCount == 0)
		if (!(blackCheck[x - 2][y + 1] == 1 && blackCheck[x - 4][y + 2] == 1 && whiteCheck[x + 4][y - 2] == 0)) // 안쪽에서 검사해줄 순수3 빼기
		if (whiteCheck[x + 2][y - 1] == 0 && blackCheck[x + 2][y - 1] == 0) // 높은 곳 바로 뒤에 흑이나 백있으면 순수 안 됌
			if (whiteCheck[x - 8][y + 4] == 0 || (whiteCheck[x - 8][y + 4] == 1 && whiteCheck[x - 6][y + 3] == 0 && blackCheck[x - 6][y + 3] == 0))// 순수 3이면
			{
				tempCount++; // 임시3 인 것들 세기
				for (int k = 0; k < 10; k++)
				{
					if (temp3[k].pos[0].pX == -1)
					{
						for (int i = 0; i < 8; i += 2)
						{
							if (blackCheck[x - i][y + (i / 2)]) // 백색 있으면 카운트
							{
								temp3[k].pos[posNum].pX = x - i;
								temp3[k].pos[posNum].pY = y + (i / 2);
								posNum++;
							}

						}
						posNum = 0; // posNum 초기화
						break; // 빈 곳 찾아서 위치값 저장했으면 다른 빈 곳 또 들가지 말고 브레이크
					} // 빈 곳 찾기 끝
				} //빈곳 찾기 끝
			} // 순수 3 체크 & 임시가입 끝
	blackCount = 0;
	whiteCount = 0;
	//======================좌하 대각선 끝=========================
	//둔 곳 중심으로 5칸씩 계산 끝

	// 둔 곳 중 심으로 양 옆 조사 시작
	//========양쪽 가로 5칸 조사======
	for (int i = -4; i < 5; i += 2)
	{
		if (blackCheck[x + i][y] == 1) // 백색 있으면 카운트
			blackCount++;
		if (whiteCheck[x + i][y] == 1) // 흑색 있으면 카운트
			whiteCount++;
	}
	if (blackCount == 5)
		return;

	if (blackCount == 3 && whiteCount == 0)
	{
		if (!(blackCheck[x + 4][y] == 1 && blackCheck[x - 4][y] == 1)) // 0  0  0 제거
		{
			if (blackCheck[x + 6][y] == 0 && blackCheck[x - 6][y] == 0) // 한쪽끝 너머에라도 백돌 있으면 무시 (순수3 아님)
			{
				if (blackCheck[x + 4][y] == 1) // 오른쪽 끝에만 백 있고
				{
					if (blackCheck[x + 6][y] == 0 && whiteCheck[x + 6][y] == 0) // 그 뒤에 아무것도 없으면 순수3
					{
						tempCount++; // 임시3 인 것들 세기
						for (int k = 0; k < 10; k++)
						{
							if (temp3[k].pos[0].pX == -1)
							{
								for (int i = -4; i < 5; i += 2)
								{
									if (blackCheck[x + i][y]) // 백색 있으면 카운트
									{
										temp3[k].pos[posNum].pX = x + i;
										temp3[k].pos[posNum].pY = y;
										posNum++;
									}

								}
								posNum = 0; // posNum 초기화
								break; // 빈 곳 찾아서 위치값 저장했으면 다른 빈 곳 또 들가지 말고 브레이크
							} // 빈 곳 찾기 끝
						} //빈곳 찾기 끝
					}
				} // 오른쪽 끝 조사 끝
				else if (blackCheck[x - 4][y] == 1) // 왼쪽 끝에만 백 있고
				{
					if (blackCheck[x - 6][y] == 0 && whiteCheck[x - 6][y] == 0) // 그 뒤에 아무것도 없으면 
					{
						tempCount++; // 임시3 인 것들 세기
						for (int k = 0; k < 10; k++)
						{
							if (temp3[k].pos[0].pX == -1)
							{
								for (int i = -4; i < 5; i += 2)
								{
									if (blackCheck[x + i][y]) // 백색 있으면 카운트
									{
										temp3[k].pos[posNum].pX = x + i;
										temp3[k].pos[posNum].pY = y;
										posNum++;
									}

								}
								posNum = 0; // posNum 초기화
								break; // 빈 곳 찾아서 위치값 저장했으면 다른 빈 곳 또 들가지 말고 브레이크
							} // 빈 곳 찾기 끝
						} //빈곳 찾기 끝
					}
				} // 왼쪽 끝 조사 끝
				else // 중앙 3개 순수 3
				{
					tempCount++; // 임시3 인 것들 세기
					for (int k = 0; k < 10; k++)
					{
						if (temp3[k].pos[0].pX == -1)
						{
							for (int i = -4; i < 5; i += 2)
							{
								if (blackCheck[x + i][y]) // 백색 있으면 카운트
								{
									temp3[k].pos[posNum].pX = x+i;
									temp3[k].pos[posNum].pY = y;
									posNum++;
								}

							}
							posNum = 0; // posNum 초기화
							break; // 빈 곳 찾아서 위치값 저장했으면 다른 빈 곳 또 들가지 말고 브레이크
						} // 빈 곳 찾기 끝
					} //빈곳 찾기 끝
				} // 중앙 3개 등록 끝


			} // 양쪽 끝에 백이 있는게 아닐 때 끝
		}
	} // (blackCount == 3 && whiteCount == 0) 끝
	blackCount = 0;
	whiteCount = 0;
	//===============양쪽 가로 5칸 조사 끝================

	//========양쪽 세로 5칸 조사======
	for (int i = -2; i < 3; i ++)
	{
		if (blackCheck[x][y + i] == 1) // 백색 있으면 카운트
			blackCount++;
		if (whiteCheck[x][y + i] == 1) // 흑색 있으면 카운트
			whiteCount++;
	}
	if (blackCount == 5)
		return;

	if (blackCount == 3 && whiteCount == 0)
	{
		if (!(blackCheck[x][y + 2] == 1 && blackCheck[x][y - 2] == 1)) // 0  0  0 제거
		{
			if (blackCheck[x][y + 3] == 0 && blackCheck[x][y - 3] == 0) // 한쪽끝 너머에라도 백돌 있으면 무시 (순수3 아님)
			{
				if (blackCheck[x][y + 2] == 1) // 오른쪽 끝에만 백 있고
				{
					if (blackCheck[x][y + 3] == 0 && whiteCheck[x][y + 3] == 0) // 그 뒤에 아무것도 없으면 
					{
						tempCount++; // 임시3 인 것들 세기
						for (int k = 0; k < 10; k++)
						{
							if (temp3[k].pos[0].pX == -1)
							{
								for (int i = -2; i < 3; i ++)
								{
									if (blackCheck[x ][y+i]) // 백색 있으면 카운트
									{
										temp3[k].pos[posNum].pX = x;
										temp3[k].pos[posNum].pY = y + i;
										posNum++;
									}

								}
								posNum = 0; // posNum 초기화
								break; // 빈 곳 찾아서 위치값 저장했으면 다른 빈 곳 또 들가지 말고 브레이크
							} // 빈 곳 찾기 끝
						} //빈곳 찾기 끝
					}
				} // 아래 끝 조사 끝
				else if (blackCheck[x][y - 2] == 1) // 왼쪽 끝에만 백 있고
				{
					if (blackCheck[x][y - 3] == 0 && whiteCheck[x][y-3] == 0) // 그 뒤에 아무것도 없으면 
					{
						tempCount++; // 임시3 인 것들 세기
						for (int k = 0; k < 10; k++)
						{
							if (temp3[k].pos[0].pX == -1)
							{
								for (int i = -2; i < 3; i++)
								{
									if (blackCheck[x][y + i]) // 백색 있으면 카운트
									{
										temp3[k].pos[posNum].pX = x;
										temp3[k].pos[posNum].pY = y + i;
										posNum++;
									}

								}
								posNum = 0; // posNum 초기화
								break; // 빈 곳 찾아서 위치값 저장했으면 다른 빈 곳 또 들가지 말고 브레이크
							} // 빈 곳 찾기 끝
						} //빈곳 찾기 끝
					}
				} // 위쪽 끝 조사 끝
				else // 중앙 3개 순수 3
				{
					tempCount++; // 임시3 인 것들 세기
					for (int k = 0; k < 10; k++)
					{
						if (temp3[k].pos[0].pX == -1)
						{
							for (int i = -2; i < 3; i++)
							{
								if (blackCheck[x][y + i]) // 백색 있으면 카운트
								{
									temp3[k].pos[posNum].pX = x;
									temp3[k].pos[posNum].pY = y + i;
									posNum++;
								}

							}
							posNum = 0; // posNum 초기화
							break; // 빈 곳 찾아서 위치값 저장했으면 다른 빈 곳 또 들가지 말고 브레이크
						} // 빈 곳 찾기 끝
					} //빈곳 찾기 끝
				} // 중앙 3개 등록 끝


			} // 양쪽 끝에 백이 있는게 아닐 때 끝
		}
	} // (blackCount == 3 && whiteCount == 0) 끝	
	blackCount = 0;
	whiteCount = 0;
	//===============양쪽 세로 5칸 조사 끝================

	//===============양쪽 좌상우하 5칸 조사================
	for (int i = -4; i < 5; i+=2)
	{
		if (blackCheck[x+i][y + (i/2)] == 1) // 백색 있으면 카운트
			blackCount++;
		if (whiteCheck[x+i][y + (i/2)] == 1) // 흑색 있으면 카운트
			whiteCount++;
	}
	if (blackCount == 5)
		return;

	if (blackCount == 3 && whiteCount == 0)
	{
		if (!(blackCheck[x - 4][y - 2] == 1 && blackCheck[x+4][y + 2] == 1)) // 0  0  0 제거
		{
			if (blackCheck[x + 6][y + 3] == 0 && blackCheck[x-6][y - 3] == 0) // 한쪽끝 너머에라도 백돌 있으면 무시 (순수3 아님)
			{
				if (blackCheck[x+4][y + 2] == 1) // 오른쪽 끝에만 백 있고
				{
					if (blackCheck[x+6][y + 3] == 0 && whiteCheck[x+6][y + 3] == 0) // 그 뒤에 아무것도 없으면 
					{
						tempCount++; // 임시3 인 것들 세기
						for (int k = 0; k < 10; k++)
						{
							if (temp3[k].pos[0].pX == -1)
							{
								for (int i = -4; i < 5; i += 2)
								{
									if (blackCheck[x + i][y + (i / 2)] == 1) // 백색 있으면 카운트
									{
										temp3[k].pos[posNum].pX = x + i;
										temp3[k].pos[posNum].pY = y + (i / 2);
										posNum++;
									}

								}
								posNum = 0; // posNum 초기화
								break; // 빈 곳 찾아서 위치값 저장했으면 다른 빈 곳 또 들가지 말고 브레이크
							} // 빈 곳 찾기 끝
						} //빈곳 찾기 끝
					}
				} // 아래 끝 조사 끝
				else if (blackCheck[x-4][y - 2] == 1) // 왼쪽 끝에만 백 있고
				{
					if (blackCheck[x-6][y - 3] == 0 && whiteCheck[x-6][y - 3] == 0) // 그 뒤에 아무것도 없으면 
					{
						tempCount++; // 임시3 인 것들 세기
						for (int k = 0; k < 10; k++)
						{
							if (temp3[k].pos[0].pX == -1)
							{
								for (int i = -4; i < 5; i += 2)
								{
									if (blackCheck[x + i][y + (i / 2)] == 1) // 백색 있으면 카운트
									{
										temp3[k].pos[posNum].pX = x + i;
										temp3[k].pos[posNum].pY = y + (i / 2);
										posNum++;
									}

								}
								posNum = 0; // posNum 초기화
								break; // 빈 곳 찾아서 위치값 저장했으면 다른 빈 곳 또 들가지 말고 브레이크
							} // 빈 곳 찾기 끝
						} //빈곳 찾기 끝
					}
				} // 위쪽 끝 조사 끝
				else // 중앙 3개 순수 3
				{
					tempCount++; // 임시3 인 것들 세기
					for (int k = 0; k < 10; k++)
					{
						if (temp3[k].pos[0].pX == -1)
						{
							
							for (int i = -4; i < 5; i += 2)
							{
								if (blackCheck[x + i][y + (i / 2)] == 1) // 백색 있으면 카운트
								{
									temp3[k].pos[posNum].pX = x + i;
									temp3[k].pos[posNum].pY = y + (i / 2);
									posNum++;
								}

							}
							posNum = 0; // posNum 초기화
							break; // 빈 곳 찾아서 위치값 저장했으면 다른 빈 곳 또 들가지 말고 브레이크
						} // 빈 곳 찾기 끝
					} //빈곳 찾기 끝
				} // 중앙 3개 등록 끝


			} // 양쪽 끝에 백이 있는게 아닐 때 끝
		}
	} // (blackCount == 3 && whiteCount == 0) 끝	
	blackCount = 0;
	whiteCount = 0;
	// =====================좌상우하 대각선 양쪽 조사 끝=================
	

	//===============양쪽 좌하우상 5칸 조사================
	for (int i = -4; i < 5; i += 2)
	{
		if (blackCheck[x + i][y - (i / 2)] == 1) // 백색 있으면 카운트
			blackCount++;
		if (whiteCheck[x + i][y - (i / 2)] == 1) // 흑색 있으면 카운트
			whiteCount++;
	}
	if (blackCount == 5)
		return;

	if (blackCount == 3 && whiteCount == 0)
	{
		if (!(blackCheck[x - 4][y + 2] == 1 && blackCheck[x + 4][y - 2] == 1)) // 0  0  0 제거
		{
			if (blackCheck[x - 6][y + 3] == 0 && blackCheck[x + 6][y - 3] == 0) // 한쪽끝 너머에라도 백돌 있으면 무시 (순수3 아님)
			{
				if (blackCheck[x + 4][y - 2] == 1) // 오른쪽 끝에만 백 있고
				{
					if (blackCheck[x + 6][y - 3] == 0 && whiteCheck[x + 6][y - 3] == 0) // 그 뒤에 아무것도 없으면 
					{
						tempCount++; // 임시3 인 것들 세기
						for (int k = 0; k < 10; k++)
						{
							if (temp3[k].pos[0].pX == -1)
							{
								for (int i = -4; i < 5; i += 2)
								{
									if (blackCheck[x + i][y - (i / 2)] == 1) // 백색 있으면 카운트
									{
										temp3[k].pos[posNum].pX = x + i;
										temp3[k].pos[posNum].pY = y - (i / 2);
										posNum++;
									}

								}
								posNum = 0; // posNum 초기화
								break; // 빈 곳 찾아서 위치값 저장했으면 다른 빈 곳 또 들가지 말고 브레이크
							} // 빈 곳 찾기 끝
						} //빈곳 찾기 끝
					}
				} // 우상 끝 조사 끝
				else if (blackCheck[x - 4][y + 2] == 1) // 왼쪽 끝에만 백 있고
				{
					if (blackCheck[x - 6][y + 3] == 0 && whiteCheck[x - 6][y + 3] == 0) // 그 뒤에 아무것도 없으면 
					{
						tempCount++; // 임시3 인 것들 세기
						for (int k = 0; k < 10; k++)
						{
							if (temp3[k].pos[0].pX == -1)
							{
								for (int i = -4; i < 5; i += 2)
								{
									if (blackCheck[x + i][y - (i / 2)] == 1) // 백색 있으면 카운트
									{
										temp3[k].pos[posNum].pX = x + i;
										temp3[k].pos[posNum].pY = y - (i / 2);
										posNum++;
									}

								}
								posNum = 0; // posNum 초기화
								break; // 빈 곳 찾아서 위치값 저장했으면 다른 빈 곳 또 들가지 말고 브레이크
							} // 빈 곳 찾기 끝
						} //빈곳 찾기 끝
					}
				} // 좌하 끝 조사 끝
				else // 중앙 3개 순수 3
				{
					for (int k = 0; k < 10; k++)
					{
						tempCount++; // 임시3 인 것들 세기
						if (temp3[k].pos[0].pX == -1)
						{
							for (int i = -4; i < 5; i += 2)
							{
								if (blackCheck[x + i][y - (i / 2)] == 1) // 백색 있으면 카운트
								{
									temp3[k].pos[posNum].pX = x + i;
									temp3[k].pos[posNum].pY = y - (i / 2);
									posNum++;
								}

							}
							posNum = 0; // posNum 초기화
							break; // 빈 곳 찾아서 위치값 저장했으면 다른 빈 곳 또 들가지 말고 브레이크
						} // 빈 곳 찾기 끝
					} //빈곳 찾기 끝
				} // 중앙 3개 등록 끝


			} // 양쪽 끝에 백이 있는게 아닐 때 끝
		}
	} // (blackCount == 3 && whiteCount == 0) 끝	
	blackCount = 0;
	whiteCount = 0;
	// =====================좌상우하 대각선 양쪽 조사 끝=================
	// ====================안쪽 5칸 조사 끝=========================

	//if (tempCount == 1) // 둔 곳이 33교차점이 아니라면
	//{
	//	//이미 순수3 클럽 회원 중에 겹치는 위치값 있는지 확인
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
	//					printf("●");
	//					blackCheck[x][y] = 1;
	//					error = 1;
	//					break;
	//				}
	//			}	
	//			if (overlap) // 하나라도 발견되면 다 나가기
	//				break;
	//		}
	//		if (overlap)// 하나라도 발견되면 다 나가기
	//			break;
	//	}

	//	//순수3 회원들 중에 겹치는 회원 없으면 이제 가입
	//	if (overlap == FALSE)
	//	for (int k = 0; k < 100; k++)
	//	{
	//		if (innocent3[k].pos[0].pX == -1)
	//		{
	//			innocent3[k].pos[0].pX = temp3[0].pos[0].pX; // 순수3클럽 가입
	//			innocent3[k].pos[0].pY = temp3[0].pos[0].pY;
	//			innocent3[k].pos[1].pX = temp3[0].pos[1].pX;
	//			innocent3[k].pos[1].pY = temp3[0].pos[1].pY;
	//			innocent3[k].pos[2].pX = temp3[0].pos[2].pX;
	//			innocent3[k].pos[2].pY = temp3[0].pos[2].pY;
	//			break;
	//		}

	//	}
	//} // 순수3클럽 가입완료
	/*else*/ if (tempCount > 1) // 교차점일 경우(33)
	{
		gotoxy(x, y);
		txtColor(0 | 96);
		printf("●");
		blackCheck[x][y] = 1;
		error = 1;
	}
				
	if (overlap == FALSE && tempCount < 2)
	{
		gotoxy(x, y);
		txtColor(0 | 96);
		printf("●");
		blackCheck[x][y] = 1;
	}

	overlap = FALSE;
		//계산 후 없다고 초기화
	
}

//=================white Exile====================
//백돌을 두었을 때 순수3 탈퇴 법
void whiteExile3(int a, int b)
{
	count = 0;
	blackCount = 0;
	whiteCount = 0;
	// =====양쪽에 두개 있을 시====
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
	} // ========양 옆에 두개 있을 시 끝=========

	// =========옆에 한개 있을 시===========
	if (blackCheck[a + 2][b] == 1) // 오른쪽 가로 한개 있을 시
	{
		for (int i = 2; i < 10; i += 2) // 4칸 범위내에 하양3 블랙0인지 체크
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

	} // 오른쪽 가로 한개 끝
	blackCount = 0;
	whiteCount = 0;

	if (blackCheck[a - 2][b] == 1) // 왼쪽 가로 한개 있을 시
	{
		for (int i = 2; i < 10; i+=2) // 4칸 범위내에 하양3 블랙0인지 체크
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

	if (blackCheck[a][b-1] == 1) // 위쪽 세로 한개 있을 시
	{
		for (int i = 1; i < 5; i++) // 4칸 범위내에 하양3 블랙0인지 체크
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

	if (blackCheck[a][b + 1] == 1) // 아래쪽 세로 한개 있을 시
	{
		for (int i = 1; i < 5; i++) // 4칸 범위내에 하양3 블랙0인지 체크
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
	
	if (blackCheck[a+2][b + 1] == 1) // 우하대각선 한개 있을 시
	{
		for (int i = 2; i < 10; i+=2) // 4칸 범위내에 하양3 블랙0인지 체크
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

	if (blackCheck[a - 2][b - 1] == 1) // 좌상대각선 한개 있을 시
	{
		for (int i = 2; i < 10; i += 2) // 4칸 범위내에 하양3 블랙0인지 체크
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

	if (blackCheck[a - 2][b + 1] == 1) // 좌하대각선 한개 있을 시
	{
		for (int i = 2; i < 10; i += 2) // 4칸 범위내에 하양3 블랙0인지 체크
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

	if (blackCheck[a + 2][b - 1] == 1) // 우상대각선 한개 있을 시
	{
		for (int i = 2; i < 10; i += 2) // 4칸 범위내에 하양3 블랙0인지 체크
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
	 // ===========한쪽 있을시 끝============

	} // ==================exileWhite 끝================== //수정 완료

	//판 위의 모든 검은색 주변 순수도 체크
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
	 // 검은 돌 놓을시 주변 순수3 순수도 체크
	void blackExile3(void)
{
	count = 0;
	blackCount = 0;
	whiteCount = 0;
	// =====양쪽에 두개 있을 시====
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
	} // ========양 옆에 두개 있을 시 끝=========

	// ===========전방위 하얀돌 체크========
	//오른쪽 가로
	for (int i = 2; i < 10; i += 2) // 4칸 범위내에 하양3 블랙0인지 체크
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

	//왼쪽 가로
	for (int i = 2; i < 10; i += 2) // 4칸 범위내에 하양3 블랙0인지 체크
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

	//위쪽 세로 
		for (int i = 1; i < 5; i++) // 4칸 범위내에 하양3 블랙0인지 체크
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

	

	//아래쪽 세로
		for (int i = 1; i < 5; i++) // 4칸 범위내에 하양3 블랙0인지 체크
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

	

	//우하대각선
		for (int i = 2; i < 10; i += 2) // 4칸 범위내에 하양3 블랙0인지 체크
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

	

	//좌상대각선
		for (int i = 2; i < 10; i += 2) // 4칸 범위내에 하양3 블랙0인지 체크
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

	

	//좌하대각선
		for (int i = 2; i < 10; i += 2) // 4칸 범위내에 하양3 블랙0인지 체크
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

	

	//우하대각선
		for (int i = 2; i < 10; i += 2) // 4칸 범위내에 하양3 블랙0인지 체크
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

	
	

} // whiteExile3 끝

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