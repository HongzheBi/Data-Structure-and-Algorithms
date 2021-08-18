#include <iostream>
#include <string>
#include <cmath>

using namespace std;

#define maxsize 1000

template <class T>
class SeqList
{
    public:
        T Data[maxsize];
        SeqList();//无参构造
        SeqList(const T bi[], int n);//有参构造
        void PrintList();//打印顺序表
        void Insert(T x);//末尾插入元素
        int GetLength();//顺序表长度

    private:
        int Length; //顺序表长度
};

//无参构造
template <class T>
SeqList<T>::SeqList()
{
    Length = 0;
}

//有参构造
template <class T>
SeqList<T>::SeqList(const T bi[],int n)
{
    //异常处理
    if(n>maxsize)
    {
        throw "超长";
    }
    else
    {
        for (int j = 0; j < n; j++)
        {
            Data[j] = bi[j];
        }
        Length = n;
    }
}

//顺序表长度
template <class T>
int SeqList<T>::GetLength()
{
    return Length;
}

//末尾插入
template<class T>
void SeqList<T>::Insert(T x)
{
    int i = GetLength();//当前长度
    Data[i] = x;
    Length++;
}

//打印顺序表
template<class T>
void SeqList<T>::PrintList()
{
    for (int i = 0; i < Length; i++)
    {
        cout << Data[i] << '\n';
    }
}

//节点信息
struct node
{
    int Loc;     //位置
    int Degree;  //连接边数
    int Color;   //颜色
};

int ColorToGo[13] = {0};

//邻接矩阵
int Map[13][13] =
{
	0,0,0,0,1,1,1,0,0,1,0,0,0,
	0,0,0,0,0,1,0,1,0,1,1,0,0,
	0,0,0,0,0,0,0,0,0,1,1,1,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,
	1,0,0,0,0,0,0,1,0,0,1,0,0,
	1,1,0,0,0,0,1,0,0,0,1,1,0,
	1,1,0,0,0,1,0,0,0,0,1,1,0,
	0,1,0,0,1,0,0,0,0,0,0,1,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,
	1,1,1,0,0,0,0,0,0,0,0,0,0,
	0,1,1,0,1,1,1,0,0,0,0,0,0,
	0,0,1,0,0,1,1,1,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,
};
//Line:AB AC AD BA BC BD DA DB DC EA EB EC ED

//节点度降序排序
void ShellSort(SeqList<node> Node,int n)
{
    for (int d = n / 2; d >= 1; d = d / 2)//增量大小递减
    {
        for (int i = d + 1; i <= n; i++)
        {
            if(Node.Data[i].Degree>Node.Data[i-1].Degree)
            {
                node Temp = Node.Data[i];
                int j = i - d;
                //依次按照间隔往前查找
                for (; Temp.Degree > Node.Data[j].Degree && j > 0; j -= d)
                {
                    Node.Data[j + d] = Node.Data[j];
                }
                Node.Data[j + d] = Temp;
            }
        }
    }
}

//打印方案
void Print(int List[],int n)
{
    //存储交通灯种类
    string OutColor[5] = {"蓝灯","红灯","绿灯","黄灯","紫灯" };
    //存储可连通方案
    string Way[13] = {"AB","AC","AD","BA","BC","BD","DA","DB","DC","EA","EB","EC","ED"};

    int NumPrint = 0;//已经打印的道路数量
    int NumColor = 1;//正在寻找的灯的颜色

    //遍历灯的颜色
    while((NumPrint<n)&&(NumColor<=5))
    {
        cout << OutColor[NumColor - 1] << "亮灯通行的道路为：";
        //遍历通路，找出与灯的颜色对应的通路
        for (int i = 0; i < n; i++)
        {
            if (List[i]==NumColor)
            {
                cout << Way[i] << " ";
                NumPrint++;
            }
        }
        cout << '\n';
        NumColor++;
    }
}

//填充颜色
void FillColor(int n)
{
    SeqList<int> Color;//建立容纳颜色的线性表
    SeqList<node> Node;//建立容纳节点信息的线性表
    Color.Insert(1);//先装入一种颜色
    //把节点信息装入顺序表
    for (int i = 0; i < n; i++)
    {
        node Temp;
        Temp.Color = 0;
        Temp.Loc = i;
        int DegreeI = 0;
        //计算节点的度
        for (int j = 0; j < n; j++)
        {
            if(Map[i][j])
            {
                DegreeI++;
            }
        }
        Temp.Degree = DegreeI;
        Node.Insert(Temp);//节点信息存入线性表
    }

    ShellSort(Node, n);

    //填充颜色
    for (int i = 0; i < n; i++)
    {
        bool Flag;//判断是否颜色冲突
        int Loc = Node.Data[i].Loc;//获得节点对应邻接矩阵下标
        //先遍历已有颜色，判断是否冲突,不冲突则用已有颜色
        for (int j = 0; j < Color.GetLength(); j++)
        {
            ColorToGo[Loc] = Color.Data[j];
            int Num = 0;
            Flag = false;
            while(Num<n)
            {
                int Temp = Map[Loc][Num];
                if((Temp==1)&&(ColorToGo[Num]==ColorToGo[Loc]))
                {
                    Flag = true;//颜色冲突时，标志位改变
                }
                Num++;
            }
            //如果不冲突
            if(!Flag)
            {
                break;
            }
        }

        //如果所有颜色冲突，则加入一个新颜色并填充
        if(Flag)
        {
            int NowMax = Color.GetLength();
            Color.Insert(NowMax + 1);//把新颜色装入顺序表
            ColorToGo[Loc] = NowMax + 1;
        }
    }
}

int main()
{
    FillColor(13);//填充颜色
    Print(ColorToGo,13);//打印通行方案
    system("pause");
    return 0;
}