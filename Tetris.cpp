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

int b[4];//һ������4��
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

struct tetris *Pa_Tetris;//����ָ��
struct tetris *Prear_Tetris;//βָ��
struct tetris *Phead_Tetris;//ͷָ��
struct tetris *PTemp_Tetris;//��ǰ����
int Contact; //0/1����

HANDLE hOut;

//��������
void Gotoxy(int x, int y);//�ƶ����
void DrawFrame();//��ӡ��Ϸ�߿�
void CreateFlag();//���������ؼ�������
void MakeBlock(struct tetris *); //��������
void PrintBlock(struct tetris *);//��ӡ����
void DeleteBlock(struct tetris *);//���
int  CanMove(struct tetris *);    //�ж��Ƿ�����ƶ�
void IfFulline(struct tetris *); //�ж��Ƿ�����
void GamePlay();//��ʼ��Ϸ

void Regulation();//��Ϸ������δ���ţ�
void Explanation();//��ؽ��ͣ���δ���ţ�
void Replay();//����
void WelCome();//������
void Title();//��ӡ����
void Close();//�˳�
int SetColor(int color);//������ɫ
//void Flower();
void HideCursor();//���ع��

int main()
{
    WelCome();
    return 0;
}

//ָ��λ�����
void Gotoxy(int x, int y)
{
	COORD Pos;
	Pos.X = x;
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

//��ӡ��Ϸ�߿�
void DrawFrame()
{
    HideCursor();
    Gotoxy(ContainerX+Container_Width-5,ContainerY-2);
    SetColor(11);
    cout << "����˹����";
    Gotoxy(ContainerX+2*Container_Width+3,ContainerY+7);
    SetColor(2);
    cout << "**********";
    Gotoxy(ContainerX+2*Container_Width+13,ContainerY+7);
    SetColor(3);
    cout << "������һ������";
    Gotoxy(ContainerX+2*Container_Width+3,ContainerY+13);
    SetColor(2);
    cout << "**********";
    Gotoxy(ContainerX+2*Container_Width+3,ContainerY+17);
    SetColor(14);
    cout << "��������ת";
    Gotoxy(ContainerX+2*Container_Width+3,ContainerY+19);
    cout << "�ո���ͣ��Ϸ";
    Gotoxy(ContainerX+2*Container_Width+3,ContainerY+15);
    cout << "Esc���˳���Ϸ";
    //Gotoxy(ContainerX-1,ContainerY);
    Gotoxy(ContainerX,ContainerY);
    SetColor(12);
    cout << "��-";
    Gotoxy(ContainerX+2*Container_Width-2,ContainerY);
    cout << "��";
    //Gotoxy(ContainerX-1,ContainerY+Container_Height);
    Gotoxy(ContainerX,ContainerY+Container_Height);
    cout << "��-";
    Gotoxy(ContainerX+2*Container_Width-2,ContainerY+Container_Height);
    cout << "��";

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
                cout << "��";
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

//��������
void MakeBlock(struct tetris* tetris)
{
    a[tetris->x][tetris->y] = b[0];
    switch(tetris->flag)
    {
        case 1://���ָ�ֻ��һ��
        {
            SetColor(10);
            a[tetris->x][tetris->y - 1] = b[1];
            a[tetris->x+2][tetris->y-1] = b[2];
            a[tetris->x + 2][tetris->y] = b[3];
            color[tetris->x][tetris->y - 1] = color[tetris->x + 2][tetris->y - 1] = color[tetris->x + 2][tetris->y] = 10;
            break;
        }
        case 2://��
        {
            SetColor(13);
            a[tetris->x - 2][tetris->y] = b[1];
            a[tetris->x + 2][tetris->y] = b[2];
            a[tetris->x + 4][tetris->y] = b[3];
            color[tetris->x - 2][tetris->y] = color[tetris->x + 2][tetris->y] = color[tetris->x + 4][tetris->y] = 13;
            break;
        }
        case 3://��
        {
            SetColor(13);
            a[tetris->x][tetris->y - 1] = b[1];
            a[tetris->x][tetris->y - 2] = b[2];
            a[tetris->x][tetris->y + 1] = b[3];
            color[tetris->x][tetris->y - 1] = color[tetris->x][tetris->y - 2] = color[tetris->x][tetris->y + 1] = 13;
            break;
        }
        case 4://��ɽ
        {
            SetColor(11);
            a[tetris->x - 2][tetris->y] = b[1];
            a[tetris->x + 2][tetris->y] = b[2];
            a[tetris->x][tetris->y + 1] = b[3];
            color[tetris->x - 2][tetris->y] = color[tetris->x + 2][tetris->y] = color[tetris->x][tetris->y + 1] = 11;
            break;
        }
        case 5://��ɽ
        {
            SetColor(11);
            a[tetris->x][tetris->y - 1] = b[1];
            a[tetris->x][tetris->y + 1] = b[2];
            a[tetris->x - 2][tetris->y] = b[3];
            color[tetris->x][tetris->y - 1] = color[tetris->x][tetris->y + 1] = color[tetris->x - 2][tetris->y] = 11;
            break;
        }
        case 6://��ɽ
        {
            SetColor(11);
            a[tetris->x][tetris->y - 1] = b[1];
            a[tetris->x - 2][tetris->y] = b[2];
            a[tetris->x + 2][tetris->y] = b[3];
            color[tetris->x][tetris->y - 1] = color[tetris->x - 2][tetris->y] = color[tetris->x + 2][tetris->y] = 11;
            break;
        }
        case 7://��ɽ
        {
            
            SetColor(11);
            a[tetris->x][tetris->y - 1] = b[1];
            a[tetris->x][tetris->y + 1] = b[2];
            a[tetris->x + 2][tetris->y] = b[3];
            color[tetris->x][tetris->y - 1] = color[tetris->x][tetris->y + 1] = color[tetris->x + 2][tetris->y] = 11;
            break;
        }
        case 8://��s
        {
            SetColor(14);
            a[tetris->x][tetris->y + 1] = b[1];
            a[tetris->x - 2][tetris->y] = b[2];
            a[tetris->x + 2][tetris->y + 1] = b[3];
            color[tetris->x][tetris->y + 1] = color[tetris->x - 2][tetris->y] = color[tetris->x + 2][tetris->y + 1] = 14;
            break;
        }
        case 9://��s
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
        case 11://��z
        {
            SetColor(14);
            a[tetris->x][tetris->y + 1] = b[1];
            a[tetris->x - 2][tetris->y - 1] = b[2];
            a[tetris->x - 2][tetris->y] = b[3];
            color[tetris->x][tetris->y + 1] = color[tetris->x - 2][tetris->y - 1] = color[tetris->x - 2][tetris->y] = 14;
            break;
        }
        case 12://��L
        {
            SetColor(12);
            a[tetris->x][tetris->y - 1] = b[1];
            a[tetris->x][tetris->y + 1] = b[2];
            a[tetris->x - 2][tetris->y - 1] = b[3];
            color[tetris->x][tetris->y - 1] = color[tetris->x][tetris->y + 1] = color[tetris->x - 2][tetris->y - 1] = 12;
            break;
        }
        case 13://J��
        {
            SetColor(12);
            a[tetris->x - 2][tetris->y] = b[1];
            a[tetris->x + 2][tetris->y - 1] = b[2];
            a[tetris->x + 2][tetris->y] = b[3];
            color[tetris->x - 2][tetris->y] = color[tetris->x + 2][tetris->y - 1] = color[tetris->x + 2][tetris->y] = 12;
            break;
        }
        case 14://��תL
        {
            SetColor(12);
            a[tetris->x][tetris->y - 1] = b[1];
            a[tetris->x][tetris->y + 1] = b[2];
            a[tetris->x + 2][tetris->y + 1] = b[3];
            color[tetris->x][tetris->y - 1] = color[tetris->x][tetris->y + 1] = color[tetris->x + 2][tetris->y + 1] = 12;
            break;
        }
        case 15://J�Ҳ��㵹
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

//ɾ��ͼ��
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
    cout << "1����ʼ��Ϸ";
    Gotoxy(40, 12);
    cout << "2������˵��";
    Gotoxy(25, 17);
    cout << "3����Ϸ����";
    Gotoxy(40, 17);
    cout << "4���˳�";
    //Gotoxy(21, 22);
    //cout<<"��ѡ��";
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

//�����ƶ�һ������֤�ж��ܷ��ƶ�
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
    int Rows = 0;//���������
    int k;       //���ڵĿ���
    for (j = ContainerY + Container_Height - 1; j >= ContainerY + 1; j--)//�±߿��ϱ߿�
    {
        k = 0;
        for (i = ContainerX + 2; i < ContainerX + 2 * Container_Width - 2; i += 2) //��߿��б߿�
        {
            if(a[i][j])
            {
                k++;
                if(k==Container_Width-2)//����
                {
                    //����
                    for (k = ContainerX + 2; k < ContainerX + 2 * Container_Width - 2; k += 2)
                    {
                        a[k][j] = 0;
                        ald[k][j] = 0;
                        Gotoxy(k, j);
                        cout << "  ";
                    }
                    //�Ϸ��м�������
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
                                cout << "��";
                            }
                        }
                    }
                    j++;//��Ϊÿ�������Ժ�������������ӣ�������һ�αȽϵ�ʱ�򱻱Ƚ����������1
                    Rows++; //����+100;
                }
            }
        }
    }
    Score += 100 * Rows; //ÿ��ȥ1�У���100��
    if(Rows>0&&(Score%1000==0||Score/1000>Level-1))
    {
        Speed -= 30;//ÿ1000�֣��ӳ��ٶȼ���30ms
        Level++;    //����
    }
}

void Explanation()
{
    HideCursor();
    Gotoxy(40, 12);
    cout << "��δ����" << endl;
    system("pause");
    system("cls");
    WelCome();
    //return;
}


void Regulation()
{
    HideCursor();
    Gotoxy(40, 12);
    cout<<"��δ����"<<endl;
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

//������ɫ,˵����0 ��ɫ1 ��ɫ2 ��ɫ3 ��ɫ 4��ɫ5 ��ɫ6 ��ɫ7 ��ɫ + 8��ǿɫ
int SetColor(int Color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Color);
    return 0;
}


//���ع��
void HideCursor()
{
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}


//�����漰��һ���ؼ����⣬����ͷ���ͷ�ͷ���ĵ�һ�����,���Զ���һ��ָ��������������
void CreateFlag()
{
    if(Contact==0)
    {
        srand(time(NULL));
        //����ָ����βָ������ڴ�
        Pa_Tetris = new Tetris;
        Prear_Tetris = new Tetris;//
        Pa_Tetris->flag = rand() % 19 + 1;//��������������ͷ���

        //����һ���½��
        Pa_Tetris->Next = NULL;
        Phead_Tetris = Pa_Tetris;
        Prear_Tetris = Pa_Tetris;

        //���������ڴ�,����ͷ�����һ�����
        Pa_Tetris = new Tetris;
        Pa_Tetris->flag = rand() % 19 + 1;
        Pa_Tetris->Next = NULL;
        Prear_Tetris->Next = Pa_Tetris;//��rearͷ���ӹ���
        Prear_Tetris = Pa_Tetris;//�ƶ�rearָ�뵽��ͷ1�Ž��

        //Ptemp����head���
        //��������ֻʹ��temp�����ڣ���rear��δ����ָ��
        PTemp_Tetris = new Tetris;
        PTemp_Tetris = Phead_Tetris;
        //head?
    }
    else//����Ϸ�е�һ������µķ��������ν�
    {
        PTemp_Tetris = Prear_Tetris;
        //�����׸�ʽ
        Pa_Tetris=new Tetris;
        Pa_Tetris->flag = rand() % 19 + 1;
        Pa_Tetris->Next = NULL;
        Prear_Tetris->Next = Pa_Tetris;//��ͷ1�Ž��
        Prear_Tetris = Pa_Tetris;
    }
}

void GamePlay()
{
    HideCursor();
    int n;
    char ch;//���ռ�������
    Contact = 0;//��û��ʼ
    CreateFlag();//ͷ���ͷ�ͷ1�Ž��
    Level = 1;
    Score = 0;
    Num = 1;
    Speed = 200;
    goto First;
    while(1)
    {
        Contact = 1;
        CreateFlag();//��ͷ1�Ž��
        First:
            //��λ�ȴ�����
            Prear_Tetris->x = ContainerX + 2 * Container_Width + 6;
            Prear_Tetris->y = ContainerY + 10;
            //��ӡ�ȴ�����
            PrintBlock(Prear_Tetris);
            //��λ���䷽��
            PTemp_Tetris->x = ContainerX + Container_Width;
            PTemp_Tetris->y = ContainerY - 1;
            while(1)//������������ķ���
            {
                Label:
                    PrintBlock(PTemp_Tetris);//��ӡ���䷽��
                    Sleep(Speed);//�ӳ٣�ms��
                    DeleteBlock(PTemp_Tetris);//ÿ�ƶ�һ�ζ������֮ǰ��print
                    Temp1 = PTemp_Tetris->x;
                    Temp2 = PTemp_Tetris->flag;
                    if(_kbhit())
                    {
                        ch = _getch();//�ȴ�����ָ��
                        if(ch==75)//��
                        {
                            PTemp_Tetris->x -= 2;
                        }
                        if(ch==77)//��
                        {
                            PTemp_Tetris->x += 2;
                        }
                        if(ch==80)//�£����٣�
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
                        if(ch==72)//��ת
                        {
                            //flag==1������ת
                            if(PTemp_Tetris->flag>=2&&PTemp_Tetris->flag<=3)
                            {
                                //����״̬�����л�
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
                        if(ch==32)//��ͣ
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
                            //����Temp,�˴μ���ָ����Ӱ��
                            PTemp_Tetris->x = Temp1;
                            PTemp_Tetris->flag = Temp2;
                        }
                        else
                        {
                            goto Label;//��������
                        }
                    }//һ�μ���ָ��
                     //һ��ָ�������һ����λ
                    PTemp_Tetris->y++;
                    if(CanMove(PTemp_Tetris)==0)
                    {
                        //������������ˣ�����++
                        PTemp_Tetris->y--;
                        PrintBlock(PTemp_Tetris);
                        //ÿ�����䵽�������䣬�ж��Ƿ��Ѿ�����
                        IfFulline(PTemp_Tetris);
                        break;
                        //������������ȫ������������
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
                    //���м���coutûд
                    Gotoxy(17, 18);
                    SetColor(14);
                    cout << "����-----------------1";

                    Gotoxy(17, 21);
                    cout << "�˳�-----------------2" << '\n';
                    int n;
                    Gotoxy(32, 20);
                    cout << "ѡ��1/2����";
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