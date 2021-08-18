#include <iostream>
#include <string>
#include <windows.h>
#include <cmath>
#include "Huffman.h"

using namespace std;

//引导界面
//O（1）
void Guide()
{
	cout << "\t\t\t\t\t\t使用须知" << '\n';
	cout << "注意！在所有操作开始之前，必须先建立哈夫曼树，以及获得编码表(对应1、3操作)"
		 <<"！译码必须在编码之后进行！从二进制文件读入必须在写入二进制文件之后进行！" << 
		"若操作有误，将会给出提示。"<<'\n';
	cout << "推荐以下操作顺序:" << '\n';
	cout << "1->3->4->5->6->7->8->9" << '\n';
	cout << "操作说明：" << '\n';
	cout << "按1(Init)对输入(或者默认)的字符串建立哈夫曼树" << '\n';
	cout << "按3(CreateHuffmanTable)对哈夫曼树进行编码，并将每个字符的编码输出" << '\n';
	cout << "按4(Encode)利用编码表对字符串进行编码，并输出编码结果" << '\n';
	cout << "按5(Decode)对编码后的字符串进行译码，并输出译码结果" << '\n';
	cout << "按6(PrintHuffmanTree)以直观的方式打印哈夫曼树【为了能直观地看到树，可以输入一个较短的字符串进行初始化】" << '\n';
	cout << "按7(Analysis)分析哈夫曼编码的压缩效果" << '\n';
	cout << "按8(BinaryOutFile)将编码结果转化为对应的二进制并写入二进制文件。" << '\n';
	cout << "按9(BinaryInFile)从二进制文件中读入二进制串，将其转化为对应的字符串，并输出译码结果" << '\n';
	cout << "按0(Help)进入帮助模式" << '\n';
}

//菜单
//O(1)
void Menu()
{
    cout << "1(Init)  3(CreateHuffmanTable)  4(Encode)  5(Decode)  6(PrintHuffmanTree)  7(Analysis)"
         << "  8(BinaryOutFile)  9(BinaryInFile)  0(Help)" << '\n';
}
//2键已有操作

//按1的效果
//O(n)
void Key1()
{
    system("cls");//清空屏幕
    Menu();//调用打印菜单栏的函数
	cout << "您是否要使用默认的测试用例？(Y/N)：";
    char ch;
    cin >> ch;
    string str;
    //使用默认数据的情况
    if(ch=='Y')
    {
        str = "";
        cout << "已输入" << '\n';
    }
    //使用自己输入数据的情况
    else if(ch=='N')
    {
        cout << "请输入一个英文字符串(Enter键结束输入):";
        cin >> str;
        cout << "已输入" << '\n';
    }
    else
        throw "输入错误，请重新输入！";
    Init(str);
    CreateHuffmanTree();
}

//按3的效果
//O(n)
void Key3()
{
    system("cls");
    Menu();
    CreateHuffmanCodeTable();
    PrintHuffmanCodeTable();
}

//按4的效果
//最好O(n),平均和最坏O(n^2)
void Key4()
{
    system("cls");
    Menu();
    Encode();
    PrintEncodeResult();
}

//按5的效果
//m为编码长度，n为树的规模
//最好O(m)，平均和最坏O(m*log2(n))
void Key5()
{
    system("cls");
    Menu();
    Decode();
    PrintDecodeResult();
}

//按6的效果
//O(n)
void Key6()
{
    system("cls");
    Menu();
    PrintHuffmanTree();
}

//按7的效果
//O(1)
void Key7()
{
    system("cls");
    Menu();
    Analysis();
}

//按8的效果
//O(n)
void Key8()
{
    system("cls");
    Menu();
    BinaryOutFile();
	cout << "二进制编码已成功存入二进制文件！" << '\n';
}
//按9的效果
//O(n)
void Key9()
{
    system("cls");
    Menu();
    BinaryInFile();
    Decode();
    PrintDecodeResult();
}

//按0的效果
//O(1)
void Key0()
{
    system("cls");
    Guide();
}

int main()
{
    //ios::sync_with_stdio(0);
    Guide();
    while(1)
    {
        try
        {
            char ch;
            cin >> ch;
            if (ch=='1')//按下1
                Key1();
            else if (ch=='3')//按下3
                Key3();
			else if (ch=='4')//按下4
                Key4();
			else if (ch=='5')//按下5
                Key5();
			else if (ch=='6')//按下6
                Key6();
			else if (ch=='7')//按下7
                Key7();
            else if (ch=='8')//按下8
                Key8();
			else if (ch=='9')//按下9
                Key9();
			else if (ch=='0')//按下0
                Key0();

        }
        catch(string* Error)//异常处理
        {
            cout << "Error" << '\n';
        }
    }
    Delete();
    system("pause");
    return 0;
}