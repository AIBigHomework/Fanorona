#include "aiplayer.h"
#include <queue>
#include <ctime>
#include <iostream>
using namespace std;
extern bool neighbors[ROWS][COLUMNS][8];
extern int neighbors2[8][2];

void AIPlay(_Board &Board, SDL_Surface* Screen, char AIColour, int Mode)
{
	switch (Mode)
	{
	case GREEDY:
		break;
	case PRUNING:
		int depth = DEPTH;
		if (AIColour == WHITE)
		{
			depth--;
		}
		AlphaBetaPlay(Board, Screen, AIColour, depth);
		break;
	}
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
	_Stone nextStone = { stone.x + dx,stone.y + dy };
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

double value(_Node node)
{
	double v = 0;
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLUMNS; j++)
		{
			if (node.board.data[i][j] == BLACK)
			{
				v += 1;
			}
			else if(node.board.data[i][j] == WHITE)
			{
				v -= 1;
			}
		}
	}
	return v;
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

vector<_Node> GetPossable(_Node node, char AIColor)
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
					result.push_back(tmp);
				}
				
				
				while (!Q.empty())
				{
					_Node tmp = Q.front();
					vector<_Node> v = GetPossable(tmp);
					Q.pop();
					if (v.size() == 0)
					{
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
	return result;
}

_Node BetaPlay(_Node Root, int depth, double a, double b)
{
	vector<_Node> subs = GetPossable(Root, WHITE);
	double beta = b;
	_Node result;
	if (depth == 0)
	{
		result = Root;
		result.value = value(result);
		return result;
	}
	for (int i = 0; i < subs.size(); i++)
	{
		_Node tmp = AlphaPlay(subs[i], depth - 1, a, beta);
		if (tmp.value < beta)
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
	return result;
}

_Node AlphaPlay(_Node Root, int depth, double a, double b)
{
	vector<_Node> subs = GetPossable(Root, BLACK);
	double alpha = a;
	_Node result;
	if (depth == 0)
	{
		result = Root;
		result.value = value(result);
		return result;
	}
	for (int i = 0; i < subs.size(); i++)
	{
		_Node tmp = BetaPlay(subs[i], depth - 1, alpha, b);
		if (tmp.value > alpha)
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
	return result;
}

void AlphaBetaPlay(_Board &Board, SDL_Surface* Screen, char AIColour, int depth)
{
	_Node next;
	_Node Root = { Board,{},0 };
	if (AIColour == BLACK)//MAX�ڵ�
	{
		time_t start = clock();
		next = AlphaPlay(Root, depth, -DBL_MAX, DBL_MAX);
		time_t end = clock();
		cout << "BLACK used time : " << end - start << " ms" << endl;
	}
	else//MIN�ڵ�
	{
		time_t start = clock();
		next = BetaPlay(Root, depth, -DBL_MAX, DBL_MAX);
		time_t end = clock();
		cout << "WHITE used time : " << end - start << " ms" << endl;
	}
	_Action action = next.lastaction;
	ShowAction(Board, Screen, action);
	cout << "*********************************************" << endl;
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