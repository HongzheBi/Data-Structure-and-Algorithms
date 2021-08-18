#include <iostream>
#include <string>
#include <fstream>
#include <bitset>
#include <algorithm>

using namespace std;

//HuffmanNode
struct HTNode
{
    int Lchild;//左孩子
    int Rchild;//右孩子
    int Parent;//双亲
    int Weight;//权重
};

//HuffmanCodeTable
struct HuffmanCode
{
    string code;//编码
    char data;//字母
};

//private
HTNode* HuffmanTree;//哈夫曼树
HuffmanCode *HuffmanCodeTable;//哈夫曼编码表
string PrimaryStr;//原始字符串
string EnCodeResult;//编码结果
string DeCodeResult;//解码结果
int Types;//字符种类数
int TreeLevel;//树的层数
int DataNum[128];//对应字符出现次数
int Visiable[400][200];//哈夫曼树可视化数组
char Data[128];


void Init(string str);//初始化，统计字符串
void CreateHuffmanTree();//建立哈夫曼树
void CreateHuffmanCodeTable();//建立哈夫曼编码表
void EnCode();//编码
void DeCode();//解码
void PrintHuffmanTree();//打印哈夫曼树
void PrintHuffmanCodeTable();//打印哈夫曼编码表
void PrintEncodeResult();//打印编码结果
void PrintDecodeResult();//打印解码结果
void Analysis();//分析压缩效果
void Delete();//释放空间
void BinaryOutFile();//把二进制编码写入二进制文件
void BinaryInFile();//从二进制文件中读取二进制编码
bool InspectChinese(string str);//判断输入的字符串里是否有中文
void PreOrder(int Parent, int m, int n, int l);//哈夫曼树的层序遍历
void GetTreeLevel();//获得哈夫曼树的层数
void GetLevel(int Parent,int a);//获得哈夫曼树的层次递归过程
void CreateHuffmanCode(int n, string a);//创建每个字符对应的哈夫曼编码
void Min(int &x, int &y, int a);//找到指定范围内最小的两个值

//初始化：统计字符串
//O(n)
void Init(string str)
{
    if(InspectChinese(str))
    {
        throw "输入的字符串含有中文字符，请按2重新输入";
    }
    PrimaryStr = str;//存储原始字符串
    char temp_data[128];//建立储存临时字符的数组
    int temp_num[128] = {0};//建立统计临时字符数量的数组

    for (int i = 0; i < (int)str.length(); i++)//统计每个字符出现的次数
    {
        temp_data[int(str[i])] = str[i];
        temp_num[int(str[i])]++;
    }

    Types = 0;

    for (int i = 0; i < 128; i++)//将出现次数不为0的字符加到
    {
        if(temp_num[i])
        {
            Data[Types] = temp_data[i];
            DataNum[Types] = temp_num[i];
            Types++;
        }
    }

    //对是否编解码进行检查
    EnCodeResult = "";
    DeCodeResult = "";

    ofstream TempFile("HuffmanCode.dat",ios::out | ios::binary);
    TempFile << "" << '\n';
    TempFile.close();
}

//建立哈夫曼树
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
        Min(x, y, i);//获得从开始0到i无父母且最小的两个数的下标
        HuffmanTree[i].Weight = HuffmanTree[x].Weight + HuffmanTree[y].Weight;
        HuffmanTree[i].Lchild = x;
        HuffmanTree[i].Rchild = y;
        HuffmanTree[i].Parent = -1;
        HuffmanTree[x].Parent = HuffmanTree[y].Parent = i;
    }

    HuffmanCodeTable[Types - 1].code = "#"; //标记，用于对后续是否建立哈夫曼编码表进行检查
}

//建立哈夫曼编码表
//O(n)
void CreateHuffmanCodeTable()
{
    if(!Types)
    {
        throw "请先建立哈夫曼树";
    }
    CreateHuffmanCode(2 * Types - 2, "");//-2!!!
}

//编码(m为编码长度，n为树的规模)
//最好O(n)，最坏&平均O(n^2)
void Encode()
{
    if(HuffmanCodeTable[Types-1].code=="#")
        throw "请先建立哈夫曼编码表";
    EnCodeResult = "";
    int m = 0;
    
    while (PrimaryStr[m]!='\0')
    {
        int i = 0;
        while(PrimaryStr[m]!=HuffmanCodeTable[i].data)
            i++;
        EnCodeResult += HuffmanCodeTable[i].code;//将该字符编码结果加入编码字符串中
        m++;
    }
}

//解码(m为编码长度，n为树的规模)
//最好O(m),最坏&平均O(m*log2(n))
void Decode()
{
    if(EnCodeResult=="")
        throw "还未进行编码，请先进行解码";

    DeCodeResult = "";//防止多次操作累加，每次操作前置空
    int m = 0;

    while(EnCodeResult[m]!='\0')
    {
        int Parent = 2 * Types - 2;
        while(HuffmanTree[Parent].Lchild!=-1)//如果不是叶子结点，则继续寻找其左右孩子
        {
            if(EnCodeResult[m]=='0')//码0则为左孩子
                Parent = HuffmanTree[Parent].Lchild;
            else//码1则为右孩子
                Parent = HuffmanTree[Parent].Rchild;
            m++;
        }
        DeCodeResult += HuffmanCodeTable[Parent].data;
    }
}

//二进制编码写入二进制文件 
//O(n)
void BinaryOutFile()
{
    if(EnCodeResult=="")
        throw "还未进行编码，请先进行编码";
    //先置空文件,防止重复写入
    ofstream TempFile("HuffmanCode.dat", ios::out | ios::binary);
    TempFile<< "" << '\n';
    TempFile.close();

    //以写入、二进制、追加的方式打开文件
    ofstream OutFile("HuffmanCode.dat", ios::out | ios::binary | ios::app);

    //分别把每个01字符转化为二进制01，并追加到文件中
    for (int i = 0; i < (int)EnCodeResult.length(); i++)
    {
        bitset<1> TempBit(EnCodeResult[i]);
        OutFile << TempBit;
    }
    OutFile.close();
}

//从二进制文件中读取二进制编码
//O(n)
void BinaryInFile()
{
    //以读取、二进制的方式打开文件
    ifstream InFile("HuffmanCode.dat", ios::in | ios::binary);

    //以字符串形式读入二进制编码
    string TempStr;
    InFile >> TempStr;
    if(TempStr=="")
        throw "二进制文件为空，请检查操作是否正确";
    EnCodeResult = TempStr;
    InFile.close();
}

//打印哈夫曼编码表
//O(1)
void PrintHuffmanCodeTable()
{
    cout<<"HuffmanCodeTable:"<<'\n';
    for (int i = 0; i < Types; i++)
    {
        cout << HuffmanCodeTable[i].data << " : " << HuffmanCodeTable[i].code << '\n';
    }
}

//打印编码结果
//O(1)
void PrintEncodeResult()
{
    cout << "Encode result:" << '\n'
         << EnCodeResult << '\n';
}

//打印解码结果
//O(1)
void PrintDecodeResult()
{
    cout << "Decode result:" << '\n'
         << DeCodeResult << '\n';
}

//分析压缩效果
//O(1)
void Analysis()
{
    int PreLen = 8 * PrimaryStr.length();//获得原始字符串所占的位
    int PressLen = EnCodeResult.length();//获得编码后的字符串所占的位
    double Ratio = double(PressLen) / PreLen * 100;//获得压缩比率
    cout << "原始字符串：" << PreLen << "bit" << '\n'
         << "压缩的字符串：" << PressLen << "bit" << '\n'
         << "压缩比：" << Ratio << "%" << '\n';
}

//创建每个字符对应的哈夫曼编码
//O(n)
void CreateHuffmanCode(int n,string a)
{
    if(HuffmanTree[n].Lchild==-1)
    {
        HuffmanCodeTable[n].code = a;
        return;
    }

    CreateHuffmanCode(HuffmanTree[n].Lchild, a + '0');//递归左孩子
    CreateHuffmanCode(HuffmanTree[n].Rchild, a + '1');//递归右孩子  
}

//获得哈夫曼树的层数递归过程
//O(n)
void GetLevel(int Parent,int a)
{
    if(a>TreeLevel)//若当前层数更深,更新原始层数
    {
        TreeLevel = a;
    }
    if(HuffmanTree[Parent].Lchild==-1)//如果左孩子为1，则已到叶子
        return;
    GetLevel(HuffmanTree[Parent].Lchild, a + 1);//递归左孩子
    GetLevel(HuffmanTree[Parent].Rchild, a + 1);//递归右孩子
}

//获得哈夫曼树的层数
//O(n)
void GetTreeLevel()
{
    TreeLevel = 0;
    GetLevel(2 * Types - 2, 1);//注意根节点的数组下标为2 * Types - 2
}

//对哈夫曼树层序遍历
//O(n)
void PreOrder(int Parent,int m,int n,int l)
{
    Visiable[m][n] = HuffmanTree[Parent].Weight;//将当前数据存入可视化数组

    if(HuffmanTree[Parent].Lchild==-1)
    {
        Visiable[m][n + 1] = -2;//标志位，表示它前面对应的叶子结点
        return;
    }

    PreOrder(HuffmanTree[Parent].Lchild, m + l / 2, n + 4, l / 2);//所有孩子的间距除2以保证不会发生重叠
    PreOrder(HuffmanTree[Parent].Rchild, m - l / 2, n + 4, l / 2);
}

//判断输入的字符串是否有中文
//O(n)
bool InspectChinese(string str)
{
    char ch;
    int m = 0;
    while(1)
    {
        ch = str[m];
        if(!ch)            //如果到字符串尾则无中文字符
            break;
        if(ch&0x00)        //如果字符高位为1且下一字符高位也为1则有中文字符
            if(str[m]&0x00)
                return 1;
        m++;
    }
    return 0;
}

//指定范围内最小的两个值
//O(1)
void Min(int& x,int& y,int a)
{
    int i = 0;
    for (; i < a; i++)//找到第一个没有父母的结点
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

    for (; i < a; i++)//之后对没有父母的结点进行比较
    {
        if((HuffmanTree[i].Parent==-1)&&(HuffmanTree[i].Weight<HuffmanTree[y].Weight))
        {
            if((HuffmanTree[i].Parent==-1)&&(HuffmanTree[i].Weight<HuffmanTree[x].Weight))
            {
                //小于x,y中最小的
                y = x;
                x = i;
            }
            else//大于等于x,小于y
                y = i;
        }
    }
}

//打印哈夫曼树
//O(n)
void PrintHuffmanTree()
{
    GetTreeLevel();//获得哈夫曼树的层数
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
    cout<<"哈夫曼树逆时针旋转90°的结果，'*'代表叶子节点"<<endl;

    int RowBeg = 0;
    int RowEnd = 0;
    for (int i = 0; i < 400; i++)//获得树的开始行数
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

    for (int i = 399; i >= 0; i--)//获得树的结束行数
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
    for (int i = 199; i >= 0; i++)//获得树的结束列数
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

    for (int i = RowBeg; i < RowEnd + 1; i++)//打印树
    {
        for (int j = 0; j < ColEnd; j++)
        {
            if(Visiable[i][j]&&Visiable[i][j]!=-2)//存在树且不是标志位
                cout << Visiable[i][j];
            else if(Visiable[i][j]==-2)//标志位输出"*"
                cout << "*";
            else
                cout << " ";
        }
        cout << '\n';
    }
}