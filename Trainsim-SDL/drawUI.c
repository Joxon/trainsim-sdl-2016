#include<SDL.h>
#include<SDL_ttf.h>
#include"drawSDL.h"
#include"var.h"

extern int trainNum;
extern struct train train[MAX_TRAIN];
extern SDL_Rect buttonClip[BUTTON_ROW][BUTTON_COLUMN];

//按钮状态枚举
enum buttonState
{
	//鼠标不在按钮范围内
	BUTTON_MOUSE_OUT,
	//鼠标在按钮范围内按下
	BUTTON_MOUSE_DOWN,
	//鼠标在按钮范围内按下后释放
	BUTTON_MOUSE_UP,
	//按钮状态共计3个
	BUTTON_STATE_TOTAL = 3,
};

//渲染特定颜色、特定位置的文字
void renderText(SDL_Renderer * ren, TTF_Font* font, SDL_Color Color, SDL_Rect dst, wchar_t * str)
{
	//文字表面转纹理
	SDL_Surface* textSurface = TTF_RenderUNICODE_Blended(font, str, Color);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(ren, textSurface);

	SDL_RenderCopy(ren, textTexture, NULL, &dst);

	SDL_DestroyTexture(textTexture); textTexture = NULL;
	SDL_FreeSurface(textSurface); textSurface = NULL;
}

void drawUI(SDL_Renderer * ren, SDL_Texture * button, SDL_Texture *banner, TTF_Font* font)
{
	//目标矩形
	SDL_Rect dst;

	//黑色
	SDL_Color ColorBlack;
	ColorBlack.r = 0;
	ColorBlack.g = 0;
	ColorBlack.b = 0;
	ColorBlack.a = 255;

	//绘制标题
	dst.x = 0; dst.y = 0;
	dst.w = WINDOW_WIDTH / 6; dst.h = 150;
	SDL_RenderCopy(ren, banner, NULL, &dst);

	//输出策略文字部分
	dst.x = 0; dst.y += dst.h;
	dst.w = WINDOW_WIDTH / 6; dst.h = 50;
	TTF_SetFontStyle(font, TTF_STYLE_UNDERLINE);
	renderText(ren, font, ColorBlack, dst, L"共轨策略控制");

	//输出策略按钮部分
	dst.y += dst.h + BUTTON_HEIGHT;
	for (int i = 0; i < 3; ++i)
	{
		dst.x = (WINDOW_WIDTH / 6 - BUTTON_WIDTH) / 2 * i;
		dst.w = BUTTON_WIDTH; dst.h = BUTTON_HEIGHT;
		SDL_RenderCopy(ren, button, &buttonClip[i][1], &dst);
	}

	//输出火车文字部分
	dst.x = 0; dst.y += BUTTON_HEIGHT * 2;
	dst.w = WINDOW_WIDTH / 6; dst.h = 50;
	TTF_SetFontStyle(font, TTF_STYLE_UNDERLINE);
	renderText(ren, font, ColorBlack, dst, L"火车状态与控制");

	//绘制各火车相应按钮
	TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
	//A文字
	dst.x = WINDOW_WIDTH / 6 / 4; dst.y += 50;
	dst.w = WINDOW_WIDTH / 6 / 2; dst.h = 50;
	renderText(ren, font, ColorBlack, dst, L"火车A");
	//A按钮
	dst.y += dst.h + BUTTON_HEIGHT;
	for (int i = 0; i < 3; ++i)
	{
		dst.x = (WINDOW_WIDTH / 6 - BUTTON_WIDTH) / 2 * i;
		dst.w = BUTTON_WIDTH; dst.h = BUTTON_HEIGHT;
		SDL_RenderCopy(ren, button, &buttonClip[i + 3][1], &dst);
	}


	//B
	dst.x = WINDOW_WIDTH / 6 / 4; dst.y += dst.h + BUTTON_HEIGHT;;
	dst.w = WINDOW_WIDTH / 6 / 2; dst.h = 50;
	renderText(ren, font, ColorBlack, dst, L"火车B");
	dst.y += dst.h + BUTTON_HEIGHT;
	for (int i = 0; i < 3; ++i)
	{
		dst.x = (WINDOW_WIDTH / 6 - BUTTON_WIDTH) / 2 * i;
		dst.w = BUTTON_WIDTH; dst.h = BUTTON_HEIGHT;
		SDL_RenderCopy(ren, button, &buttonClip[i + 3][1], &dst);
	}

	//C
	dst.x = WINDOW_WIDTH / 6 / 4; dst.y += dst.h + BUTTON_HEIGHT;;
	dst.w = WINDOW_WIDTH / 6 / 2; dst.h = 50;
	renderText(ren, font, ColorBlack, dst, L"火车C");
	dst.y += dst.h + BUTTON_HEIGHT;
	for (int i = 0; i < 3; ++i)
	{
		dst.x = (WINDOW_WIDTH / 6 - BUTTON_WIDTH) / 2 * i;
		dst.w = BUTTON_WIDTH; dst.h = BUTTON_HEIGHT;
		SDL_RenderCopy(ren, button, &buttonClip[i + 3][1], &dst);
	}

	//绘制输入命令栏
	dst.x = 0; dst.y += dst.h + BUTTON_HEIGHT;;
	dst.w = WINDOW_WIDTH / 6 ; dst.h = 50;
	renderText(ren, font, ColorBlack, dst, L"命令输入栏");
	dst.y += dst.h;
	renderText(ren, font, ColorBlack, dst, L"__________");

	//绘制退出按钮
	dst.x = (WINDOW_WIDTH / 6 - BUTTON_WIDTH) / 2;
	dst.y = WINDOW_HEIGHT - BUTTON_HEIGHT;
	dst.w = BUTTON_WIDTH; dst.h = BUTTON_HEIGHT;
	SDL_RenderCopy(ren, button, &buttonClip[6][1], &dst);
}


