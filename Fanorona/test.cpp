#define _CRT_SECURE_NO_WARNINGS
#include <iostream>  
#include <string.h>  
#include <stdio.h>  

#include "BP.h"  

using namespace std;

double sample[41][4] =
{
	{ 0,0,0,0 },
	{ 5,1,4,19.020 },
	{ 5,3,3,14.150 },
	{ 5,5,2,14.360 },
	{ 5,3,3,14.150 },
	{ 5,3,2,15.390 },
	{ 5,3,2,15.390 },
	{ 5,5,1,19.680 },
	{ 5,1,2,21.060 },
	{ 5,3,3,14.150 },
	{ 5,5,4,12.680 },
	{ 5,5,2,14.360 },
	{ 5,1,3,19.610 },
	{ 5,3,4,13.650 },
	{ 5,5,5,12.430 },
	{ 5,1,4,19.020 },
	{ 5,1,4,19.020 },
	{ 5,3,5,13.390 },
	{ 5,5,4,12.680 },
	{ 5,1,3,19.610 },
	{ 5,3,2,15.390 },
	{ 1,3,1,11.110 },
	{ 1,5,2,6.521 },
	{ 1,1,3,10.190 },
	{ 1,3,4,6.043 },
	{ 1,5,5,5.242 },
	{ 1,5,3,5.724 },
	{ 1,1,4,9.766 },
	{ 1,3,5,5.870 },
	{ 1,5,4,5.406 },
	{ 1,1,3,10.190 },
	{ 1,1,5,9.545 },
	{ 1,3,4,6.043 },
	{ 1,5,3,5.724 },
	{ 1,1,2,11.250 },
	{ 1,3,1,11.110 },
	{ 1,3,3,6.380 },
	{ 1,5,2,6.521 },
	{ 1,1,1,16.000 },
	{ 1,3,2,7.219 },
	{ 1,5,3,5.724 }
};

int main()
{
	Vector<Data> data;
	for (int i = 0; i < 41; i++)
	{
		Data t;
		for (int j = 0; j < 3; j++)
			t.x.push_back(sample[i][j]);

		t.y.push_back(sample[i][0] + sample[i][1] + sample[i][2]);
		data.push_back(t);
	}
	BP *bp = new BP();
	bp->GetData(data);
	bp->Train();


	while (1)
	{
		Vector<Type> in;
		for (int i = 0; i < 3; i++)
		{
			Type v;
			scanf("%lf", &v);
			in.push_back(v);
		}
		Vector<Type> ou;
		ou = bp->ForeCast(in);
		printf("%lf\n", ou[0]);
	}
	return 0;
}