#include <iostream>
#include <string>
#include <cmath>
using namespace std;

const int MAXSIZE = 1000;//˳������󳤶�

template <class T> 
class SeqList 
{
public:
	SeqList();//�޲ι��캯��
	SeqList(const T a[], int n);//�вι��캯��
	int GetLength();//���˳�����
	void Insert(T x);//��˳����ĩβ����x
	void PrintList();//��ӡ˳���
	T 	data[MAXSIZE];//�洢˳��������
private:
	int 	length;		         //˳���ĳ���
};

//�޲ι��캯��
template <class T>
SeqList<T>::SeqList() {
	
	length = 0;
}

//�вι��캯��
template <class T>
SeqList<T>::SeqList(const T a[], int n) {
	
	//�쳣����
	if (n > MAXSIZE) {
		throw "���ȳ����˷�Χ";
	}
	else {
		for (int i = 0; i < n; i++) {
			data[i] = a[i];
		}
		length = n;
	}
}

//���˳�����
template <class T>
int SeqList<T>::GetLength() {
	return length;
}

//��˳����ĩβ����x
template<class T>
void SeqList<T>::Insert(T x) {
	int i = GetLength();//���Ŀǰ˳���ĳ���
	data[i] = x;
	length++;
}

//��ӡ˳���
template<class T>
void SeqList<T>::PrintList() {
	for (int i = 0; i < length; i++) {
		cout << data[i]<<endl;
	}
}

//�洢�ڵ�������Ϣ�Ľṹ��
struct Node {
	int loc;
	int degree;
	int color;
};

void fillColor(int n);//��ɫ���
void Shellsort(SeqList<Node> node, int n);//����ϣ������Խڵ����ݰ��նȵ���Ŀ��������
void Print(int list[], int n);//��ӡͨ�з���

//�ڽӾ����������еĺ������£�
//AB AC AD BA BC BD DA DB DC EA EB EC ED
int map[13][13] =
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
int colorToGo[13] = { 0 };//��¼ÿ���ڵ���ɫ������
 
int main()
{
    fillColor(13);//��ɫ���
	Print(colorToGo, 13);//��ӡͨ�з���
	system("pause");
	return 0;
}

//�ڵ���ɫ���
void fillColor(int n)
{
	SeqList<int> color;//����������ɫ��˳���
	SeqList<Node> node;//�������ɽڵ���Ϣ��˳���
	color.Insert(1);//��װ���һ����ɫ
	//int numOfColor = 0;
	//�ѽڵ���Ϣװ��˳���
	for (int i = 0; i < n; i++)
	{
		Node temp;
		temp.color = 0;
		temp.loc = i;
		int degreeOfinode = 0;
		//ͳ�ƽڵ�Ķ�
		for (int j = 0; j < n; j++)
		{
			if (map[i][j] != 0)
			{
				degreeOfinode++;
			}
		}
		temp.degree = degreeOfinode;
		node.Insert(temp);//��һ�������Ľڵ���Ϣװ��˳���
	}
	//�Խڵ㰴�նȵ���Ŀ��������
	Shellsort(node, n);

	//��ʼ�����ɫ
	for (int i = 0; i < n; i++)
	{
		bool flag;//�ж���ɫ�Ƿ��ͻ�ı�־
        int loc = node.data[i].loc;//��ýڵ��Ӧ���ڽӾ����±�

		//�ȱ������е���ɫ���������ͻ��ʹ�����е���ɫ
		for (int j = 0; j < color.GetLength(); j++)
		{
			colorToGo[loc] = color.data[j];
			int num = 0;
            flag = false;
			while (num < n)
			{
				int temp = map[loc][num];
				if ((temp == 1) && (colorToGo[num] == colorToGo[loc]))
					flag = true;//��ɫ��ͻʱ����־λ��Ϊtrue
				num++;
			}
			//�����ɫ����ͻ������ɫ�����ϣ�����ѭ��
			if (flag == false)
				break;
		}

		//������е���ɫ���������ͻ�������һ���µ���ɫ�������ڵ������ɫ
		if (flag == true) {
			int nowMax = color.GetLength();
			color.Insert(nowMax + 1);//������ɫװ��˳���
			colorToGo[loc] = nowMax + 1;
		}
	}
}

//�Խڵ㰴�նȵ���Ŀ��������
void Shellsort(SeqList<Node> node, int n)
{
	for (int d = n / 2; d >= 1; d = d / 2)//�����Ĵ�С�ݼ�
	{
		for (int i = d + 1; i <= n; i++)//����һ��ϣ������
		{
			if (node.data[i].degree > node.data[i - d].degree)
			{
				Node temp = node.data[i];//��ʱ�洢Ҫ����Ľڵ���Ϣ
				int j = i - d;
                //���ΰ��ռ����ǰ���бȽ�
				for (; temp.degree > node.data[j].degree&&j > 0; j -= d)
					node.data[j + d] = node.data[j];
				node.data[j + d] = temp;
			}
		}
	}
}

//��ӡͨ�з���
void Print(int list[], int n)
{
	//�洢��ͨ�Ƶ�����
    string outcolor[5] = { "����","���","�̵�","�Ƶ�","�ϵ�" };
	//�洢��������ͨ�ķ���
	string assessWay[13] = { "AB","AC","AD","BA","BC","BD","DA","DB","DC","EA","EB","EC","ED" };
	int numOfPrint = 0;//�Ѿ���ӡ��ͨ·����Ŀ
	int numOfColor = 1;//����Ѱ�ҵĵƵ���ɫ
	//�����Ƶ���ɫ
	while (numOfPrint < n && numOfColor <= 5)
	{
		cout << outcolor[numOfColor-1] << "��ʱͨ�еĵ�·Ϊ��";
		//����ͨ·���ҳ���Ƶ���ɫ��Ӧ��ͨ·
		for (int i = 0; i < n; i++)
		{
			if (list[i] == numOfColor)
			{
				cout << assessWay[i] << " ";
				numOfPrint++;
			}
		}
		cout << endl;
		numOfColor++;
	}
}
