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


vector<_Node> GetPossable(_Node node)
{
	vector<_Node> result;
	_Stone stone = { node.lastaction.actions.back().x,node.lastaction.actions.back().y };
	for (int j = 0; j < 8; j++)
	{
		if (neighbors[stone.x][stone.y][j] && node.board.data[stone.x + neighbors2[j][0]][stone.y + neighbors2[j][1]] == EMPTY)
		{
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
				if (canRemoveFront(tmp, 1))
				{
					Q.push(tmp);
				}
				if (canRemoveBack(tmp2, 1))
				{
					Q.push(tmp2);
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