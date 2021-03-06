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
	double value;
};
void AIPlay(_Board &Board, SDL_Surface* Screen, char AIColour, int Mode);
void AlphaBetaPlay(_Board &Board, SDL_Surface* Screen, char AIColour, int depth, int mode);
_Node AlphaPlay(_Node Root, int depth, double a, double b, int mode);
_Node BetaPlay(_Node Root, int depth, double a, double b, int mode);
std::vector<_Node> GetPossable(_Node node, char AIColor, int mode);
std::vector<_Node> GetPossable(_Node node);
bool canRemoveBack(_Node &node, int remove);
bool canRemoveFront(_Node &node, int remove);
void ShowAction(_Board &Board, _Action action);
void ShowAction(_Board &Board, SDL_Surface *Screen, _Action action);
void TestingPlay(_Board &Board, char AIColour);
double value(_Node node, int mode, char AIColor);
void AlphaBetaPlayForTrain(_Board &Board, char AIColour, int depth, int mode);
