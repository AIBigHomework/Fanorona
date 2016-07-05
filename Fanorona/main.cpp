#include <iostream>
#include <ctime>
#include "constants.h"
#include "board.h"
#include "aiplayer.h"
#include "train.h"
#undef main

using namespace std;
extern BP bp;
//extern "C" { FILE __iob_func[3] = { *stdin,*stdout,*stderr }; }
SDL_Surface *Screen;

int Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		cout << "SDL Initialization failed: " << SDL_GetError() << endl;
		return -1;
	}

	Screen = SDL_SetVideoMode(ScreenWidth, ScreenHeight, PixelDepth, SDL_SWSURFACE);

	if (Screen == NULL)
	{
		printf("Failed to acquire video Mode: %s\n", SDL_GetError());
		return -1;
	}

	SDL_WM_SetCaption(Title, Title);

	return 0;
}

void CleanUp()
{
	SDL_FreeSurface(Screen);
	SDL_Quit();
}

int main()
{
	_Board Board;
	SDL_Event Event;
	bool Running = true;
	char AIColour1 = BLACK;
	char AIColour2 = WHITE;
	char nowColor = WHITE;
	int status = PLAY;
	int trainMark = 1;
	cout << "ÊÇ·ñÑµÁ·?(0/1)";
	cin >> trainMark;
	srand(time(NULL));
	if (trainMark)
	{
		training(10000);
		cout << "over" << endl;
	}

	bp.Read("saved-10000");

	
	Initialize();
	Populate(Board);
	PopulateGUI(Board, Screen);
	
	while (Running)
	{
		while (SDL_PollEvent(&Event))
		{
			switch (Event.type)
			{
			case SDL_QUIT:
				Running = false;
				break;
			default:
				break;
			}
		}
		if (status == PLAY)
		{
			if (nowColor == AIColour1)
			{
				AIPlay(Board, Screen, AIColour1, TRAINING);
				char w = win(Board);
				
				if (w != EMPTY)
				{
					char str[3][6] = { "","BLACK","WHITE" };
					status = PAUSE;
					cout << str[w] << " win!" << endl;
				}
				
			} 
			else
			{
				AIPlay(Board, Screen, AIColour2, PRUNING);
				char w = win(Board);
				
				if (w != EMPTY)
				{
					char str[3][6] = { "","BLACK","WHITE" };
					status = PAUSE;
					cout << str[w] << " win!" << endl;
				}
				
			}
			nowColor = 3 - nowColor;
		}
			
	}

	CleanUp();
	return 0;
}
