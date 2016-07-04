#define _CRT_SECURE_NO_WARNINGS
#include "train.h"
#include <iostream>
using namespace std;

BP bp = BP(ROWS*COLUMNS, 2);
double BPvalue(_Node node)
{
	vector<double> input;
	vector<double> output;
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLUMNS; j++)
		{
			input.push_back(node.board.data[i][j]);
		}
	}
	output = bp.ForeCast(input);
	return output[0] - output[1];
}
void training(int time)
{
	_Board Board;

	for (int t = 1; t <= time; t++)
	{
		Populate(Board);
		char AIColour1 = BLACK;
		char AIColour2 = WHITE;
		char nowColor = WHITE;
		int status = PLAY;
		std::stack<Data> stk;
		while (status == PLAY)
		{
			if (nowColor == AIColour1)
			{
				AIPlay(Board, NULL, AIColour1, TESTING);
			}
			else
			{
				AIPlay(Board, NULL, AIColour2, TESTING);
			}
			Data d;
			std::vector<double> input;
			for (int i = 0; i < ROWS; i++)
			{
				for (int j = 0; j < COLUMNS; j++)
				{
					input.push_back(Board.data[i][j]);
				}
			}
			d.x = input;
			stk.push(d);
			nowColor = 3 - nowColor;
			char w = win(Board);
			if (w != EMPTY)
			{
				cout << "Get a sample, finished ";
				status = PAUSE;
				if (w == WHITE)
				{
					stk.top().y = { 0,1 };
				}
				else
				{
					stk.top().y = { 1,0 };
				}
				
				bp.TrainFanorona(stk);
			}
			
		}
		cout  << t << " training." << endl;
		if (t % 1000 == 0)
		{
			char filename[20];
			sprintf(filename, "saved-%d", t);
			bp.Write(filename);
		}
	}
	char filename[20];
	sprintf(filename, "saved-%d", time);
	bp.Write(filename);
}