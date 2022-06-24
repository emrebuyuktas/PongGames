#include <iostream>
#include <graphics.h>


#define UP_BOARD_ONE 119
#define DOWN_BOARD_ONE 115
#define UP_BOARD_TWO 107
#define DOWN_BOARD_TWO 109

#define WIDTH 1000
#define HEIGHT 600

#define BOARD_ONE_LEFT        10
#define BOARD_ONE_TOP         (HEIGHT/2)-40
#define BOARD_ONE_RIGHT       20
#define BOARD_ONE_BOTTOM      (height/2)+40


#define BOARD_TWO_LEFT        WIDTH-20
#define BOARD_TWO_TOP         (HEIGHT/2)-40
#define BOARD_TWO_RIGHT       WIDTH-10
#define BOARD_TWO_BOTTOM      (HEIGHT/2)+40



#define BOARD_CLR      RED
#define BOARD_YUP   10
#define BOARD_YDOWN   -10

static int playerOneScore=0;
static int playerTwoScore=0;


int counter=0;
using namespace std;

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


class Board{
  public:
    int left;
    int top;
    int right;
    int bottom;
    int color=RED;
    Board(){}
    Board(int leftInput,int topInput,int rightInput,int bottomInput)
    {
        left=leftInput;
        top=topInput;
        right=rightInput;
        bottom=bottomInput;
    }
};

class Ball {
  public:
    int positionX=WIDTH/2;
    int positionY=HEIGHT/2;
    int directionX=2;
    int directionY=-2;
    Positions position=reset;
};


void drawFilledRectangle(Board &boardOne, Board &boardTwo,int key)
{


    if(key==UP_BOARD_ONE && boardOne.top!=0)
    {
        setcolor(RED);
        setfillstyle(SOLID_FILL, BLUE);
        bar(boardOne.left, boardOne.top, boardOne.right, boardOne.bottom);

        setcolor(BLUE);
        setfillstyle(SOLID_FILL, BLUE);
        boardOne.top-=BOARD_YUP;
        boardOne.bottom-=BOARD_YUP;
    }
    else if(key==DOWN_BOARD_ONE && boardOne.bottom!=HEIGHT)
    {
        setcolor(RED);
        setfillstyle(SOLID_FILL, BLUE);
        bar(boardOne.left, boardOne.top, boardOne.right, boardOne.bottom);

        setcolor(BLUE);
        setfillstyle(SOLID_FILL, BLUE);
        boardOne.top+=BOARD_YUP;
        boardOne.bottom+=BOARD_YUP;
    }
    else if(key==UP_BOARD_TWO && boardTwo.top!=0)
    {
        setcolor(RED);
        setfillstyle(SOLID_FILL, BLUE);
        bar(boardTwo.left, boardTwo.top, boardTwo.right, boardTwo.bottom);

        setcolor(BLUE);
        setfillstyle(SOLID_FILL, BLUE);
        boardTwo.top-=BOARD_YUP;
        boardTwo.bottom-=BOARD_YUP;
    }
    else if(key==DOWN_BOARD_TWO && boardTwo.bottom!=HEIGHT)
    {
        setcolor(RED);
        setfillstyle(SOLID_FILL, BLUE);
        bar(boardTwo.left, boardTwo.top, boardTwo.right, boardTwo.bottom);

        setcolor(BLUE);
        setfillstyle(SOLID_FILL, BLUE);
        boardTwo.top+=BOARD_YUP;
        boardTwo.bottom+=BOARD_YUP;
    }
    else
    {
        setcolor(boardOne.color);
        setfillstyle(SOLID_FILL, boardOne.color);
        bar(boardOne.left, boardOne.top, boardOne.right, boardOne.bottom);
        bar(boardTwo.left, boardTwo.top, boardTwo.right, boardTwo.bottom);
    }
}



void DrawBall(Ball &ball, Board boardOne, Board boardTwo)
{
    if(ball.positionX<=0 )
    {
        playerTwoScore+=1;
        setcolor(BLUE);
        setfillstyle(SOLID_FILL,BLUE);
        fillellipse(ball.positionX, ball.positionY, 10,10);
        setcolor(YELLOW);
        setfillstyle(SOLID_FILL,YELLOW);
        ball.positionX=WIDTH/2;
        ball.positionY=HEIGHT/2;
        ball.directionX=-1*ball.directionX;
        ball.directionY=-1*ball.directionY;
        fillellipse(ball.positionX, ball.positionY, 10,10);

    }else if(ball.positionX>=WIDTH)
    {
        playerOneScore+=1;
        setcolor(BLUE);
        setfillstyle(SOLID_FILL,BLUE);
        fillellipse(ball.positionX, ball.positionY, 10,10);
        setcolor(YELLOW);
        setfillstyle(SOLID_FILL,YELLOW);
        ball.positionX=WIDTH/2;
        ball.positionY=HEIGHT/2;
        ball.directionX=-1*ball.directionX;
        ball.directionY=-1*ball.directionY;
        fillellipse(ball.positionX, ball.positionY, 10,10);
    }
    if(ball.positionY>=HEIGHT-10 || ball.positionY<=10)
    {
        ball.directionY=-1*ball.directionY;
    }

    if(ball.positionX>WIDTH/2)
    {
        if(boardTwo.top<= ball.positionY && ball.positionY<= boardTwo.bottom && ball.positionX>=boardTwo.left-10){
            ball.directionX=-1*ball.directionX;
        }
    }
    else
    {
        if(boardOne.top<= ball.positionY && ball.positionY<= boardOne.bottom && ball.positionX<=boardOne.right+10)
        {
            ball.directionX=-1*ball.directionX;
        }
    }
        setcolor(BLUE);
        setfillstyle(SOLID_FILL,BLUE);
        fillellipse(ball.positionX, ball.positionY, 10,10);
        setcolor(YELLOW);
        setfillstyle(SOLID_FILL,YELLOW);
        ball.positionX+=ball.directionX;
        ball.positionY+=ball.directionY;
        fillellipse(ball.positionX, ball.positionY, 10,10);

        Sleep(10);
}

void Run()
{
    initwindow(WIDTH,HEIGHT);
    setbkcolor(1);
    cleardevice();

    Board boardOne(BOARD_ONE_LEFT,BOARD_ONE_TOP,BOARD_ONE_RIGHT,BOARD_TWO_BOTTOM);
    Board boardTwo(BOARD_TWO_LEFT,BOARD_TWO_TOP,BOARD_TWO_RIGHT,BOARD_TWO_BOTTOM);
    drawFilledRectangle(boardOne,boardTwo,0);

    Ball ball;
    DrawBall(ball,boardOne,boardTwo);

    while(TRUE)
    {
        std::string tmp = std::to_string(playerOneScore);
        char const *num_char = tmp.c_str();
        std::string tmptwo = std::to_string(playerTwoScore);
        char const *num_char_two = tmptwo.c_str();
        outtextxy(WIDTH/2-7, 4, (char*)num_char);
        outtextxy(WIDTH/2+7, 4, (char*)num_char_two);
        DrawBall(ball,boardOne,boardTwo);
        int value=0;
        if(kbhit())
        {
            char key=getch();
            value=key;
        }
        drawFilledRectangle(boardOne,boardTwo,value);
    }
    getch();
    closegraph();
}

int main()
{
    Run();
    return 0;
}
