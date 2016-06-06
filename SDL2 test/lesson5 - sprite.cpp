#include <stdexcept>
#include <string>
#include <iostream>
#include "SDL.h"
#include "SDL_image.h"

//��Ļ���
const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;

//ȫ�ִ��ں���Ⱦ��
SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

//��¼SDL����
void logSDLError(std::ostream &os, const std::string &msg)
{
	os << msg << " error: " << SDL_GetError() << std::endl;
}

//����ͼ�����
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren)
{
	SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
	if (texture == nullptr) logSDLError(std::cout, "LoadTexture");
	return texture;
}


//�����������ɽ�ȡ���򲢸����������Ⱦ��
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip = nullptr)
{
	//Ŀ����������ʼ�����ṩĿ������Ⱦ��������
	SDL_Rect dst; dst.x = x; dst.y = y;
	//Դ��������Ϊ�գ�����ֵ����Ŀ�����
	if (clip != nullptr) { dst.w = clip->w; dst.h = clip->h; }
	//�������������Ⱦ��
	SDL_RenderCopy(ren, tex, clip, &dst);
}

int main(int argc, char** argv)
{
	//��ʼ��SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		std::cout << SDL_GetError() << std::endl;
		return 1;
	}

	//��������
	window = SDL_CreateWindow("Sprite test",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr)
	{
		std::cout << SDL_GetError() << std::endl;
		return 2;
	}

	//������Ⱦ��
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr)
	{
		std::cout << SDL_GetError() << std::endl;
		return 3;
	}

	//��������
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

	//iW��iH ��ʾ������
	int iW = 100, iH = 100;
	//x��y �ֱ�Ϊ��������Ⱦ�����������
	int x = SCREEN_WIDTH / 2 - iW / 2;
	int y = SCREEN_HEIGHT / 2 - iH / 2;

	//Ϊ����ͼ���ý���
	SDL_Rect clips[4];

	//��ʼ��������Ϣ
	for (int i = 0; i < 4; ++i)
	{
		clips[i].x = i / 2 * iW;
		clips[i].y = i % 2 * iH;
		clips[i].w = iW;
		clips[i].h = iH;
	}

	//��ʾ��Ҫ����Ľ���
	int useClip = 0;

	SDL_Event e;
	bool quit = false;
	//��ѭ��
	while (!quit)
	{
		//�¼���ѯ
		while (SDL_PollEvent(&e))
		{
			//�����Ͻǵ�X�˳�
			if (e.type == SDL_QUIT) quit = true;

			//����������������
			if (e.type == SDL_MOUSEBUTTONDOWN) useClip = rand() % 4;

			//ʹ�����ּ�����������棬�ֱ���1��2��3��4
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
		//�����Ⱦ��
		SDL_RenderClear(renderer);
		//���Ʋ���
		renderTexture(image, renderer, x, y, &clips[useClip]);
		//������Ⱦ��
		SDL_RenderPresent(renderer);
	}

	//�ͷ���Դ
	SDL_DestroyTexture(image);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}
