#include <iostream>
#include <string>
#include <fstream>
#include <bitset>
#include <algorithm>

using namespace std;

//HuffmanNode
struct HTNode
{
    int Lchild;//����
    int Rchild;//�Һ���
    int Parent;//˫��
    int Weight;//Ȩ��
};

//HuffmanCodeTable
struct HuffmanCode
{
    string code;//����
    char data;//��ĸ
};

//private
HTNode* HuffmanTree;//��������
HuffmanCode *HuffmanCodeTable;//�����������
string PrimaryStr;//ԭʼ�ַ���
string EnCodeResult;//������
string DeCodeResult;//������
int Types;//�ַ�������
int TreeLevel;//���Ĳ���
int DataNum[128];//��Ӧ�ַ����ִ���
int Visiable[400][200];//�����������ӻ�����
char Data[128];


void Init(string str);//��ʼ����ͳ���ַ���
void CreateHuffmanTree();//������������
void CreateHuffmanCodeTable();//���������������
void EnCode();//����
void DeCode();//����
void PrintHuffmanTree();//��ӡ��������
void PrintHuffmanCodeTable();//��ӡ�����������
void PrintEncodeResult();//��ӡ������
void PrintDecodeResult();//��ӡ������
void Analysis();//����ѹ��Ч��
void Delete();//�ͷſռ�
void BinaryOutFile();//�Ѷ����Ʊ���д��������ļ�
void BinaryInFile();//�Ӷ������ļ��ж�ȡ�����Ʊ���
bool InspectChinese(string str);//�ж�������ַ������Ƿ�������
void PreOrder(int Parent, int m, int n, int l);//���������Ĳ������
void GetTreeLevel();//��ù��������Ĳ���
void GetLevel(int Parent,int a);//��ù��������Ĳ�εݹ����
void CreateHuffmanCode(int n, string a);//����ÿ���ַ���Ӧ�Ĺ���������
void Min(int &x, int &y, int a);//�ҵ�ָ����Χ����С������ֵ

//��ʼ����ͳ���ַ���
//O(n)
void Init(string str)
{
    if(InspectChinese(str))
    {
        throw "������ַ������������ַ����밴2��������";
    }
    PrimaryStr = str;//�洢ԭʼ�ַ���
    char temp_data[128];//����������ʱ�ַ�������
    int temp_num[128] = {0};//����ͳ����ʱ�ַ�����������

    for (int i = 0; i < (int)str.length(); i++)//ͳ��ÿ���ַ����ֵĴ���
    {
        temp_data[int(str[i])] = str[i];
        temp_num[int(str[i])]++;
    }

    Types = 0;

    for (int i = 0; i < 128; i++)//�����ִ�����Ϊ0���ַ��ӵ�
    {
        if(temp_num[i])
        {
            Data[Types] = temp_data[i];
            DataNum[Types] = temp_num[i];
            Types++;
        }
    }

    //���Ƿ�������м��
    EnCodeResult = "";
    DeCodeResult = "";

    ofstream TempFile("HuffmanCode.dat",ios::out | ios::binary);
    TempFile << "" << '\n';
    TempFile.close();
}

//������������
//O(n)
void CreateHuffmanTree()
{
    HuffmanTree = new HTNode[2 * Types - 1];
    HuffmanCodeTable = new HuffmanCode[Types];

    for (int i = 0; i < Types; i++)
    {
        HuffmanTree[i].Weight = DataNum[i];
        HuffmanTree[i].Parent = -1;
        HuffmanTree[i].Lchild = -1;
        HuffmanTree[i].Rchild = -1;
        HuffmanCodeTable[i].data = Data[i];
    }

    int x, y;
    for (int i = Types; i < 2 * Types - 1; i++)
    {
        Min(x, y, i);//��ôӿ�ʼ0��i�޸�ĸ����С�����������±�
        HuffmanTree[i].Weight = HuffmanTree[x].Weight + HuffmanTree[y].Weight;
        HuffmanTree[i].Lchild = x;
        HuffmanTree[i].Rchild = y;
        HuffmanTree[i].Parent = -1;
        HuffmanTree[x].Parent = HuffmanTree[y].Parent = i;
    }

    HuffmanCodeTable[Types - 1].code = "#"; //��ǣ����ڶԺ����Ƿ����������������м��
}

//���������������
//O(n)
void CreateHuffmanCodeTable()
{
    if(!Types)
    {
        throw "���Ƚ�����������";
    }
    CreateHuffmanCode(2 * Types - 2, "");//-2!!!
}

//����(mΪ���볤�ȣ�nΪ���Ĺ�ģ)
//���O(n)���&ƽ��O(n^2)
void Encode()
{
    if(HuffmanCodeTable[Types-1].code=="#")
        throw "���Ƚ��������������";
    EnCodeResult = "";
    int m = 0;
    
    while (PrimaryStr[m]!='\0')
    {
        int i = 0;
        while(PrimaryStr[m]!=HuffmanCodeTable[i].data)
            i++;
        EnCodeResult += HuffmanCodeTable[i].code;//�����ַ���������������ַ�����
        m++;
    }
}

//����(mΪ���볤�ȣ�nΪ���Ĺ�ģ)
//���O(m),�&ƽ��O(m*log2(n))
void Decode()
{
    if(EnCodeResult=="")
        throw "��δ���б��룬���Ƚ��н���";

    DeCodeResult = "";//��ֹ��β����ۼӣ�ÿ�β���ǰ�ÿ�
    int m = 0;

    while(EnCodeResult[m]!='\0')
    {
        int Parent = 2 * Types - 2;
        while(HuffmanTree[Parent].Lchild!=-1)//�������Ҷ�ӽ�㣬�����Ѱ�������Һ���
        {
            if(EnCodeResult[m]=='0')//��0��Ϊ����
                Parent = HuffmanTree[Parent].Lchild;
            else//��1��Ϊ�Һ���
                Parent = HuffmanTree[Parent].Rchild;
            m++;
        }
        DeCodeResult += HuffmanCodeTable[Parent].data;
    }
}

//�����Ʊ���д��������ļ� 
//O(n)
void BinaryOutFile()
{
    if(EnCodeResult=="")
        throw "��δ���б��룬���Ƚ��б���";
    //���ÿ��ļ�,��ֹ�ظ�д��
    ofstream TempFile("HuffmanCode.dat", ios::out | ios::binary);
    TempFile<< "" << '\n';
    TempFile.close();

    //��д�롢�����ơ�׷�ӵķ�ʽ���ļ�
    ofstream OutFile("HuffmanCode.dat", ios::out | ios::binary | ios::app);

    //�ֱ��ÿ��01�ַ�ת��Ϊ������01����׷�ӵ��ļ���
    for (int i = 0; i < (int)EnCodeResult.length(); i++)
    {
        bitset<1> TempBit(EnCodeResult[i]);
        OutFile << TempBit;
    }
    OutFile.close();
}

//�Ӷ������ļ��ж�ȡ�����Ʊ���
//O(n)
void BinaryInFile()
{
    //�Զ�ȡ�������Ƶķ�ʽ���ļ�
    ifstream InFile("HuffmanCode.dat", ios::in | ios::binary);

    //���ַ�����ʽ��������Ʊ���
    string TempStr;
    InFile >> TempStr;
    if(TempStr=="")
        throw "�������ļ�Ϊ�գ���������Ƿ���ȷ";
    EnCodeResult = TempStr;
    InFile.close();
}

//��ӡ�����������
//O(1)
void PrintHuffmanCodeTable()
{
    cout<<"HuffmanCodeTable:"<<'\n';
    for (int i = 0; i < Types; i++)
    {
        cout << HuffmanCodeTable[i].data << " : " << HuffmanCodeTable[i].code << '\n';
    }
}

//��ӡ������
//O(1)
void PrintEncodeResult()
{
    cout << "Encode result:" << '\n'
         << EnCodeResult << '\n';
}

//��ӡ������
//O(1)
void PrintDecodeResult()
{
    cout << "Decode result:" << '\n'
         << DeCodeResult << '\n';
}

//����ѹ��Ч��
//O(1)
void Analysis()
{
    int PreLen = 8 * PrimaryStr.length();//���ԭʼ�ַ�����ռ��λ
    int PressLen = EnCodeResult.length();//��ñ������ַ�����ռ��λ
    double Ratio = double(PressLen) / PreLen * 100;//���ѹ������
    cout << "ԭʼ�ַ�����" << PreLen << "bit" << '\n'
         << "ѹ�����ַ�����" << PressLen << "bit" << '\n'
         << "ѹ���ȣ�" << Ratio << "%" << '\n';
}

//����ÿ���ַ���Ӧ�Ĺ���������
//O(n)
void CreateHuffmanCode(int n,string a)
{
    if(HuffmanTree[n].Lchild==-1)
    {
        HuffmanCodeTable[n].code = a;
        return;
    }

    CreateHuffmanCode(HuffmanTree[n].Lchild, a + '0');//�ݹ�����
    CreateHuffmanCode(HuffmanTree[n].Rchild, a + '1');//�ݹ��Һ���  
}

//��ù��������Ĳ����ݹ����
//O(n)
void GetLevel(int Parent,int a)
{
    if(a>TreeLevel)//����ǰ��������,����ԭʼ����
    {
        TreeLevel = a;
    }
    if(HuffmanTree[Parent].Lchild==-1)//�������Ϊ1�����ѵ�Ҷ��
        return;
    GetLevel(HuffmanTree[Parent].Lchild, a + 1);//�ݹ�����
    GetLevel(HuffmanTree[Parent].Rchild, a + 1);//�ݹ��Һ���
}

//��ù��������Ĳ���
//O(n)
void GetTreeLevel()
{
    TreeLevel = 0;
    GetLevel(2 * Types - 2, 1);//ע����ڵ�������±�Ϊ2 * Types - 2
}

//�Թ��������������
//O(n)
void PreOrder(int Parent,int m,int n,int l)
{
    Visiable[m][n] = HuffmanTree[Parent].Weight;//����ǰ���ݴ�����ӻ�����

    if(HuffmanTree[Parent].Lchild==-1)
    {
        Visiable[m][n + 1] = -2;//��־λ����ʾ��ǰ���Ӧ��Ҷ�ӽ��
        return;
    }

    PreOrder(HuffmanTree[Parent].Lchild, m + l / 2, n + 4, l / 2);//���к��ӵļ���2�Ա�֤���ᷢ���ص�
    PreOrder(HuffmanTree[Parent].Rchild, m - l / 2, n + 4, l / 2);
}

//�ж�������ַ����Ƿ�������
//O(n)
bool InspectChinese(string str)
{
    char ch;
    int m = 0;
    while(1)
    {
        ch = str[m];
        if(!ch)            //������ַ���β���������ַ�
            break;
        if(ch&0x00)        //����ַ���λΪ1����һ�ַ���λҲΪ1���������ַ�
            if(str[m]&0x00)
                return 1;
        m++;
    }
    return 0;
}

//ָ����Χ����С������ֵ
//O(1)
void Min(int& x,int& y,int a)
{
    int i = 0;
    for (; i < a; i++)//�ҵ���һ��û�и�ĸ�Ľ��
    {
        if(HuffmanTree[i].Parent==-1)
        {
            x = i;
            i++;
            break;
        }
    }

    for (; i < a; i++)
    {
        if(HuffmanTree[i].Parent==-1)
        {
            y = i;
            i++;
            break;
        }
    }

    if(HuffmanTree[x].Weight>HuffmanTree[y].Weight)
    {
        swap(x, y);
    }

    for (; i < a; i++)//֮���û�и�ĸ�Ľ����бȽ�
    {
        if((HuffmanTree[i].Parent==-1)&&(HuffmanTree[i].Weight<HuffmanTree[y].Weight))
        {
            if((HuffmanTree[i].Parent==-1)&&(HuffmanTree[i].Weight<HuffmanTree[x].Weight))
            {
                //С��x,y����С��
                y = x;
                x = i;
            }
            else//���ڵ���x,С��y
                y = i;
        }
    }
}

//��ӡ��������
//O(n)
void PrintHuffmanTree()
{
    GetTreeLevel();//��ù��������Ĳ���
    int bet = 1;
    for (int i = 0; i < TreeLevel - 1; i++)
    {
        bet *= 2;
    }

    for (int i = 0; i < 400; i++)
    {
        for (int j = 0; j < 200; j++)
        {
            Visiable[i][j] = 0;
        }
    }

    PreOrder(2*Types-2,100,0,bet);//Parent,m,n,l
    cout<<"����������ʱ����ת90��Ľ����'*'����Ҷ�ӽڵ�"<<endl;

    int RowBeg = 0;
    int RowEnd = 0;
    for (int i = 0; i < 400; i++)//������Ŀ�ʼ����
    {
        int flag = 0;
        for (int j = 0; j < 200; j++)
        {
            if(Visiable[i][j])
            {
                flag = 1;
                break;
            }
        }
        if(flag==1)
        {
            RowBeg = i;
            break;
        }
    }

    for (int i = 399; i >= 0; i--)//������Ľ�������
    {
        int flag = 0;
        for (int j = 0; j < 200; j++)
        {
            if(Visiable[i][j])
            {
                flag = 1;
                break;
            }
        }
        if (flag == 1)
        {
            RowEnd = i;
            break;
        }
    }

    int ColEnd = 0;
    for (int i = 199; i >= 0; i++)//������Ľ�������
    {
        int flag = 0;
        for (int j = 0; j < 400;j++)
        {
            if(Visiable[j][i])
            {
                flag = 1;
                break;
            }
        }
        if(flag==1)
        {
            ColEnd = i;
            break;
        }
    }

    for (int i = RowBeg; i < RowEnd + 1; i++)//��ӡ��
    {
        for (int j = 0; j < ColEnd; j++)
        {
            if(Visiable[i][j]&&Visiable[i][j]!=-2)//�������Ҳ��Ǳ�־λ
                cout << Visiable[i][j];
            else if(Visiable[i][j]==-2)//��־λ���"*"
                cout << "*";
            else
                cout << " ";
        }
        cout << '\n';
    }
}