#include "aiplayer.h"
#include <queue>
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
		AlphaBetaPlay(Board, Screen, AIColour, DEPTH);
		break;
	}
}

bool canRemoveFront(_Node &node, int remove)
{

}

bool canRemoveBack(_Node &node, int remove)
{

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
				tmp.lastaction = { freestones[i],{ { freestones[i].x - 1,freestones[i].y - 1 } } };
				tmp.board.data[freestones[i].x][freestones[i].y] = EMPTY;
				tmp.board.data[freestones[i].x - 1][freestones[i].y - 1] = AIColor;
				queue<_Node> Q;
				Q.push(tmp);
				while (!Q.empty())
				{
					_Node tmp = Q.front(), tmp2 = Q.front();
					Q.pop();
					if (canRemoveFront(tmp))
					{

					}
					if (canRemoveBack(tmp2))
					{

					}
					result.push_back(tmp);
				}
				result.push_back(tmp);
			}
		}
		
		
	}
	return result;
}

_Node BetaPlay(_Node Root, int depth, double a, double b)
{
	_Node result;
	return result;
}

_Node AlphaPlay(_Node Root, int depth, double a, double b)
{
	vector<_Node> subs = GetPossable(Root, BLACK);
	double alpha = a;
	_Node result;
	for (int i = 0; i < subs.size(); i++)
	{
		_Node tmp = BetaPlay(subs[i], depth - 1, alpha, b);
		if (tmp.alpha > alpha)
		{
			alpha = tmp.alpha;
			result = tmp;
		}
	}
	return result;
}

void AlphaBetaPlay(_Board &Board, SDL_Surface* Screen, char AIColour, int depth)
{
	_Node next;
	_Node Root = { Board,{},-DBL_MAX,DBL_MAX };
	if (AIColour == BLACK)//MAX节点
	{
		next = AlphaPlay(Root, depth, -DBL_MAX, DBL_MAX);
	}
	else//MIN节点
	{
		next = BetaPlay(Root, depth, -DBL_MAX, DBL_MAX);
	}
	_Action action = next.lastaction;

}