#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<cmath>
#include<iomanip>
//#include<bits/stdc++.h>
using namespace std;


#define ContainerX 13
#define ContainerY 3
#define Container_Height 20
#define Container_Width 18

int i,j,Temp1,Temp2;
int a[80][80]={0};
int color[80][80] = {0};

bool ald[80][80] = {0};

int b[4];//一个方块4块
int Speed;
int Num;
int Score;
int Level;

typedef struct tetris
{
    int x;
    int y;
    int flag;
    struct tetris *Next;
} Tetris;

struct tetris *Pa_Tetris;//工具指针
struct tetris *Prear_Tetris;//尾指针
struct tetris *Phead_Tetris;//头指针
struct tetris *PTemp_Tetris;//当前方块
int Contact; //0/1变量

HANDLE hOut;

//函数部分
void Gotoxy(int x, int y);//移动光标
void DrawFrame();//打印游戏边框
void CreateFlag();//构建链表（关键函数）
void MakeBlock(struct tetris *); //制作方块
void PrintBlock(struct tetris *);//打印方块
void DeleteBlock(struct tetris *);//清除
int  CanMove(struct tetris *);    //判断是否可以移动
void IfFulline(struct tetris *); //判断是否满行
void GamePlay();//开始游戏

void Regulation();//游戏规则（暂未开放）
void Explanation();//相关解释（暂未开放）
void Replay();//重玩
void WelCome();//主界面
void Title();//打印标题
void Close();//退出
int SetColor(int color);//设置颜色
//void Flower();
void HideCursor();//隐藏光标

int main()
{
    WelCome();
    return 0;
}

//指定位置输出
void Gotoxy(int x, int y)
{
	COORD Pos;
	Pos.X = x;
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

//打印游戏边框
void DrawFrame()
{
    HideCursor();
    Gotoxy(ContainerX+Container_Width-5,ContainerY-2);
    SetColor(11);
    cout << "俄罗斯方块";
    Gotoxy(ContainerX+2*Container_Width+3,ContainerY+7);
    SetColor(2);
    cout << "**********";
    Gotoxy(ContainerX+2*Container_Width+13,ContainerY+7);
    SetColor(3);
    cout << "出现下一个方块";
    Gotoxy(ContainerX+2*Container_Width+3,ContainerY+13);
    SetColor(2);
    cout << "**********";
    Gotoxy(ContainerX+2*Container_Width+3,ContainerY+17);
    SetColor(14);
    cout << "↑键：旋转";
    Gotoxy(ContainerX+2*Container_Width+3,ContainerY+19);
    cout << "空格：暂停游戏";
    Gotoxy(ContainerX+2*Container_Width+3,ContainerY+15);
    cout << "Esc：退出游戏";
    //Gotoxy(ContainerX-1,ContainerY);
    Gotoxy(ContainerX,ContainerY);
    SetColor(12);
    cout << "┌-";
    Gotoxy(ContainerX+2*Container_Width-2,ContainerY);
    cout << "┐";
    //Gotoxy(ContainerX-1,ContainerY+Container_Height);
    Gotoxy(ContainerX,ContainerY+Container_Height);
    cout << "└-";
    Gotoxy(ContainerX+2*Container_Width-2,ContainerY+Container_Height);
    cout << "┘";

    for (i = 2; i < 2 * Container_Width - 2; i += 2)
    {
        Gotoxy(ContainerX+i, ContainerY);
        cout << "--";
        //a[ContainerX + i][ContainerY] = 2;
    }

    for (i = 2; i < 2 * Container_Width - 2; i += 2)
    {
        Gotoxy(ContainerX+i, ContainerY+Container_Height);
        cout << "--";
        a[ContainerX + i][ContainerY+Container_Height] = 2;
    }

    for (i = 1; i < Container_Height; i++)
    {
        Gotoxy(ContainerX+1, ContainerY + i);
        cout << "|";
        a[ContainerX][ContainerY+i] = 2;
    }

    for (i = 1; i < Container_Height; i++)
    {
        Gotoxy(ContainerX + 2 * Container_Width - 2, ContainerY + i);
        cout << "|";
        //a[ContainerX+Container_Width][ContainerY] = 2;
        a[ContainerX + 2 * Container_Width - 2][ContainerY + i] = 2;
    }
    cout << endl
         << endl;
}

void PrintBlock(struct tetris* tetris)
{
    HideCursor();
    for (i = 0; i < 4; i++)
    {
        b[i] = 1;
    }

    MakeBlock(tetris);

    for (i = tetris->x - 2; i <= tetris->x + 4; i += 2)
        for (j = tetris->y - 2; j <= tetris->y+ 1; j++)
        {
            if(a[i][j]==1&&j>ContainerY&&ald[i][j]==0)
            {
                Gotoxy(i,j);
                ald[i][j] = 1;
                cout << "■";
            }

        }

    Gotoxy(ContainerX + 2 * Container_Width + 3, ContainerY + 1);
    SetColor(4);
    cout << "Level:";
    SetColor(12);
    cout << Level;

    Gotoxy(ContainerX+2*Container_Width+3,ContainerY+3);
    SetColor(4);
    cout << "Score:";
    SetColor(12);
    cout << Score;

    Gotoxy(ContainerX+2*Container_Width+3,ContainerY+5);
    SetColor(4);
    cout << "Speed:";
    SetColor(12);
    cout << Speed;
}

//制作方块
void MakeBlock(struct tetris* tetris)
{
    a[tetris->x][tetris->y] = b[0];
    switch(tetris->flag)
    {
        case 1://田字格，只有一种
        {
            SetColor(10);
            a[tetris->x][tetris->y - 1] = b[1];
            a[tetris->x+2][tetris->y-1] = b[2];
            a[tetris->x + 2][tetris->y] = b[3];
            color[tetris->x][tetris->y - 1] = color[tetris->x + 2][tetris->y - 1] = color[tetris->x + 2][tetris->y] = 10;
            break;
        }
        case 2://横
        {
            SetColor(13);
            a[tetris->x - 2][tetris->y] = b[1];
            a[tetris->x + 2][tetris->y] = b[2];
            a[tetris->x + 4][tetris->y] = b[3];
            color[tetris->x - 2][tetris->y] = color[tetris->x + 2][tetris->y] = color[tetris->x + 4][tetris->y] = 13;
            break;
        }
        case 3://竖
        {
            SetColor(13);
            a[tetris->x][tetris->y - 1] = b[1];
            a[tetris->x][tetris->y - 2] = b[2];
            a[tetris->x][tetris->y + 1] = b[3];
            color[tetris->x][tetris->y - 1] = color[tetris->x][tetris->y - 2] = color[tetris->x][tetris->y + 1] = 13;
            break;
        }
        case 4://上山
        {
            SetColor(11);
            a[tetris->x - 2][tetris->y] = b[1];
            a[tetris->x + 2][tetris->y] = b[2];
            a[tetris->x][tetris->y + 1] = b[3];
            color[tetris->x - 2][tetris->y] = color[tetris->x + 2][tetris->y] = color[tetris->x][tetris->y + 1] = 11;
            break;
        }
        case 5://左山
        {
            SetColor(11);
            a[tetris->x][tetris->y - 1] = b[1];
            a[tetris->x][tetris->y + 1] = b[2];
            a[tetris->x - 2][tetris->y] = b[3];
            color[tetris->x][tetris->y - 1] = color[tetris->x][tetris->y + 1] = color[tetris->x - 2][tetris->y] = 11;
            break;
        }
        case 6://下山
        {
            SetColor(11);
            a[tetris->x][tetris->y - 1] = b[1];
            a[tetris->x - 2][tetris->y] = b[2];
            a[tetris->x + 2][tetris->y] = b[3];
            color[tetris->x][tetris->y - 1] = color[tetris->x - 2][tetris->y] = color[tetris->x + 2][tetris->y] = 11;
            break;
        }
        case 7://右山
        {
            
            SetColor(11);
            a[tetris->x][tetris->y - 1] = b[1];
            a[tetris->x][tetris->y + 1] = b[2];
            a[tetris->x + 2][tetris->y] = b[3];
            color[tetris->x][tetris->y - 1] = color[tetris->x][tetris->y + 1] = color[tetris->x + 2][tetris->y] = 11;
            break;
        }
        case 8://右s
        {
            SetColor(14);
            a[tetris->x][tetris->y + 1] = b[1];
            a[tetris->x - 2][tetris->y] = b[2];
            a[tetris->x + 2][tetris->y + 1] = b[3];
            color[tetris->x][tetris->y + 1] = color[tetris->x - 2][tetris->y] = color[tetris->x + 2][tetris->y + 1] = 14;
            break;
        }
        case 9://上s
        {
            SetColor(14);
            a[tetris->x][tetris->y - 1] = b[1];
            a[tetris->x - 2][tetris->y] = b[2];
            a[tetris->x - 2][tetris->y + 1] = b[3];
            color[tetris->x][tetris->y - 1] = color[tetris->x - 2][tetris->y] = color[tetris->x - 2][tetris->y + 1] = 14;
            break;
        }
        case 10://
        {
            SetColor(14);
            a[tetris->x][tetris->y - 1] = b[1];
            a[tetris->x - 2][tetris->y - 1] = b[2];
            a[tetris->x + 2][tetris->y] = b[3];
            color[tetris->x][tetris->y - 1] = color[tetris->x - 2][tetris->y - 1] = color[tetris->x + 2][tetris->y] = 14;
            break;
        }
        case 11://上z
        {
            SetColor(14);
            a[tetris->x][tetris->y + 1] = b[1];
            a[tetris->x - 2][tetris->y - 1] = b[2];
            a[tetris->x - 2][tetris->y] = b[3];
            color[tetris->x][tetris->y + 1] = color[tetris->x - 2][tetris->y - 1] = color[tetris->x - 2][tetris->y] = 14;
            break;
        }
        case 12://左L
        {
            SetColor(12);
            a[tetris->x][tetris->y - 1] = b[1];
            a[tetris->x][tetris->y + 1] = b[2];
            a[tetris->x - 2][tetris->y - 1] = b[3];
            color[tetris->x][tetris->y - 1] = color[tetris->x][tetris->y + 1] = color[tetris->x - 2][tetris->y - 1] = 12;
            break;
        }
        case 13://J倒
        {
            SetColor(12);
            a[tetris->x - 2][tetris->y] = b[1];
            a[tetris->x + 2][tetris->y - 1] = b[2];
            a[tetris->x + 2][tetris->y] = b[3];
            color[tetris->x - 2][tetris->y] = color[tetris->x + 2][tetris->y - 1] = color[tetris->x + 2][tetris->y] = 12;
            break;
        }
        case 14://翻转L
        {
            SetColor(12);
            a[tetris->x][tetris->y - 1] = b[1];
            a[tetris->x][tetris->y + 1] = b[2];
            a[tetris->x + 2][tetris->y + 1] = b[3];
            color[tetris->x][tetris->y - 1] = color[tetris->x][tetris->y + 1] = color[tetris->x + 2][tetris->y + 1] = 12;
            break;
        }
        case 15://J右侧倾倒
        {
            SetColor(12);
            a[tetris->x - 2][tetris->y] = b[1];
            a[tetris->x - 2][tetris->y + 1] = b[2];
            a[tetris->x + 2][tetris->y] = b[3];
            color[tetris->x - 2][tetris->y] = color[tetris->x - 2][tetris->y + 1] = color[tetris->x + 2][tetris->y] = 12;
            break;
        }
        case 16://
        {
            SetColor(9);
            a[tetris->x][tetris->y + 1] = b[1];
            a[tetris->x][tetris->y - 1] = b[2];
            a[tetris->x + 2][tetris->y - 1] = b[3];
            color[tetris->x][tetris->y + 1] = color[tetris->x][tetris->y - 1] = color[tetris->x + 2][tetris->y - 1] = 9;
            break;
        }
        case 17://
        {
            SetColor(9);
            a[tetris->x - 2][tetris->y] = b[1];
            a[tetris->x + 2][tetris->y + 1] = b[2];
            a[tetris->x + 2][tetris->y] = b[3];
            color[tetris->x - 2][tetris->y] = color[tetris->x + 2][tetris->y + 1] = color[tetris->x + 2][tetris->y] = 9;
            break;
        }
        case 18:
        {
            SetColor(9);
            a[tetris->x][tetris->y - 1] = b[1];
            a[tetris->x][tetris->y + 1] = b[2];
            a[tetris->x - 2][tetris->y + 1] = b[3];
            color[tetris->x][tetris->y - 1] = color[tetris->x][tetris->y + 1] = color[tetris->x - 2][tetris->y + 1] = 9;
            break;
        }
        case 19:
        {
            SetColor(9);
            a[tetris->x - 2][tetris->y] = b[1];
            a[tetris->x - 2][tetris->y - 1] = b[2];
            a[tetris->x + 2][tetris->y] = b[3];
            color[tetris->x - 2][tetris->y] = color[tetris->x - 2][tetris->y - 1] = color[tetris->x + 2][tetris->y] = 9;
            break;
        }
    }
}

//删除图形
void DeleteBlock(struct tetris* tetris)
{
    HideCursor();
    for (i = 0; i < 4; i++)
    {
        b[i] = 0;
    }

    MakeBlock(tetris);

    for (i = tetris->x - 2; i <= tetris->x + 4; i += 2)
        for (j = tetris->y - 2; j <= tetris->y+ 1; j++)
        {
            if(a[i][j]==0&&j>ContainerY)
            {
                ald[i][j] = 0;
                Gotoxy(i,j);
                cout << "  ";
            }
        }
}

void WelCome()
{
    HideCursor();
    int n;
    int i, j = 1;
    SetColor(14);
    for (int i = 9; i <= 20;i++)
        for (int j = 15; j <= 60;j++)
        {
            Gotoxy(j, i);
            if(i==9||i==20)
                cout << "=";
            else if(j==15||j==59)
                cout << "||";
        }
    SetColor(12);
    Gotoxy(25, 12);
    cout << "1、开始游戏";
    Gotoxy(40, 12);
    cout << "2、按键说明";
    Gotoxy(25, 17);
    cout << "3、游戏规则";
    Gotoxy(40, 17);
    cout << "4、退出";
    //Gotoxy(21, 22);
    //cout<<"请选择";
    SetColor(14);
    cin >> n;
    switch(n)
    {
        case 1:
            system("cls");
            DrawFrame();
            GamePlay();
            break;
        case 2:
            Explanation();
            break;
        case 3:
            Regulation();
            break;
        case 4:
            Close();
            break;
    }
}

//假想移动一步后验证判断能否移动
int CanMove(struct tetris*tetris)
{
   if(a[tetris->x][tetris->y]!=0)
        return 0;
    else 
    {
        if((tetris->flag==1&&(a[tetris->x][tetris->y-1]==0&&a[tetris->x+2][tetris->y-1]==0&&a[tetris->x+2][tetris->y]==0) ) ||
           (tetris->flag==2&&(a[tetris->x-2][tetris->y]==0&&a[tetris->x+2][tetris->y]==0&&a[tetris->x+4][tetris->y]==0) ) ||
           (tetris->flag==3&&(a[tetris->x][tetris->y-1]==0&&a[tetris->x][tetris->y-2]==0&&a[tetris->x][tetris->y+1]==0) ) ||
           (tetris->flag==4&&(a[tetris->x-2][tetris->y]==0&&a[tetris->x+2][tetris->y]==0&&a[tetris->x][tetris->y+1]==0) ) ||
           (tetris->flag==5&&(a[tetris->x][tetris->y-1]==0&&a[tetris->x][tetris->y+1]==0&&a[tetris->x-2][tetris->y]==0) ) ||
           (tetris->flag==6&&(a[tetris->x][tetris->y-1]==0&&a[tetris->x-2][tetris->y]==0&&a[tetris->x+2][tetris->y]==0) ) ||
           (tetris->flag==7&&(a[tetris->x][tetris->y-1]==0&&a[tetris->x][tetris->y+1]==0&&a[tetris->x+2][tetris->y]==0) ) ||
           (tetris->flag==8&&(a[tetris->x][tetris->y+1]==0&&a[tetris->x-2][tetris->y]==0&&a[tetris->x+2][tetris->y+1]==0) ) ||
           (tetris->flag==9&&(a[tetris->x][tetris->y-1]==0&&a[tetris->x-2][tetris->y-1]==0&&a[tetris->x-2][tetris->y+1]==0) ) ||
           (tetris->flag==10&&(a[tetris->x][tetris->y-1]==0&&a[tetris->x-2][tetris->y-1]==0&&a[tetris->x+2][tetris->y]==0) ) ||
           (tetris->flag==11&&(a[tetris->x][tetris->y+1]==0&&a[tetris->x-2][tetris->y-1]==0&&a[tetris->x-2][tetris->y]==0) ) ||
           (tetris->flag==12&&(a[tetris->x][tetris->y-1]==0&&a[tetris->x][tetris->y+1]==0&&a[tetris->x-2][tetris->y-1]==0) ) ||
           (tetris->flag==15&&(a[tetris->x-2][tetris->y]==0&&a[tetris->x-2][tetris->y+1]==0&&a[tetris->x+2][tetris->y]==0) ) ||
           (tetris->flag==14&&(a[tetris->x][tetris->y-1]==0&&a[tetris->x][tetris->y+1]==0&&a[tetris->x+2][tetris->y+1]==0) ) ||
           (tetris->flag==13&&(a[tetris->x-2][tetris->y]==0&&a[tetris->x+2][tetris->y-1]==0&&a[tetris->x+2][tetris->y]==0) ) ||
           (tetris->flag==16&&(a[tetris->x][tetris->y+1]==0&&a[tetris->x][tetris->y-1]==0&&a[tetris->x+2][tetris->y-1]==0) ) ||
           (tetris->flag==19&&(a[tetris->x-2][tetris->y]==0&&a[tetris->x-2][tetris->y-1]==0&&a[tetris->x+2][tetris->y]==0) ) ||
           (tetris->flag==18&&(a[tetris->x][tetris->y-1]==0&&a[tetris->x][tetris->y+1]==0&&a[tetris->x-2][tetris->y+1]==0) ) ||
           (tetris->flag==17&&(a[tetris->x-2][tetris->y]==0&&a[tetris->x+2][tetris->y+1]==0&&a[tetris->x+2][tetris->y]==0) ))
            {    
                return 1; 
            }
    }
    return 0;
}

void IfFulline(struct tetris*tetris)
{
    HideCursor();
    int Rows = 0;//清除的行数
    int k;       //存在的块数
    for (j = ContainerY + Container_Height - 1; j >= ContainerY + 1; j--)//下边框到上边框
    {
        k = 0;
        for (i = ContainerX + 2; i < ContainerX + 2 * Container_Width - 2; i += 2) //左边框到有边框
        {
            if(a[i][j])
            {
                k++;
                if(k==Container_Width-2)//满行
                {
                    //清行
                    for (k = ContainerX + 2; k < ContainerX + 2 * Container_Width - 2; k += 2)
                    {
                        a[k][j] = 0;
                        ald[k][j] = 0;
                        Gotoxy(k, j);
                        cout << "  ";
                    }
                    //上方行集体落下
                    for (int k = j - 1; k > ContainerY; k--)
                    {
                        for (i = ContainerX + 2; i < ContainerX + 2 * Container_Width - 2; i += 2)
                        {
                            if(a[i][k]==1)
                            {
                                a[i][k] = 0;
                                ald[i][k] = 0;
                                Gotoxy(i, k);
                                cout << "  ";
                                a[i][k + 1] = 1;
                                ald[i][k + 1] = 1;
                                Gotoxy(i,k+1);
                                SetColor(color[i][k]);
                                cout << "■";
                            }
                        }
                    }
                    j++;//因为每次清行以后所有行纵坐标加，所以再一次比较的时候被比较行纵坐标加1
                    Rows++; //分数+100;
                }
            }
        }
    }
    Score += 100 * Rows; //每消去1行，加100分
    if(Rows>0&&(Score%1000==0||Score/1000>Level-1))
    {
        Speed -= 30;//每1000分，延迟速度减少30ms
        Level++;    //升级
    }
}

void Explanation()
{
    HideCursor();
    Gotoxy(40, 12);
    cout << "暂未开放" << endl;
    system("pause");
    system("cls");
    WelCome();
    //return;
}


void Regulation()
{
    HideCursor();
    Gotoxy(40, 12);
    cout<<"暂未开放"<<endl;
    system("pause");
    system("cls");
    WelCome();
    //return;
}

void Replay()
{
    system("cls");
    memset(a, 0, 6400 * sizeof(int));
    DrawFrame();
    GamePlay();
}

void Close()
{
    exit(0);
}

//设置颜色,说明：0 黑色1 蓝色2 绿色3 青色 4红色5 紫色6 黄色7 白色 + 8加强色
int SetColor(int Color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Color);
    return 0;
}


//隐藏光标
void HideCursor()
{
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}


//这里涉及到一个关键问题，就是头结点和非头结点的第一个结点,所以对于一个指针来讲申请两次
void CreateFlag()
{
    if(Contact==0)
    {
        srand(time(NULL));
        //工具指针与尾指针分配内存
        Pa_Tetris = new Tetris;
        Prear_Tetris = new Tetris;//
        Pa_Tetris->flag = rand() % 19 + 1;//随机设置哪种类型方块

        //生成一个新结点
        Pa_Tetris->Next = NULL;
        Phead_Tetris = Pa_Tetris;
        Prear_Tetris = Pa_Tetris;

        //重新申请内存,创建头结点后第一个结点
        Pa_Tetris = new Tetris;
        Pa_Tetris->flag = rand() % 19 + 1;
        Pa_Tetris->Next = NULL;
        Prear_Tetris->Next = Pa_Tetris;//把rear头结点接过来
        Prear_Tetris = Pa_Tetris;//移动rear指针到非头1号结点

        //Ptemp代替head结点
        //后期流程只使用temp（现在）和rear（未来）指针
        PTemp_Tetris = new Tetris;
        PTemp_Tetris = Phead_Tetris;
        //head?
    }
    else//在游戏中的一般情况下的方块链表衔接
    {
        PTemp_Tetris = Prear_Tetris;
        //三件套格式
        Pa_Tetris=new Tetris;
        Pa_Tetris->flag = rand() % 19 + 1;
        Pa_Tetris->Next = NULL;
        Prear_Tetris->Next = Pa_Tetris;//非头1号结点
        Prear_Tetris = Pa_Tetris;
    }
}

void GamePlay()
{
    HideCursor();
    int n;
    char ch;//接收键盘输入
    Contact = 0;//开没开始
    CreateFlag();//头结点和非头1号结点
    Level = 1;
    Score = 0;
    Num = 1;
    Speed = 200;
    goto First;
    while(1)
    {
        Contact = 1;
        CreateFlag();//非头1号结点
        First:
            //定位等待方块
            Prear_Tetris->x = ContainerX + 2 * Container_Width + 6;
            Prear_Tetris->y = ContainerY + 10;
            //打印等待方块
            PrintBlock(Prear_Tetris);
            //定位下落方块
            PTemp_Tetris->x = ContainerX + Container_Width;
            PTemp_Tetris->y = ContainerY - 1;
            while(1)//操作正在下落的方块
            {
                Label:
                    PrintBlock(PTemp_Tetris);//打印下落方块
                    Sleep(Speed);//延迟（ms）
                    DeleteBlock(PTemp_Tetris);//每移动一次都需清除之前的print
                    Temp1 = PTemp_Tetris->x;
                    Temp2 = PTemp_Tetris->flag;
                    if(_kbhit())
                    {
                        ch = _getch();//等待键盘指令
                        if(ch==75)//左
                        {
                            PTemp_Tetris->x -= 2;
                        }
                        if(ch==77)//右
                        {
                            PTemp_Tetris->x += 2;
                        }
                        if(ch==80)//下（加速）
                        {
                            if(CanMove(PTemp_Tetris)!=0)
                            {
                                PTemp_Tetris->y += 2;
                            }
                            if(CanMove(PTemp_Tetris)==0)
                            {
                                PTemp_Tetris->y = ContainerY + Container_Height - 2;
                            }
                        }
                        if(ch==72)//旋转
                        {
                            //flag==1不用旋转
                            if(PTemp_Tetris->flag>=2&&PTemp_Tetris->flag<=3)
                            {
                                //多种状态不断切换
                                PTemp_Tetris->flag++;
                                PTemp_Tetris->flag %= 2;
                                PTemp_Tetris->flag += 2;
                            }
                            if(PTemp_Tetris->flag>=4&&PTemp_Tetris->flag<=7)
                            {
                                PTemp_Tetris->flag++;
                                PTemp_Tetris->flag %= 4;
                                PTemp_Tetris->flag += 4;
                            }
                            if(PTemp_Tetris->flag>=8&&PTemp_Tetris->flag<=11)
                            {
                                PTemp_Tetris->flag++;
                                PTemp_Tetris->flag %= 4;
                                PTemp_Tetris->flag += 8;
                            }
                            if(PTemp_Tetris->flag>=12&&PTemp_Tetris->flag<=15)
                            {
                                PTemp_Tetris->flag++;
                                PTemp_Tetris->flag %= 4;
                                PTemp_Tetris->flag += 12;
                            }
                            if(PTemp_Tetris->flag>=16&&PTemp_Tetris->flag<=19)
                            {
                                PTemp_Tetris->flag++;
                                PTemp_Tetris->flag %= 4;
                                PTemp_Tetris->flag += 16;
                            }
                        }
                        if(ch==32)//暂停
                        {
                            PrintBlock(PTemp_Tetris);
                            while(1)
                            {
                                if(_kbhit())
                                {
                                    ch = _getch();
                                    if(ch==32)
                                    {
                                        goto Label;
                                    }
                                }
                            }
                        }
                    
                        if(ch==27)//Esc
                        {
                            system("cls");
                            memset(a, 0, 6400 * sizeof(int));
                            WelCome();
                        }
                        if(CanMove(PTemp_Tetris)==0)
                        {
                            //两个Temp,此次键盘指令无影响
                            PTemp_Tetris->x = Temp1;
                            PTemp_Tetris->flag = Temp2;
                        }
                        else
                        {
                            goto Label;//继续下落
                        }
                    }//一次键盘指令
                     //一次指令后下落一个单位
                    PTemp_Tetris->y++;
                    if(CanMove(PTemp_Tetris)==0)
                    {
                        //如果不能下落了，消除++
                        PTemp_Tetris->y--;
                        PrintBlock(PTemp_Tetris);
                        //每次下落到不能下落，判断是否已经满行
                        IfFulline(PTemp_Tetris);
                        break;
                        //对于这个方块的全部操作结束了
                    }
            }

            for (i = PTemp_Tetris->y - 2; i < PTemp_Tetris->y + 2;i++)
            {
                if(i==ContainerY)
                {
                    system("cls");
                    Gotoxy(29, 7);
                    cout << "  " << '\n';
                    SetColor(12);
                    cout << '\t' << '\t' << '\t' << "" << '\n';
                    //还有几行cout没写
                    Gotoxy(17, 18);
                    SetColor(14);
                    cout << "重玩-----------------1";

                    Gotoxy(17, 21);
                    cout << "退出-----------------2" << '\n';
                    int n;
                    Gotoxy(32, 20);
                    cout << "选择【1/2】：";
                    SetColor(11);
                    cin >> n;
                    switch(n)
                    {
                        case 1:
                            system("cls");
                            Replay();
                            break;
                        case 2:
                            exit(0);
                            break;
                    }
                }
            }

        PTemp_Tetris->flag = Prear_Tetris->flag;
        PTemp_Tetris->x = ContainerX + 2 * Container_Width + 6;
        PTemp_Tetris->y = ContainerY + 10;
        DeleteBlock(PTemp_Tetris);
    }
}