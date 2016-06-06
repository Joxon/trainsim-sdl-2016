#include <iostream>
#include "SDL.h"
#include "SDL_ttf.h"

int main(int argc, char** argv)
{
	//屏幕宽度
	const int SCREEN_WIDTH = 680;
	const int SCREEN_HEIGHT = 400;
	//初始化SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	//初始化字体库
	TTF_Init();
	//创建窗口
	SDL_Window *window = SDL_CreateWindow("Font Test",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	//创建渲染器
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	//打开字体
	TTF_Font *font = TTF_OpenFont("XHei.TTC", 30);
	//文本转表面
	SDL_Color color = { 255, 255, 255 };
	SDL_Surface *surf = TTF_RenderText_Blended(font, "Cool, Cooler, Coolest!", color);
	//表面转纹理
	SDL_Texture *text = SDL_CreateTextureFromSurface(renderer, surf);
	//释放资源
	SDL_FreeSurface(surf);
	TTF_CloseFont(font);

	bool quit = false;
	SDL_Event e;
	//主循环
	while (!quit)
	{
		//事件栈轮询
		while (SDL_PollEvent(&e))
		{
			//按右上角的X或点鼠标退出
			if (e.type == SDL_QUIT || e.type == SDL_MOUSEBUTTONDOWN) quit = true;
			//清空渲染器
			SDL_RenderClear(renderer);
			//渲染字体
			SDL_RenderCopy(renderer, text, NULL, NULL);
			//呈现渲染器
			SDL_RenderPresent(renderer);
		}
	}

	//释放资源
	SDL_DestroyTexture(text);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}
