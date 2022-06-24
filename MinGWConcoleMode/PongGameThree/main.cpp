#include <iostream>
#include <windows.h>
#include <conio.h>
using namespace std;
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;
int width=80;
int height=25;

static int playerOneScore=0;
static int playerTwoScore=0;

#define UP_BOARD_ONE 119
#define DOWN_BOARD_ONE 115
#define UP_BOARD_TWO 107
#define DOWN_BOARD_TWO 109

enum Positions{
    turnbacright=0,
    turnbackleft=1,
    leftcrossdown=2,
    rightcrossdown=3,
    leftcrossup=4,
    rightcrossup=5,
    reset=6,
    movetoright=7
};

void gotoxy(int x, int y)
{
  CursorPosition.X = x;
  CursorPosition.Y = y;
  SetConsoleCursorPosition(console, CursorPosition);
}

class Ball {
  public:
    int positionX;
    int positionY;
    int directionX=1;
    int directionY=1;
};

class Board{
  public:
    int position;
};

void DrawBorders()
{
  for (int i = 2; i <= width; i++) {
    gotoxy(i, 1);
    cout << '-';
    gotoxy(i, height);
    cout << '-';
  }
  for (int i = 1; i <= height; i++) {
    gotoxy(2, i);
    cout << '|';
    gotoxy(width, i);
    cout << '|';
  }
}

void DrawBoards(Board &boardOne,Board &boardTwo,int key)
{
    if(key==UP_BOARD_ONE && boardOne.position!=4){
        for(int i=0;i<5;i++){
        gotoxy(4,boardOne.position+i-2);
        cout << " "<< endl;
        }
        boardOne.position-=1;
        for(int i=0;i<5;i++){
        gotoxy(4,boardOne.position+i-2);
        cout << '|'<< endl;
        }
    }
    else if(key==DOWN_BOARD_ONE && boardOne.position!=height-3)
    {
        for(int i=0;i<5;i++){
        gotoxy(4,boardOne.position+i-2);
        cout << " "<< endl;
        }
        boardOne.position+=1;
        for(int i=0;i<5;i++){
        gotoxy(4,boardOne.position+i-2);
        cout << '|'<< endl;
        }
    }
    else if(key==UP_BOARD_TWO && boardTwo.position!=4){
        for(int i=0;i<5;i++){
        gotoxy(width-2,boardTwo.position+i-2);
        cout << " "<< endl;
        }
        boardTwo.position-=1;
        for(int i=0;i<5;i++){
        gotoxy(width-2,boardTwo.position+i-2);
        cout << '|'<< endl;
        }
    }
    else if(key==DOWN_BOARD_TWO && boardTwo.position!=height-3)
    {
        for(int i=0;i<5;i++){
        gotoxy(width-2,boardTwo.position+i-2);
        cout << " "<< endl;
        }
        boardTwo.position+=1;
        for(int i=0;i<5;i++){
        gotoxy(width-2,boardTwo.position+i-2);
        cout << '|'<< endl;
        }
    }
}
void DrawBoards(Board boardOne,Board boardTwo)
{
    for(int i=0;i<5;i++){
        gotoxy(4,boardOne.position+i-2);
        cout << '|'<< endl;
    }
    for(int i=0;i<5;i++){
        gotoxy(width-2,boardTwo.position+i-2);
        cout << '|'<< endl;
    }
}

void CalculateBallPosition(Ball &ball, Board boardOne, Board boardTwo)
{
    gotoxy(84,10);
    cout<<playerOneScore;
    gotoxy(94,10);
    cout<<playerTwoScore;
    if(ball.positionX==3 || ball.positionX==79)
    {
        playerOneScore=ball.positionX==79?playerOneScore+1:playerOneScore;
        playerTwoScore=ball.positionX==3?playerTwoScore+1:playerTwoScore;
        gotoxy(ball.positionX,ball.positionY);
        cout<<" ";
        ball.positionX=40;
        ball.positionY=12;

    }
    else if(ball.positionY==height-1 || ball.positionY==2)
    {
        ball.directionY=ball.directionY*-1;
    }
    else if(boardTwo.position-2<= ball.positionY && ball.positionY<= boardTwo.position+2 && ball.positionX==78){
            ball.directionX=ball.directionX*-1;
    }
    else if(boardOne.position-2<= ball.positionY && ball.positionY<= boardOne.position+2 && ball.positionX==4){
            ball.directionX=ball.directionX*-1;
        }
        gotoxy(ball.positionX,ball.positionY);
        cout<<" ";
        ball.positionX+=ball.directionX;
        ball.positionY+=ball.directionY;
        gotoxy(ball.positionX,ball.positionY);
        cout<<"O";
        Sleep(50);
}

void Run()
{
    DrawBorders();
    Board boardOne;
    Board boardTwo;
    boardOne.position=(height/2);
    boardTwo.position=(height/2);


    Ball ball;
    ball.positionX=40;
    ball.positionY=12;
    DrawBoards(boardOne,boardTwo);
    gotoxy(ball.positionX,ball.positionY);
    cout<<"O";
    while (TRUE) {
        //system("cls");
        if (_kbhit()) {
            char key = _getch();
            int value = key;
            DrawBoards(boardOne, boardTwo, key);
        }
        else {
            DrawBoards(boardOne, boardTwo);
        }
        CalculateBallPosition(ball,boardOne,boardTwo);
    }
}

int main()
{
    Run();
    return 0;
}
