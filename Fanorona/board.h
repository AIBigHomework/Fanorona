#pragma once
#include "constants.h"
#include "stone.h"
#ifndef _SDL_H
#include "SDL/include/SDL.h"
#include "SDL/include/SDL_image.h"
#endif // _SDL_H

#include <vector>

struct _Board
{
	char data[ROWS][COLUMNS]; 
	int turn;
};

void Populate(_Board &Board);
void PopulateGUI(_Board Board, SDL_Surface *Screen);

std::vector<_Stone> GetFreeStones(_Board board, char color);
char win(_Board board);