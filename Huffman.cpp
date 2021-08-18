#include <iostream>
#include <string>
#include <windows.h>
#include <cmath>
#include "Huffman.h"

using namespace std;

//��������
//O��1��
void Guide()
{
	cout << "\t\t\t\t\t\tʹ����֪" << '\n';
	cout << "ע�⣡�����в�����ʼ֮ǰ�������Ƚ��������������Լ���ñ����(��Ӧ1��3����)"
		 <<"����������ڱ���֮����У��Ӷ������ļ����������д��������ļ�֮����У�" << 
		"���������󣬽��������ʾ��"<<'\n';
	cout << "�Ƽ����²���˳��:" << '\n';
	cout << "1->3->4->5->6->7->8->9" << '\n';
	cout << "����˵����" << '\n';
	cout << "��1(Init)������(����Ĭ��)���ַ���������������" << '\n';
	cout << "��3(CreateHuffmanTable)�Թ����������б��룬����ÿ���ַ��ı������" << '\n';
	cout << "��4(Encode)���ñ������ַ������б��룬�����������" << '\n';
	cout << "��5(Decode)�Ա������ַ����������룬�����������" << '\n';
	cout << "��6(PrintHuffmanTree)��ֱ�۵ķ�ʽ��ӡ����������Ϊ����ֱ�۵ؿ���������������һ���϶̵��ַ������г�ʼ����" << '\n';
	cout << "��7(Analysis)���������������ѹ��Ч��" << '\n';
	cout << "��8(BinaryOutFile)��������ת��Ϊ��Ӧ�Ķ����Ʋ�д��������ļ���" << '\n';
	cout << "��9(BinaryInFile)�Ӷ������ļ��ж�������ƴ�������ת��Ϊ��Ӧ���ַ����������������" << '\n';
	cout << "��0(Help)�������ģʽ" << '\n';
}

//�˵�
//O(1)
void Menu()
{
    cout << "1(Init)  3(CreateHuffmanTable)  4(Encode)  5(Decode)  6(PrintHuffmanTree)  7(Analysis)"
         << "  8(BinaryOutFile)  9(BinaryInFile)  0(Help)" << '\n';
}
//2�����в���

//��1��Ч��
//O(n)
void Key1()
{
    system("cls");//�����Ļ
    Menu();//���ô�ӡ�˵����ĺ���
	cout << "���Ƿ�Ҫʹ��Ĭ�ϵĲ���������(Y/N)��";
    char ch;
    cin >> ch;
    string str;
    //ʹ��Ĭ�����ݵ����
    if(ch=='Y')
    {
        str = "";
        cout << "������" << '\n';
    }
    //ʹ���Լ��������ݵ����
    else if(ch=='N')
    {
        cout << "������һ��Ӣ���ַ���(Enter����������):";
        cin >> str;
        cout << "������" << '\n';
    }
    else
        throw "����������������룡";
    Init(str);
    CreateHuffmanTree();
}

//��3��Ч��
//O(n)
void Key3()
{
    system("cls");
    Menu();
    CreateHuffmanCodeTable();
    PrintHuffmanCodeTable();
}

//��4��Ч��
//���O(n),ƽ�����O(n^2)
void Key4()
{
    system("cls");
    Menu();
    Encode();
    PrintEncodeResult();
}

//��5��Ч��
//mΪ���볤�ȣ�nΪ���Ĺ�ģ
//���O(m)��ƽ�����O(m*log2(n))
void Key5()
{
    system("cls");
    Menu();
    Decode();
    PrintDecodeResult();
}

//��6��Ч��
//O(n)
void Key6()
{
    system("cls");
    Menu();
    PrintHuffmanTree();
}

//��7��Ч��
//O(1)
void Key7()
{
    system("cls");
    Menu();
    Analysis();
}

//��8��Ч��
//O(n)
void Key8()
{
    system("cls");
    Menu();
    BinaryOutFile();
	cout << "�����Ʊ����ѳɹ�����������ļ���" << '\n';
}
//��9��Ч��
//O(n)
void Key9()
{
    system("cls");
    Menu();
    BinaryInFile();
    Decode();
    PrintDecodeResult();
}

//��0��Ч��
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
            if (ch=='1')//����1
                Key1();
            else if (ch=='3')//����3
                Key3();
			else if (ch=='4')//����4
                Key4();
			else if (ch=='5')//����5
                Key5();
			else if (ch=='6')//����6
                Key6();
			else if (ch=='7')//����7
                Key7();
            else if (ch=='8')//����8
                Key8();
			else if (ch=='9')//����9
                Key9();
			else if (ch=='0')//����0
                Key0();

        }
        catch(string* Error)//�쳣����
        {
            cout << "Error" << '\n';
        }
    }
    Delete();
    system("pause");
    return 0;
}