#include<wchar.h>
#include<SDL.h>
#include<SDL_ttf.h>
#include"drawSDL.h"
#include"var.h"

extern int strategy;
extern int trainNum;
extern struct train train[MAX_TRAIN];

extern SDL_Rect buttonClip[BUTTON_ROW][BUTTON_COLUMN];
extern SDL_Rect strategyButtonPos[3];
extern SDL_Rect trainButtonPos[3][3];
extern SDL_Rect exitButtonPos;

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
		dst.w = BUTTON_WIDTH;
		dst.h = BUTTON_HEIGHT;

		strategyButtonPos[i].x = dst.x;
		strategyButtonPos[i].y = dst.y;
		strategyButtonPos[i].w = dst.w;
		strategyButtonPos[i].h = dst.h;

		//默认为暗色
		SDL_RenderCopy(ren, button, &buttonClip[i][0], &dst);
	}
	switch (strategy)
	{
	case ALTERNATIVE:SDL_RenderCopy(ren, button, &buttonClip[0][2], &strategyButtonPos[0]); break;
	case FAST_FIRST:SDL_RenderCopy(ren, button, &buttonClip[1][2], &strategyButtonPos[1]); break;
	case MANUAL:SDL_RenderCopy(ren, button, &buttonClip[2][2], &strategyButtonPos[2]); break;
	}

	//输出火车文字部分
	dst.x = 0; dst.y += BUTTON_HEIGHT * 2;
	dst.w = WINDOW_WIDTH / 6; dst.h = 50;
	TTF_SetFontStyle(font, TTF_STYLE_UNDERLINE);
	renderText(ren, font, ColorBlack, dst, L"火车状态与控制");

	//绘制各火车相应按钮
	TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
	for (int trainID = 0; trainID < trainNum; ++trainID)
	{
		//文字
		dst.x = WINDOW_WIDTH / 6 / 4; dst.y += 50;
		dst.w = WINDOW_WIDTH / 6 / 2; dst.h = 50;

		wchar_t tempWcstr[10] = L"火车";
		if (trainID == 0) wcscat(tempWcstr, L"A");
		else if (trainID == 1) wcscat(tempWcstr, L"B");
		else if (trainID == 2) wcscat(tempWcstr, L"C");
		renderText(ren, font, ColorBlack, dst, tempWcstr);

		//按钮
		dst.y += dst.h + BUTTON_HEIGHT;
		for (int buttonID = 0; buttonID < 3; ++buttonID)
		{
			dst.x = (WINDOW_WIDTH / 6 - BUTTON_WIDTH) / 2 * buttonID;
			dst.w = BUTTON_WIDTH;
			dst.h = BUTTON_HEIGHT;

			trainButtonPos[trainID][buttonID].x = dst.x;
			trainButtonPos[trainID][buttonID].y = dst.y;
			trainButtonPos[trainID][buttonID].w = dst.w;
			trainButtonPos[trainID][buttonID].h = dst.h;

			SDL_RenderCopy(ren, button, &buttonClip[buttonID + 3][1], &dst);
		}
	}

	//绘制输入命令栏
	dst.x = 0;
	dst.y += dst.h + BUTTON_HEIGHT;
	dst.w = WINDOW_WIDTH / 6;
	dst.h = 50;
	renderText(ren, font, ColorBlack, dst, L"命令输入栏");

	dst.y += dst.h;
	renderText(ren, font, ColorBlack, dst, L"__________");

	//绘制退出按钮
	dst.x = (WINDOW_WIDTH / 6 - BUTTON_WIDTH) / 2;
	dst.y = WINDOW_HEIGHT - BUTTON_HEIGHT;
	dst.w = BUTTON_WIDTH;
	dst.h = BUTTON_HEIGHT;

	exitButtonPos.x = dst.x;
	exitButtonPos.y = dst.y;
	exitButtonPos.w = dst.w;
	exitButtonPos.h = dst.h;

	SDL_RenderCopy(ren, button, &buttonClip[6][1], &dst);
}


