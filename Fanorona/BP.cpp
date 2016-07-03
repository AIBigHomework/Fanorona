#include <string.h>  
#include <stdio.h>  
#include <math.h>  
#include <assert.h>  
#include "BP.h"  

BP::BP(int innum, int outnum)
{
	in_num = innum;
	ou_num = outnum;
	hd_num = (int)sqrt((in_num + ou_num) * 1.0) + 5;
	if (hd_num > NUM) hd_num = NUM; 
	InitNetWork();
}

//获取训练所有样本数据  
void BP::GetData(const Data _data)
{
	data = _data;
}

void BP::TrainFanorona(std::stack<Data> s)
{
	std::vector<Type> inValue;
	std::vector<Type> outValue = s.top().y;
	while (!s.empty())
	{
		Data d = s.top();
		s.pop();
		d.y = outValue;
		inValue = d.x;
		GetData(d);
		Train();
		outValue = ForeCast(inValue);
	}
	
}

//开始进行训练  
void BP::Train()
{
	printf("Begin to train BP NetWork!\n");


	for (int iter = 0; iter <= ITERS; iter++)
	{

			//第一层输入节点赋值  
			for (int i = 0; i < in_num; i++)
				x[0][i] = data.x[i];

			while (1)
			{
				ForwardTransfer();
				if (GetError() < ERROR)    //如果误差比较小，则针对单个样本跳出循环  
					break;
				ReverseTransfer();
			}

		Type accu = GetAccu();
		if (accu < ACCU) break;
	}
}

//根据训练好的网络来预测输出值  
Vector<Type> BP::ForeCast(const Vector<Type> data)
{
	int n = data.size();
	assert(n == in_num);
	for (int i = 0; i < in_num; i++)
		x[0][i] = data[i];

	ForwardTransfer();
	Vector<Type> v;
	for (int i = 0; i < ou_num; i++)
		v.push_back(x[2][i]);
	return v;
}

//获取网络节点数  
void BP::GetNums()
{
	in_num = data.x.size();                         //获取输入层节点数  
	ou_num = data.y.size();                         //获取输出层节点数  
	hd_num = (int)sqrt((in_num + ou_num) * 1.0) + 5;   //获取隐含层节点数  
	if (hd_num > NUM) hd_num = NUM;                     //隐含层数目不能超过最大设置  
}

//初始化网络  
void BP::InitNetWork()
{
	memset(w, 0, sizeof(w));      //初始化权值和阀值为0，也可以初始化随机值  
	memset(b, 0, sizeof(b));
}

//工作信号正向传递子过程  
void BP::ForwardTransfer()
{
	//计算隐含层各个节点的输出值  
	for (int j = 0; j < hd_num; j++)
	{
		Type t = 0;
		for (int i = 0; i < in_num; i++)
			t += w[1][i][j] * x[0][i];
		t += b[1][j];
		x[1][j] = Sigmoid(t);
	}

	//计算输出层各节点的输出值  
	for (int j = 0; j < ou_num; j++)
	{
		Type t = 0;
		for (int i = 0; i < hd_num; i++)
			t += w[2][i][j] * x[1][i];
		t += b[2][j];
		x[2][j] = Sigmoid(t);
	}
}

//计算单个样本的误差  
Type BP::GetError()
{
	Type ans = 0;
	for (int i = 0; i < ou_num; i++)
		ans += 0.5 * (x[2][i] - data.y[i]) * (x[2][i] - data.y[i]);
	return ans;
}

//误差信号反向传递子过程  
void BP::ReverseTransfer()
{
	CalcDelta();
	UpdateNetWork();
}

//计算所有样本的精度  
Type BP::GetAccu()
{
	Type ans = 0;

		int m = data.x.size();
		for (int j = 0; j < m; j++)
			x[0][j] = data.x[j];
		ForwardTransfer();
		int n = data.y.size();
		for (int j = 0; j < n; j++)
			ans += 0.5 * (x[2][j] - data.y[j]) * (x[2][j] - data.y[j]);

	return ans;
}

//计算调整量  
void BP::CalcDelta()
{
	//计算输出层的delta值  
	for (int i = 0; i < ou_num; i++)
		d[2][i] = (x[2][i] - data.y[i]) * x[2][i] * (A - x[2][i]) / (A * B);
	//计算隐含层的delta值  
	for (int i = 0; i < hd_num; i++)
	{
		Type t = 0;
		for (int j = 0; j < ou_num; j++)
			t += w[2][i][j] * d[2][j];
		d[1][i] = t * x[1][i] * (A - x[1][i]) / (A * B);
	}
}

//根据计算出的调整量对BP网络进行调整  
void BP::UpdateNetWork()
{
	//隐含层和输出层之间权值和阀值调整  
	for (int i = 0; i < hd_num; i++)
	{
		for (int j = 0; j < ou_num; j++)
			w[2][i][j] -= ETA_W * d[2][j] * x[1][i];
	}
	for (int i = 0; i < ou_num; i++)
		b[2][i] -= ETA_B * d[2][i];

	//输入层和隐含层之间权值和阀值调整  
	for (int i = 0; i < in_num; i++)
	{
		for (int j = 0; j < hd_num; j++)
			w[1][i][j] -= ETA_W * d[1][j] * x[0][i];
	}
	for (int i = 0; i < hd_num; i++)
		b[1][i] -= ETA_B * d[1][i];
}

//计算Sigmoid函数的值  
Type BP::Sigmoid(const Type x)
{
	return A / (1 + exp(-x / B));
}