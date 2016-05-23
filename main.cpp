#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <random>
#include <math.h>

#define WIDTH 640
#define HEIGHT 480

int dX = 10;
int dY = 0;

std::vector<SDL_Rect> tail;
std::vector<SDL_Rect> food;

SDL_Rect new_SDL_Rect(int x, int y, int w, int h)
{
	SDL_Rect ret;
	ret.x = x;
	ret.y = y;
	ret.w = w;
	ret.h = h;
	return ret;
}

int main()
{
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> distW(0, WIDTH - 10);
	std::uniform_int_distribution<int> distH(0, HEIGHT - 10);

	int food_count;
	std::cin >> food_count;
	for(int i = 0; i < food_count; i++)
	{
		food.push_back(new_SDL_Rect(distW(rng), distH(rng), 10, 10));
	}

	tail.push_back(new_SDL_Rect(10, 10, 10, 10));
	tail.push_back(new_SDL_Rect(0, 0, 10, 10));

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_Event event;

	SDL_Window *win = SDL_CreateWindow("Snake", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
	if (win == nullptr)
	{
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr)
	{
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	std::string body_part = "./body_part.bmp";
	std::string food_image = "./food.bmp";

	SDL_Surface *bmp = SDL_LoadBMP(body_part.c_str());
	if (bmp == nullptr)
	{
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);
	SDL_FreeSurface(bmp);
	bmp = SDL_LoadBMP(food_image.c_str());
	SDL_Texture *tex2 = SDL_CreateTextureFromSurface(ren, bmp);
	if (tex == nullptr)
	{
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}
	bool runs = true;
	bool canImove;
	while(runs)
	{
		canImove = true;
		for(size_t i = 1; i < food.size(); i++)
		{
			if (tail[i].x < tail[0].x + 10 && tail[i].x + 10 > tail[0].x 
					&& tail[i].y < tail[0].y+10 && tail[i].y + 10 > tail[0].y) 
			{
				std::cout<<"You died!!!"<<std::endl;
				return 0;
			}
		}
		for(size_t i = 0; i < food.size(); i++)
		{
			if (food[i].x < tail[0].x + 10 && food[i].x + 10 > tail[0].x 
					&& food[i].y < tail[0].y+10 && food[i].y + 10 > tail[0].y) 
			{
				food[i] = new_SDL_Rect(distW(rng), distH(rng), 10, 10);
				tail.push_back(tail[tail.size() - 1]);
				while(food[i].x % 10 != 0)
				{
					food[i].x --;
				}
				while(food[i].y % 10 != 0)
				{
					food[i].y --;
				}
			}
		}
		for(size_t i = tail.size() - 1; i >= 1; i--)
		{
			tail[i] = tail[i-1];
		}
		tail[0].x += dX;
		tail[0].y += dY;
		SDL_RenderClear(ren);
		for(size_t i = 0; i < food.size(); i++)
		{
			SDL_RenderCopy(ren, tex2, nullptr, &food[i]);
		}
		for(size_t i = 0; i < tail.size(); i++)
		{
			SDL_RenderCopy(ren, tex, nullptr, &tail[i]);
		}

		SDL_RenderPresent(ren);
		SDL_Delay(100);
		while(SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					runs = false;
					break;
				case SDL_KEYDOWN:
					if(canImove)
					{
						switch(event.key.keysym.sym)
						{
							case SDLK_UP:
								if(dY!=10)
								{
									dX = 0;
									dY = -10;
									canImove = false;
								}
								break;
							case SDLK_DOWN:
								if(dY!= -10)
								{
									dX = 0;
									dY = 10;
									canImove = false;
								}
								break;
							case SDLK_LEFT:
								if(dX != 10)
								{
									dX = -10;
									dY = 0;
									canImove = false;
								}
								break;
							case SDLK_RIGHT:
								if(dX != -10)
								{
									dX = 10;
									dY = 0;
									canImove = false;
								}
								break;
							case SDLK_ESCAPE:
								runs = false;
								break;
						}
					}
					break;
			}
		}
	}

	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
