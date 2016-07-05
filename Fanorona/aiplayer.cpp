#include "aiplayer.h"
#include "train.h"
#include <queue>
#include <ctime>
#include <iostream>
#include <algorithm>
using namespace std;
extern bool neighbors[ROWS][COLUMNS][8];
extern int neighbors2[8][2];

void AIPlay(_Board &Board, SDL_Surface* Screen, char AIColour, int Mode)
{
	switch (Mode)
	{
	case TESTING:
		TestingPlay(Board, AIColour);
		break;
	case PRUNING:
	case TRAINING:
		AlphaBetaPlay(Board, Screen, AIColour, DEPTH, Mode);
		break;
	case PRUNING2:
	case TRAINING2:
		AlphaBetaPlayForTrain(Board, AIColour, 1, Mode);
		break;
	}
}

void TestingPlay(_Board &Board, char AIColour)
{
	_Node Root = { Board,{},0 };
	vector<_Node> subs = GetPossable(Root, AIColour, TRAINING);
	int count = subs.size();
	if (count == 0)
	{
		return;
	}
	int ran = rand() % count;

	_Action action = subs[ran].lastaction;
	ShowAction(Board, action);
}

bool canRemoveFront(_Node &node, int remove)
{
	_Stone stone = { node.lastaction.actions.back().x,node.lastaction.actions.back().y };
	_Stone laststone;
	int actionNum = node.lastaction.actions.size();
	if (actionNum >= 2)
	{
		laststone = { node.lastaction.actions[actionNum - 2].x, node.lastaction.actions[actionNum - 2].y };
	}
	else
	{
		laststone = node.lastaction.stone;
	}
	int count = 0;
	char AIColor = node.board.data[stone.x][stone.y];
	char rmColor = 3 - AIColor;
	int dx = stone.x - laststone.x, dy = stone.y - laststone.y;
	_Stone nextStone = { stone.x + dx,stone.y + dy };
	while (nextStone.x>=0 && nextStone.x<=4 && nextStone.y>=0 && nextStone.y<=8 && node.board.data[nextStone.x][nextStone.y] == rmColor)
	{
		count++;
		if (remove)
		{
			node.lastaction.actions.back().removedNum = count;
			node.lastaction.actions.back().removed[count - 1] = nextStone;
			node.board.data[nextStone.x][nextStone.y] = EMPTY;
		}
		nextStone = { nextStone.x + dx,nextStone.y + dy };
	}
	return count > 0;
}

bool canRemoveBack(_Node &node, int remove)
{
	_Stone stone = { node.lastaction.actions.back().x,node.lastaction.actions.back().y };
	_Stone laststone;
	int actionNum = node.lastaction.actions.size();
	if (actionNum >= 2)
	{
		laststone = { node.lastaction.actions[actionNum - 2].x, node.lastaction.actions[actionNum - 2].y };
	}
	else
	{
		laststone = node.lastaction.stone;
	}
	int count = 0;
	char AIColor = node.board.data[stone.x][stone.y];
	char rmColor = 3 - AIColor;
	int dx = -stone.x + laststone.x, dy = -stone.y + laststone.y;
	_Stone nextStone = { laststone.x + dx,laststone.y + dy };
	while (nextStone.x >= 0 && nextStone.x <= 4 && nextStone.y >= 0 && nextStone.y <= 8 && node.board.data[nextStone.x][nextStone.y] == rmColor)
	{
		count++;
		if (remove)
		{
			node.lastaction.actions.back().removedNum = count;
			node.lastaction.actions.back().removed[count - 1] = nextStone;
			node.board.data[nextStone.x][nextStone.y] = EMPTY;
		}
		nextStone = { nextStone.x + dx,nextStone.y + dy };
	}
	return count > 0;
}

double value(_Node node, int mode, char AIColor)
{
	if (mode == TRAINING || mode == TRAINING2)
	{
		return BPvalue(node);
	}
	double v1 = 0, v2 = 0;
	double x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLUMNS; j++)
		{
			if (node.board.data[i][j] == BLACK)
			{
				v1 += 1;
				x1 += i;
				y1 += j;
			}
			else if(node.board.data[i][j] == WHITE)
			{
				v2 += 1;
				x2 += i;
				y2 += j;
			}
		}
	}
	x1 /= (v1 == 0 ? 1 : v1); x2 /= (v2 == 0 ? 1 : v2); y1 /= (v1 == 0 ? 1 : v1); y2 /= (v2 == 0 ? 1 : v2);
	double disx = x1 - x2 > 0 ? x1 - x2 : x2 - x1;
	double disy = y1 - y2 > 0 ? y1 - y2 : y2 - y1;
	double dis = (v1 == 0 || v2 == 0) ? 1 : disx + disy;
	if (AIColor == BLACK)
	{
		return 100 * (v1 - v2) - dis;
	}
	else
	{
		return 100 * (v1 - v2) + dis;
	}
	
}

vector<_Node> GetPossable(_Node node)
{
	vector<_Node> result;
	_Stone stone = { node.lastaction.actions.back().x,node.lastaction.actions.back().y };
	_Stone laststone;
	int actionNum = node.lastaction.actions.size();
	if (actionNum >= 2)
	{
		laststone = { node.lastaction.actions[actionNum - 2].x, node.lastaction.actions[actionNum - 2].y };
	}
	else
	{
		laststone = node.lastaction.stone;
	}
	int dx = stone.x - laststone.x, dy = stone.y - laststone.y;
	for (int j = 0; j < 8; j++)
	{
		if (neighbors[stone.x][stone.y][j] && node.board.data[stone.x + neighbors2[j][0]][stone.y + neighbors2[j][1]] == EMPTY)
		{
			if ((neighbors2[j][0] == dx && neighbors2[j][1] == dy) || (neighbors2[j][0] == -dx && neighbors2[j][1] == -dy))
				continue;
			_Node tmp = node;
			char AIColor = tmp.board.data[stone.x][stone.y];
			tmp.lastaction.actions.push_back({ stone.x + neighbors2[j][0], stone.y + neighbors2[j][1] });
			tmp.board.data[stone.x][stone.y] = EMPTY;
			tmp.board.data[stone.x + neighbors2[j][0]][stone.y + neighbors2[j][1]] = AIColor;
			_Node tmp2 = tmp;
			queue<_Node> Q;
			if (canRemoveFront(tmp, 0) || canRemoveBack(tmp, 0))
			{
				if (canRemoveFront(tmp, 1))
				{
					result.push_back(tmp);
				}
				if (canRemoveBack(tmp2,1))
				{
					result.push_back(tmp2);
				}
			}
			else
			{
				continue;
			}
			
		}
	}
	return result;
}

int compare1(_Node n1, _Node n2)
{
	return n1.value > n2.value;
}
int compare2(_Node n1, _Node n2)
{
	return n2.value < n1.value;
}
vector<_Node> GetPossable(_Node node, char AIColor, int mode)
{
	vector<_Node> result;
	vector<_Stone> freestones = GetFreeStones(node.board, AIColor);
	for (int i = 0; i < freestones.size(); i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (neighbors[freestones[i].x][freestones[i].y][j] && node.board.data[freestones[i].x + neighbors2[j][0]][freestones[i].y + neighbors2[j][1]] == EMPTY)
			{
				_Node tmp = node;
				tmp.lastaction = { freestones[i],{ { freestones[i].x + neighbors2[j][0],freestones[i].y + neighbors2[j][1] } } };
				tmp.board.data[freestones[i].x][freestones[i].y] = EMPTY;
				tmp.board.data[freestones[i].x + neighbors2[j][0]][freestones[i].y + neighbors2[j][1]] = AIColor;
				_Node tmp2 = tmp;
				queue<_Node> Q;
				if (canRemoveBack(tmp, 0) || canRemoveFront(tmp, 0))
				{
					if (canRemoveFront(tmp, 1))
					{
						Q.push(tmp);
					}
					if (canRemoveBack(tmp2, 1))
					{
						Q.push(tmp2);
					}
				}
				else
				{
					tmp.value = value(tmp, mode, AIColor);
					result.push_back(tmp);
				}
				
				
				while (!Q.empty())
				{
					_Node tmp = Q.front();
					vector<_Node> v = GetPossable(tmp);
					Q.pop();
					if (v.size() == 0)
					{
						tmp.value = value(tmp, mode, AIColor);
						result.push_back(tmp);
						continue;
					}
					for (int k = 0; k < v.size(); k++)
					{
						Q.push(v[k]);
					}
				}
			}
		}
		
		
	}
	if (AIColor == BLACK)
	{
		sort(result.begin(), result.end(), compare1);
	}
	else
	{
		sort(result.begin(), result.end(), compare2);
	}
	int s = result.size();
	if (mode == TESTING)
	{
		if (s > 0)
		{
			s = rand() % s + 2;
		}
		if (s > result.size())
		{
			s = result.size();
		}
	}

	result = vector<_Node>(result.begin(), result.begin() + s);
	return result;
}

_Node BetaPlay(_Node Root, int depth, double a, double b, int mode)
{
	_Node result = Root;
	if (depth == 0)
	{
		result = Root;
		result.value = value(result, mode, BLACK);
		return result;
	}
	vector<_Node> subs = GetPossable(Root, WHITE, mode);

	double beta = DBL_MAX;
	
	
	for (int i = 0; i < subs.size(); i++)
	{
		_Node tmp = AlphaPlay(subs[i], depth - 1, a, beta, mode);
		if (tmp.value <= beta)
		{
			beta = tmp.value;
			subs[i].value = tmp.value;
			result = subs[i];
		}
		if (tmp.value <= a)
		{
			return result;
		}
	}
	if (subs.size() == 0)
	{
		result.value = DBL_MAX;
	}
	return result;
}

_Node AlphaPlay(_Node Root, int depth, double a, double b, int mode)
{
	_Node result = Root;
	if (depth == 0)
	{
		result = Root;
		result.value = value(result, mode, WHITE);
		return result;
	}
	vector<_Node> subs = GetPossable(Root, BLACK, mode);

	double alpha = -DBL_MAX;
	
	for (int i = 0; i < subs.size(); i++)
	{
		_Node tmp = BetaPlay(subs[i], depth - 1, alpha, b, mode);
		if (tmp.value >= alpha)
		{
			alpha = tmp.value;
			subs[i].value = tmp.value;
			result = subs[i];
		}
		if (tmp.value >= b)
		{
			return result;
		}
	}
	if (subs.size() == 0)
	{
		result.value = -DBL_MAX;
	}
	return result;
}

void AlphaBetaPlay(_Board &Board, SDL_Surface* Screen, char AIColour, int depth, int mode)
{
	_Node next;
	_Node Root = { Board,{},0 };
	if (AIColour == BLACK)//MAX节点
	{
		time_t start = clock();
		next = AlphaPlay(Root, depth, -DBL_MAX, DBL_MAX, mode);
		time_t end = clock();
		cout << "BLACK used time : " << end - start << " ms" << endl;
	}
	else//MIN节点
	{
		time_t start = clock();
		next = BetaPlay(Root, depth, -DBL_MAX, DBL_MAX, mode);
		time_t end = clock();
		cout << "WHITE used time : " << end - start << " ms" << endl;
	}
	_Action action = next.lastaction;
	ShowAction(Board, Screen, action);
	cout << "*********************************************" << endl;
}

void AlphaBetaPlayForTrain(_Board &Board, char AIColour, int depth, int mode)
{
	_Node next;
	_Node Root = { Board,{},0 };
	if (AIColour == BLACK)//MAX节点
	{
		next = AlphaPlay(Root, depth, -DBL_MAX, DBL_MAX, mode);
	}
	else//MIN节点
	{
		next = BetaPlay(Root, depth, -DBL_MAX, DBL_MAX, mode);
	}
	_Action action = next.lastaction;
	ShowAction(Board, action);
}

void ShowAction(_Board &Board, SDL_Surface* Screen, _Action action)
{
	_Stone stone = action.stone;
	Board.data[stone.x][stone.y] += SELECT;//Selected
	PopulateGUI(Board, Screen);
	SDL_UpdateRect(Screen, 0, 0, 0, 0);
	SDL_Delay(300);
	for (int i = 0; i < action.actions.size(); i++)
	{
		cout << "(" << stone.x << "," << stone.y << ") --> (" << action.actions[i].x << "," << action.actions[i].y << ")";
		Board.data[action.actions[i].x][action.actions[i].y] = Board.data[stone.x][stone.y];//Move
		Board.data[stone.x][stone.y] = EMPTY;
		stone.x = action.actions[i].x;
		stone.y = action.actions[i].y;
		PopulateGUI(Board, Screen);
		SDL_UpdateRect(Screen, 0, 0, 0, 0);
		SDL_Delay(300);
		//Remove
		cout << "\t RM";
		for (int j = 0; j < action.actions[i].removedNum; j++)
		{
			Board.data[action.actions[i].removed[j].x][action.actions[i].removed[j].y] = EMPTY;
			cout << " (" << action.actions[i].removed[j].x << "," << action.actions[i].removed[j].y << ")";
		}
		cout << endl;
		PopulateGUI(Board, Screen);
		SDL_UpdateRect(Screen, 0, 0, 0, 0);
		SDL_Delay(300);
	}
	Board.data[stone.x][stone.y] -= SELECT;//Unselected
	PopulateGUI(Board, Screen);
	SDL_UpdateRect(Screen, 0, 0, 0, 0);
}

void ShowAction(_Board &Board, _Action action)
{
	_Stone stone = action.stone;
	for (int i = 0; i < action.actions.size(); i++)
	{
		Board.data[action.actions[i].x][action.actions[i].y] = Board.data[stone.x][stone.y];
		Board.data[stone.x][stone.y] = EMPTY;
		stone.x = action.actions[i].x;
		stone.y = action.actions[i].y;

		//Remove
		for (int j = 0; j < action.actions[i].removedNum; j++)
		{
			Board.data[action.actions[i].removed[j].x][action.actions[i].removed[j].y] = EMPTY;
		}
	}
}