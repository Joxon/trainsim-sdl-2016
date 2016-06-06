#include <stdexcept>
#include <string>
#include <iostream>
#include "SDL.h"
#include "SDL_image.h"

//屏幕宽度
const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;

//全局窗口和渲染器
SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

//记录SDL错误
void logSDLError(std::ostream &os, const std::string &msg)
{
	os << msg << " error: " << SDL_GetError() << std::endl;
}

//加载图像材质
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren)
{
	SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
	if (texture == nullptr) logSDLError(std::cout, "LoadTexture");
	return texture;
}


//根据坐标生成截取区域并复制输出到渲染器
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip = nullptr)
{
	//目标截面区域初始化，提供目标在渲染器的坐标
	SDL_Rect dst; dst.x = x; dst.y = y;
	//源截面区域不为空，将其值赋给目标截面
	if (clip != nullptr) { dst.w = clip->w; dst.h = clip->h; }
	//将截面输出到渲染器
	SDL_RenderCopy(ren, tex, clip, &dst);
}

int main(int argc, char** argv)
{
	//初始化SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		std::cout << SDL_GetError() << std::endl;
		return 1;
	}

	//创建窗口
	window = SDL_CreateWindow("Sprite test",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr)
	{
		std::cout << SDL_GetError() << std::endl;
		return 2;
	}

	//创建渲染器
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr)
	{
		std::cout << SDL_GetError() << std::endl;
		return 3;
	}

	//创建纹理
	SDL_Texture *image = nullptr;
	try
	{
		image = loadTexture("sprite.png", renderer);
	}
	catch (const std::runtime_error &e)
	{
		std::cout << e.what() << std::endl;
		return 4;
	}

	//iW，iH 表示截面宽高
	int iW = 100, iH = 100;
	//x，y 分别为截面在渲染器输出的坐标
	int x = SCREEN_WIDTH / 2 - iW / 2;
	int y = SCREEN_HEIGHT / 2 - iH / 2;

	//为精灵图设置截面
	SDL_Rect clips[4];

	//初始化截面信息
	for (int i = 0; i < 4; ++i)
	{
		clips[i].x = i / 2 * iW;
		clips[i].y = i % 2 * iH;
		clips[i].w = iW;
		clips[i].h = iH;
	}

	//表示将要输出的截面
	int useClip = 0;

	SDL_Event e;
	bool quit = false;
	//主循环
	while (!quit)
	{
		//事件轮询
		while (SDL_PollEvent(&e))
		{
			//按右上角的X退出
			if (e.type == SDL_QUIT) quit = true;

			//点击鼠标随机输出截面
			if (e.type == SDL_MOUSEBUTTONDOWN) useClip = rand() % 4;

			//使用数字键决定输出截面，分别有1，2，3，4
			if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_1:
				case SDLK_KP_1:
					useClip = 0;
					break;
				case SDLK_2:
				case SDLK_KP_2:
					useClip = 1;
					break;
				case SDLK_3:
				case SDLK_KP_3:
					useClip = 2;
					break;
				case SDLK_4:
				case SDLK_KP_4:
					useClip = 3;
					break;
				case SDLK_ESCAPE:
					quit = true;
					break;
				default:
					break;
				}
			}
		}
		//清空渲染器
		SDL_RenderClear(renderer);
		//绘制材质
		renderTexture(image, renderer, x, y, &clips[useClip]);
		//呈现渲染器
		SDL_RenderPresent(renderer);
	}

	//释放资源
	SDL_DestroyTexture(image);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}
