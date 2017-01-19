#include <stdio.h>											// 입출력
#include <Windows.h>										// 커서 이동
#include <conio.h>											// getch함수 (키를 하나 받는 함수)
#include "turboc.h"
#include <string.h>
#include <wingdi.h>
#include <stdlib.h>
#include <mmsystem.h>

#pragma comment(lib,"winmm.lib")

#define ARR_SIZE_R 19
#define ARR_SIZE_C 19

int x, y;
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


int whiteCheck[100][100] = { 0 };
int blackCheck[100][100] = { 0 };
BOOL blackTurn = TRUE;
BOOL whiteTurn = TRUE;
BOOL gameBreaker = FALSE;
BOOL gameReboot = TRUE;
int server = 1;
BOOL spaceSelect = FALSE;
char p1[50] = "gh";
char p2[50] = "gh";
int p1diceNum = 1;
int p2diceNum = 1;
int first = 1;


//가로로 갈 때는 0, 2, 4, ... 36 짝수 (x)
//세로로 갈 때는 0, 1, 2... 18 (y)
//메인
int main(void)
{
	
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

void gameStart(void)
{
	if (gameBreaker == FALSE && gameReboot == TRUE)
		enterName();
	
	gameBreaker = FALSE;
	gameReboot = FALSE;
	
		
		//바둑판 세팅
		setPan();
		//게임 설명
		intro();

		//게임 진행
		gameProcess();
		
		return;
}

void enterName(void)
{
	txtColor(14);
	gotoxy(20, 8);
	puts("각각 이름이나 닉네임을 입력해주세요~");
	gotoxy(20, 11);
	txtColor(15);
	printf("첫 번째 참가자(Player 1) : ");
	scanf("%s", p1);
	gotoxy(20, 14);
	printf("두 번째 참가자(Player 2) : ");
	scanf("%s", p2);
	system("cls");



	//로또 추첨기계 돌리기
	while (1)
	{

		system("cls");
		txtColor(14);
		puts("\n\n   이제 최신식 로또 추첨기를 돌려 누가 선공을 할지 정할 겁니다.\n   0부터 99중에 더 높은 숫자를 뽑으신 분이 백돌로 선공을 할 것입니다. ");
		txtColor(15);
		printf("\n\n   %s님 Enter를 눌러 로또 추첨기계를 돌려주세요~\n", p1);
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
		printf("\n   %s님의 숫자는 ", p1);
		txtColor(14);
		printf("%d", p1diceNum);
		txtColor(15);
		printf("입니다\n");
		

		printf("\n   이제 %s님 Enter를 눌러 로또 추첨기계를 돌려주세요~\n", p2);
		getch();
		randomize();
		p2diceNum = rand()%100;

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
		printf("\n   %s님의 숫자는 ", p1);
		txtColor(14);
		printf("%d", p1diceNum);
		txtColor(15);
		printf("입니다\n");
		

	

		txtColor(15);
		printf("\n   %s님의 숫자는 ", p2);
		txtColor(14);
		printf("%d", p2diceNum);
		txtColor(15);
		printf("입니다\n");
		

		Sleep(1000);
	
		if (p1diceNum > p2diceNum)
		{
			printf("\n\n\n\n                                %s님의 선공입니다!!", p1);
			first = 1;
			break;
		}
		else if (p2diceNum > p1diceNum)
		{
			printf("\n\n\n\n                                %s님의 선공입니다!!", p2);
			first = 2;
			break;
		}
		if (p1diceNum == p2diceNum)
			printf("\n\n\n\n                                비겼습니다. 다시 돌려주세요.");
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
				else if (j == ARR_SIZE_C-1)
					printf("┓\n");
				else
					printf("┳");
			}
			else if (i != 1 && i != ARR_SIZE_R)
			{
				if (j == 0)
					printf("┣");
				else if (j == ARR_SIZE_C-1)
					printf("┫\n");
				else
					printf("╋");
			}
			else if (i == ARR_SIZE_R)
			{
				if (j == 0)
					printf("┗");
				else if (j == ARR_SIZE_C-1)
					printf("┛\n");
				else
					printf("┻");
			}
		}
	}
} // 바둑판 만들기 끝


//마우스 클릭 함수
void MouseClick(int *x, int *y)
{
	HANDLE       hIn, hOut;
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
	// check 배열 초기화(중복되면 안되니)
	if (gameBreaker == FALSE && gameReboot == FALSE)
	{
		for (int i = 0; i <= 100 ; i++)
		{
			for (int j = 0; j <=  100; j++)
			{
				whiteCheck[i][j] = 0;
				blackCheck[i][j] = 0;
			}
		}
	}
	
	//게임이 중단되지 않을 경우 계속 돌리기
	while (gameBreaker == FALSE)
	{
		while (whiteTurn && gameBreaker == FALSE)
		{
			
			if (first == 1)
			{
				txtColor(15);
				gotoxy(43, 7);
				printf("%s님의 차례입니다", p1);
			}
			else if (first == 2)
			{
				txtColor(15);
				gotoxy(43, 7);
				printf("%s님의 차례입니다", p2);
			}
			
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
					whiteTurn = FALSE;
					// check if White wins
					CheckWhiteWins();
				}
			}
		} // whiteturn while 끝

		while (blackTurn && gameBreaker == FALSE)
		{
			if (first == 1)
			{
				txtColor(15);
				gotoxy(43, 7);
				printf("%s님의 차례입니다", p2);
			}
			else if (first == 2)
			{
				txtColor(15);
				gotoxy(43, 7);
				printf("%s님의 차례입니다", p1);
			}
			MouseClick(&x, &y);
			if (0 <= x && x <= 37 && 0 <= y && y <= 18)
			{
				if (x % 2 == 1)
					x = x - 1;
				if (whiteCheck[x][y] == 0 && blackCheck[x][y] == 0)
				{
					gotoxy(x, y);
					txtColor(0 | 96);
					printf("●");
					blackCheck[x][y] = 1;
					blackTurn = FALSE;
					//check if Black wins
					CheckBlackWins();
				}
			}
		}// blackturn while 끝

		
		whiteTurn = TRUE; // white turn reset
		blackTurn = TRUE; // black turn reset
	
	} // while 끝

	

	return;
}

//하얀색이 이겼는지 판별
void CheckWhiteWins(void)
{
	for (int i = 0; i < 100; i++)
	{
		if (whiteCheck[i][y] == 1 && whiteCheck[i + 2][y] == 1 && whiteCheck[i + 4][y] == 1 && whiteCheck[i+6][y] == 1 && whiteCheck[i + 8][y] == 1)
		{
			txtColor(14);
			gotoxy(49, 12);
			if (first == 1)
				printf("%s님이 승리하셨습니다!", p1);
			else if (first == 2)
				printf("%s님이 승리하셨습니다!", p2);
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
			WhiteMsgBox();
			return;
		}
		if (whiteCheck[x][i] == 1 && whiteCheck[x][i + 1] == 1 && whiteCheck[x][i + 2] == 1 && whiteCheck[x][i + 3] == 1 && whiteCheck[x][i + 4] == 1)
		{
			txtColor(14);
			gotoxy(49, 12);
			if (first == 1)
				printf("%s님이 승리하셨습니다!", p1);
			else if (first == 2)
				printf("%s님이 승리하셨습니다!", p2);
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
			WhiteMsgBox();
			return;
		}
		for (int j = 0; j < 100; j++)
		{
			if (whiteCheck[j][i] == 1 && whiteCheck[j + 2][i + 1] == 1 && whiteCheck[j + 4][i + 2] == 1 && whiteCheck[j + 6][i + 3] == 1 && whiteCheck[j + 8][i + 4] == 1)
			{
				txtColor(14);
				gotoxy(49, 12);
				if (first == 1)
					printf("%s님이 승리하셨습니다!", p1);
				else if (first == 2)
					printf("%s님이 승리하셨습니다!", p2);
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
				WhiteMsgBox();
				return;
			}
			if (whiteCheck[j][i] == 1 && whiteCheck[j - 2][i + 1] == 1 && whiteCheck[j - 4][i + 2] == 1 && whiteCheck[j -6][i + 3] == 1 && whiteCheck[j - 8][i + 4] == 1)
			{
				txtColor(14);
				gotoxy(49, 12);
				if (first == 1)
					printf("%s님이 승리하셨습니다!", p1);
				else if (first == 2)
					printf("%s님이 승리하셨습니다!", p2);
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
				WhiteMsgBox();
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
		//check row
		if (blackCheck[i][y] == 1 && blackCheck[i + 2][y] == 1 && blackCheck[i + 4][y] == 1 && blackCheck[i+6][y] == 1 && blackCheck[i + 8][y] == 1)
		{
			txtColor(14);
			gotoxy(49, 12);
			if (first == 1)
				printf("%s님이 승리하셨습니다!", p2);
			else if (first == 2)
				printf("%s님이 승리하셨습니다!", p1);
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
			BlackMsgBox();
			return;
		}
		//check column
		if (blackCheck[x][i] == 1 && blackCheck[x][i+1] == 1 && blackCheck[x][i+2] == 1 && blackCheck[x][i+3] == 1 && blackCheck[x][i+4] == 1)
		{
			txtColor(14);
			gotoxy(49, 12);
			if (first == 1)
				printf("%s님이 승리하셨습니다!", p2);
			else if (first == 2)
				printf("%s님이 승리하셨습니다!", p1);
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
			BlackMsgBox();
			return;
		}
		for (int j = 0; j < 100; j++)
		{
			if (blackCheck[j][i] == 1 && blackCheck[j + 2][i + 1] == 1 && blackCheck[j + 4][i + 2] == 1 && blackCheck[j + 6][i + 3] == 1 && blackCheck[j + 8][i + 4] == 1)
			{
				txtColor(14);
				gotoxy(49, 12);
				if (first == 1)
					printf("%s님이 승리하셨습니다!", p2);
				else if (first == 2)
					printf("%s님이 승리하셨습니다!", p1);
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
				BlackMsgBox();
				return;
			}
			if (blackCheck[j][i] == 1 && blackCheck[j - 2][i + 1] == 1 && blackCheck[j - 4][i + 2] == 1 && blackCheck[j - 6][i + 3] == 1 && blackCheck[j - 8][i + 4] == 1)
			{
				txtColor(14);
				gotoxy(49, 12);
				if (first == 1)
					printf("%s님이 승리하셨습니다!", p2);
				else if (first == 2)
					printf("%s님이 승리하셨습니다!", p1);
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
				BlackMsgBox();
				
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
	
	int msgID = MessageBoxA(NULL, "☆☆☆  백돌이 이겼습니다!  ☆☆☆", "축하합니다!", MB_RETRYCANCEL);
	switch (msgID)
	{
		case IDRETRY :
			txtColor(0);
			system("cls");
			
			gameBreaker = TRUE;
			gameReboot = TRUE;
			break;
		case IDCANCEL :
			gameBreaker = TRUE;
			gameReboot = FALSE;
			exit(1);
			break;
	}
	
}


void BlackMsgBox(void)
{
	int msgID = MessageBoxA(NULL, "★★★  흑돌이 이겼습니다! ★★★", "축하합니다!", MB_RETRYCANCEL);
	switch (msgID)
	{
	case IDRETRY:
		// system("cls");
		txtColor(0);
		gameBreaker = TRUE;
		gameReboot = TRUE;
		break;
	case IDCANCEL:
		gameBreaker = TRUE;
		gameReboot = FALSE;
		exit(1);
		break;
	}
	return;
}

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

