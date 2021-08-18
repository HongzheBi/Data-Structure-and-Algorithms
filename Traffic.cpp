#include <iostream>
#include <string>
#include <cmath>
using namespace std;

const int MAXSIZE = 1000;//顺序表的最大长度

template <class T> 
class SeqList 
{
public:
	SeqList();//无参构造函数
	SeqList(const T a[], int n);//有参构造函数
	int GetLength();//获得顺序表长度
	void Insert(T x);//在顺序表的末尾插入x
	void PrintList();//打印顺序表
	T 	data[MAXSIZE];//存储顺序表的数组
private:
	int 	length;		         //顺序表的长度
};

//无参构造函数
template <class T>
SeqList<T>::SeqList() {
	
	length = 0;
}

//有参构造函数
template <class T>
SeqList<T>::SeqList(const T a[], int n) {
	
	//异常处理
	if (n > MAXSIZE) {
		throw "长度超过了范围";
	}
	else {
		for (int i = 0; i < n; i++) {
			data[i] = a[i];
		}
		length = n;
	}
}

//获得顺序表长度
template <class T>
int SeqList<T>::GetLength() {
	return length;
}

//在顺序表的末尾插入x
template<class T>
void SeqList<T>::Insert(T x) {
	int i = GetLength();//获得目前顺序表的长度
	data[i] = x;
	length++;
}

//打印顺序表
template<class T>
void SeqList<T>::PrintList() {
	for (int i = 0; i < length; i++) {
		cout << data[i]<<endl;
	}
}

//存储节点数据信息的结构体
struct Node {
	int loc;
	int degree;
	int color;
};

void fillColor(int n);//颜色填充
void Shellsort(SeqList<Node> node, int n);//利用希尔排序对节点数据按照度的数目降序排序
void Print(int list[], int n);//打印通行方案

//邻接矩阵，其中行列的含义如下：
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
int colorToGo[13] = { 0 };//记录每个节点颜色的数组
 
int main()
{
    fillColor(13);//颜色填充
	Print(colorToGo, 13);//打印通行方案
	system("pause");
	return 0;
}

//节点颜色填充
void fillColor(int n)
{
	SeqList<int> color;//建立容纳颜色的顺序表
	SeqList<Node> node;//建立容纳节点信息的顺序表
	color.Insert(1);//先装入第一种颜色
	//int numOfColor = 0;
	//把节点信息装入顺序表
	for (int i = 0; i < n; i++)
	{
		Node temp;
		temp.color = 0;
		temp.loc = i;
		int degreeOfinode = 0;
		//统计节点的度
		for (int j = 0; j < n; j++)
		{
			if (map[i][j] != 0)
			{
				degreeOfinode++;
			}
		}
		temp.degree = degreeOfinode;
		node.Insert(temp);//把一个完整的节点信息装入顺序表
	}
	//对节点按照度的数目降序排序
	Shellsort(node, n);

	//开始填充颜色
	for (int i = 0; i < n; i++)
	{
		bool flag;//判断颜色是否冲突的标志
        int loc = node.data[i].loc;//获得节点对应的邻接矩阵下标

		//先遍历已有的颜色，如果不冲突则使用已有的颜色
		for (int j = 0; j < color.GetLength(); j++)
		{
			colorToGo[loc] = color.data[j];
			int num = 0;
            flag = false;
			while (num < n)
			{
				int temp = map[loc][num];
				if ((temp == 1) && (colorToGo[num] == colorToGo[loc]))
					flag = true;//颜色冲突时，标志位变为true
				num++;
			}
			//如果颜色不冲突，则颜色填充完毕，跳出循环
			if (flag == false)
				break;
		}

		//如果已有的颜色均会产生冲突，则加入一种新的颜色，并给节点填充颜色
		if (flag == true) {
			int nowMax = color.GetLength();
			color.Insert(nowMax + 1);//把新颜色装入顺序表
			colorToGo[loc] = nowMax + 1;
		}
	}
}

//对节点按照度的数目降序排序
void Shellsort(SeqList<Node> node, int n)
{
	for (int d = n / 2; d >= 1; d = d / 2)//增量的大小递减
	{
		for (int i = d + 1; i <= n; i++)//进行一次希尔排序
		{
			if (node.data[i].degree > node.data[i - d].degree)
			{
				Node temp = node.data[i];//暂时存储要插入的节点信息
				int j = i - d;
                //依次按照间隔往前进行比较
				for (; temp.degree > node.data[j].degree&&j > 0; j -= d)
					node.data[j + d] = node.data[j];
				node.data[j + d] = temp;
			}
		}
	}
}

//打印通行方案
void Print(int list[], int n)
{
	//存储交通灯的种类
    string outcolor[5] = { "蓝灯","红灯","绿灯","黄灯","紫灯" };
	//存储各个可连通的方案
	string assessWay[13] = { "AB","AC","AD","BA","BC","BD","DA","DB","DC","EA","EB","EC","ED" };
	int numOfPrint = 0;//已经打印的通路的数目
	int numOfColor = 1;//正在寻找的灯的颜色
	//遍历灯的颜色
	while (numOfPrint < n && numOfColor <= 5)
	{
		cout << outcolor[numOfColor-1] << "亮时通行的道路为：";
		//遍历通路，找出与灯的颜色对应的通路
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
