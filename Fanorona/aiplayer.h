#pragma once
#include "board.h"
struct _ActionBase
{
	int x, y;
	int removedNum;
	_Stone removed[8];
};
struct _Action
{
	_Stone stone;
	std::vector<_ActionBase> actions;
};
struct _Node
{
	_Board board;
	_Action lastaction;
	double alpha, beta;
};
void AIPlay(_Board &Board, SDL_Surface* Screen, char AIColour, int Mode);
void AlphaBetaPlay(_Board &Board, SDL_Surface* Screen, char AIColour, int depth);
_Node AlphaPlay(_Node Root, int depth, double a, double b);
_Node BetaPlay(_Node Root, int depth, double a, double b);
std::vector<_Node> GetPossable(_Node node, char AIColor);