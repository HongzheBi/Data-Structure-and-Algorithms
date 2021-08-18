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
        SeqList();//�޲ι���
        SeqList(const T bi[], int n);//�вι���
        void PrintList();//��ӡ˳���
        void Insert(T x);//ĩβ����Ԫ��
        int GetLength();//˳�����

    private:
        int Length; //˳�����
};

//�޲ι���
template <class T>
SeqList<T>::SeqList()
{
    Length = 0;
}

//�вι���
template <class T>
SeqList<T>::SeqList(const T bi[],int n)
{
    //�쳣����
    if(n>maxsize)
    {
        throw "����";
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

//˳�����
template <class T>
int SeqList<T>::GetLength()
{
    return Length;
}

//ĩβ����
template<class T>
void SeqList<T>::Insert(T x)
{
    int i = GetLength();//��ǰ����
    Data[i] = x;
    Length++;
}

//��ӡ˳���
template<class T>
void SeqList<T>::PrintList()
{
    for (int i = 0; i < Length; i++)
    {
        cout << Data[i] << '\n';
    }
}

//�ڵ���Ϣ
struct node
{
    int Loc;     //λ��
    int Degree;  //���ӱ���
    int Color;   //��ɫ
};

int ColorToGo[13] = {0};

//�ڽӾ���
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

//�ڵ�Ƚ�������
void ShellSort(SeqList<node> Node,int n)
{
    for (int d = n / 2; d >= 1; d = d / 2)//������С�ݼ�
    {
        for (int i = d + 1; i <= n; i++)
        {
            if(Node.Data[i].Degree>Node.Data[i-1].Degree)
            {
                node Temp = Node.Data[i];
                int j = i - d;
                //���ΰ��ռ����ǰ����
                for (; Temp.Degree > Node.Data[j].Degree && j > 0; j -= d)
                {
                    Node.Data[j + d] = Node.Data[j];
                }
                Node.Data[j + d] = Temp;
            }
        }
    }
}

//��ӡ����
void Print(int List[],int n)
{
    //�洢��ͨ������
    string OutColor[5] = {"����","���","�̵�","�Ƶ�","�ϵ�" };
    //�洢����ͨ����
    string Way[13] = {"AB","AC","AD","BA","BC","BD","DA","DB","DC","EA","EB","EC","ED"};

    int NumPrint = 0;//�Ѿ���ӡ�ĵ�·����
    int NumColor = 1;//����Ѱ�ҵĵƵ���ɫ

    //�����Ƶ���ɫ
    while((NumPrint<n)&&(NumColor<=5))
    {
        cout << OutColor[NumColor - 1] << "����ͨ�еĵ�·Ϊ��";
        //����ͨ·���ҳ���Ƶ���ɫ��Ӧ��ͨ·
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

//�����ɫ
void FillColor(int n)
{
    SeqList<int> Color;//����������ɫ�����Ա�
    SeqList<node> Node;//�������ɽڵ���Ϣ�����Ա�
    Color.Insert(1);//��װ��һ����ɫ
    //�ѽڵ���Ϣװ��˳���
    for (int i = 0; i < n; i++)
    {
        node Temp;
        Temp.Color = 0;
        Temp.Loc = i;
        int DegreeI = 0;
        //����ڵ�Ķ�
        for (int j = 0; j < n; j++)
        {
            if(Map[i][j])
            {
                DegreeI++;
            }
        }
        Temp.Degree = DegreeI;
        Node.Insert(Temp);//�ڵ���Ϣ�������Ա�
    }

    ShellSort(Node, n);

    //�����ɫ
    for (int i = 0; i < n; i++)
    {
        bool Flag;//�ж��Ƿ���ɫ��ͻ
        int Loc = Node.Data[i].Loc;//��ýڵ��Ӧ�ڽӾ����±�
        //�ȱ���������ɫ���ж��Ƿ��ͻ,����ͻ����������ɫ
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
                    Flag = true;//��ɫ��ͻʱ����־λ�ı�
                }
                Num++;
            }
            //�������ͻ
            if(!Flag)
            {
                break;
            }
        }

        //���������ɫ��ͻ�������һ������ɫ�����
        if(Flag)
        {
            int NowMax = Color.GetLength();
            Color.Insert(NowMax + 1);//������ɫװ��˳���
            ColorToGo[Loc] = NowMax + 1;
        }
    }
}

int main()
{
    FillColor(13);//�����ɫ
    Print(ColorToGo,13);//��ӡͨ�з���
    system("pause");
    return 0;
}