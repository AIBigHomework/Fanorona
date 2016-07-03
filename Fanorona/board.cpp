#include "board.h"
#include "constants.h"

bool neighbors[ROWS][COLUMNS][8] = {
	{
		{ 0, 0, 0,
		  0,    1,
		  0, 1, 1 },//0
		{ 0, 0, 0,
		  1,    1,
		  0, 1, 0 },//1
		{ 0, 0, 0,
		  1,    1,
		  1, 1, 1 },//2
		{ 0, 0, 0,
		  1,    1,
		  0, 1, 0 },//3
		{ 0, 0, 0,
		  1,    1,
		  1, 1, 1 },//4
		{ 0, 0, 0,
		  1,    1,
		  0, 1, 0 },//5
		{ 0, 0, 0,
		  1,    1,
		  1, 1, 1 },//6
		{ 0, 0, 0,
		  1,    1,
		  0, 1, 0 },//7
		{ 0, 0, 0,
		  1,    0,
		  1, 1, 0 },//8
	},
	{
		{ 0, 1, 0,
		  0,    1,
		  0, 1, 0 },//0
		{ 1, 1, 1,
		  1,    1,
		  1, 1, 1 },//1
		{ 0, 1, 0,
		  1,    1,
		  0, 1, 0 },//2
		{ 1, 1, 1,
		  1,    1,
		  1, 1, 1 },//3
		{ 0, 1, 0,
		  1,    1,
		  0, 1, 0 },//4
		{ 1, 1, 1,
		  1,    1,
		  1, 1, 1 },//5
		{ 0, 1, 0,
		  1,    1,
		  0, 1, 0 },//6
		{ 1, 1, 1,
		  1,    1,
		  1, 1, 1 },//7
		{ 0, 1, 0,
		  1,    0,
		  0, 1, 0 },//8
	},
	{
		{ 0, 1, 1,
		  0,    1,
		  0, 1, 1 },//0
		{ 0, 1, 0,
		  1,    1,
		  0, 1, 0 },//1
		{ 1, 1, 1,
		  1,    1,
		  1, 1, 1 },//2
		{ 0, 1, 0,
		  1,    1,
		  0, 1, 0 },//3
		{ 1, 1, 1,
		  1,    1,
		  1, 1, 1 },//4
		{ 0, 1, 0,
		  1,    1,
		  0, 1, 0 },//5
		{ 1, 1, 1,
		  1,    1,
		  1, 1, 1 },//6
		{ 0, 1, 0,
		  1,    1,
		  0, 1, 0 },//7
		{ 1, 1, 0,
		  1,    0,
		  1, 1, 0 },//8
	},
	{
		{ 0, 1, 0,
		  0,    1,
		  0, 1, 0 },//0
		{ 1, 1, 1,
		  1,    1,
		  1, 1, 1 },//1
		{ 0, 1, 0,
		  1,    1,
		  0, 1, 0 },//2
		{ 1, 1, 1,
		  1,    1,
		  1, 1, 1 },//3
		{ 0, 1, 0,
		  1,    1,
		  0, 1, 0 },//4
		{ 1, 1, 1,
		  1,    1,
		  1, 1, 1 },//5
		{ 0, 1, 0,
		  1,    1,
		  0, 1, 0 },//6
		{ 1, 1, 1,
		  1,    1,
		  1, 1, 1 },//7
		{ 0, 1, 0,
		  1,    0,
		  0, 1, 0 },//8
	},
	{
		{ 0, 1, 1,
		  0,    1,
		  0, 0, 0 },//0
		{ 0, 1, 0,
		  1,    1,
		  0, 0, 0 },//1
		{ 1, 1, 1,
		  1,    1,
		  0, 0, 0 },//2
		{ 0, 1, 0,
		  1,    1,
		  0, 0, 0 },//3
		{ 1, 1, 1,
		  1,    1,
		  0, 0, 0 },//4
		{ 0, 1, 0,
		  1,    1,
		  0, 0, 0 },//5
		{ 1, 1, 1,
		  1,    1,
		  0, 0, 0 },//6
		{ 0, 1, 0,
		  1,    1,
		  0, 0, 0 },//7
		{ 1, 1, 0,
		  1,    0,
		  0, 0, 0 },//8
	}
};
int neighbors2[8][2] = { {-1,-1},{-1,0},{-1,1},{0,-1},{0,1}, {1,-1}, {1,0}, {1,1} };

void Populate(_Board &Board)
{
	int x = 0;
	int y = 0;

	while (y < ROWS)
	{
		x = 0;
		while (x < COLUMNS)
		{
			if (y < 2)
			{
				Board.data[y][x] = BLACK;
			}
			else if (y > 2)
			{
				Board.data[y][x] = WHITE;
			}
			else
			{
				if (x < 4)
				{
					if (x % 2 == 0)
					{
						Board.data[y][x] = WHITE;
					}
					else
					{
						Board.data[y][x] = BLACK;
					}
				}
				else if (x > 4)
				{
					if (x % 2 == 0)
					{
						Board.data[y][x] = BLACK;
					}
					else
					{
						Board.data[y][x] = WHITE;
					}
				}
				else
				{
					Board.data[y][x] = EMPTY;
				}
			}
			x++;
		}
		y++;
	}
}

void PopulateGUI(_Board Board, SDL_Surface *Screen)
{
	static SDL_Surface* iBlackStone = IMG_Load(BlackStone);
	static SDL_Surface* iSelectedBlackStone = IMG_Load(SelectedBlackStone);
	static SDL_Surface* iDeleteBlackStone = IMG_Load(DeleteBlackStone);

	static SDL_Surface* iWhiteStone = IMG_Load(WhiteStone);
	static SDL_Surface* iSelectedWhiteStone = IMG_Load(SelectedWhiteStone);
	static SDL_Surface* iDeleteWhiteStone = IMG_Load(DeleteWhiteStone);

	static SDL_Surface* iBackground = IMG_Load(Background);

	if (iBlackStone == NULL || iSelectedBlackStone == NULL || iDeleteBlackStone == NULL ||
		iWhiteStone == NULL || iSelectedWhiteStone == NULL || iDeleteWhiteStone == NULL ||
		iBackground == NULL
		)
	{
		printf("Failed to load images: %s\n", IMG_GetError());
	}

	SDL_Rect ScreenRect;
	ScreenRect.h = ScreenHeight;
	ScreenRect.w = ScreenWidth;
	ScreenRect.x = 0;
	ScreenRect.y = 0;

	SDL_BlitSurface(iBackground, NULL, Screen, &ScreenRect);

	SDL_Rect ImageRect;
	ImageRect.h = StoneSize;
	ImageRect.w = StoneSize;
	int Start = 50 - StoneSize / 2;

	for (int i = 0; i < ROWS; i++)
	{
		ImageRect.y = Start + StoneSpace*i;
		for (int j = 0; j < COLUMNS; j++)
		{
			ImageRect.x = Start + StoneSpace*j;

			switch (Board.data[i][j])
			{
			case BLACK:
				SDL_BlitSurface(iBlackStone, NULL, Screen, &ImageRect);
				break;
			case SELECTED_BLACK:
				SDL_BlitSurface(iSelectedBlackStone, NULL, Screen, &ImageRect);
				break;
			case DELETE_APPROACH_BLACK:
			case DELETE_WITHDRAW_BLACK:
				SDL_BlitSurface(iDeleteBlackStone, NULL, Screen, &ImageRect);
				break;
			case WHITE:
				SDL_BlitSurface(iWhiteStone, NULL, Screen, &ImageRect);
				break;
			case SELECTED_WHITE:
				SDL_BlitSurface(iSelectedWhiteStone, NULL, Screen, &ImageRect);
				break;
			case DELETE_APPROACH_WHITE:
			case DELETE_WITHDRAW_WHITE:
				SDL_BlitSurface(iDeleteWhiteStone, NULL, Screen, &ImageRect);
				break;
			}
		}
	}
	SDL_UpdateRect(Screen, 0, 0, 0, 0);
}

std::vector<_Stone> GetFreeStones(_Board board, char color)
{
	std::vector<_Stone> freeStones;
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLUMNS; j++)
		{
			if (board.data[i][j] == color)
			{
				if ((neighbors[i][j][0] && board.data[i - 1][j - 1] == EMPTY) ||
					(neighbors[i][j][1] && board.data[i - 1][j] == EMPTY) ||
					(neighbors[i][j][2] && board.data[i - 1][j + 1] == EMPTY) ||
					(neighbors[i][j][3] && board.data[i][j - 1] == EMPTY) ||
					(neighbors[i][j][4] && board.data[i][j + 1] == EMPTY) ||
					(neighbors[i][j][5] && board.data[i + 1][j - 1] == EMPTY) ||
					(neighbors[i][j][6] && board.data[i + 1][j] == EMPTY) ||
					(neighbors[i][j][7] && board.data[i + 1][j + 1] == EMPTY))
				{
					freeStones.push_back({ i,j });
				}
			}
		}
	}
	return freeStones;
}

char win(_Board Board)
{
	int v1 = 0, v2 = 0;
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLUMNS; j++)
		{
			if (Board.data[i][j] == BLACK)
			{
				v1 += 1;

			}
			else if (Board.data[i][j] == WHITE)
			{
				v2 += 1;
			}
		}
	}
	if (v1 == 0)
	{
		return WHITE;
	}
	if (v2 == 0)
	{
		return BLACK;
	}
	return EMPTY;
}