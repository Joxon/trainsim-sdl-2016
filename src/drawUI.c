#include<wchar.h>
#include<SDL.h>
#include<SDL_ttf.h>
#include"drawSDL.h"
#include"var.h"

extern int strategy;
extern int trainNum;
extern struct train trains[MAX_TRAIN];
extern int processTime; 

extern SDL_Rect buttonClip[BUTTON_ROW][BUTTON_COLUMN];
extern SDL_Rect strategyButtonPos[3];
extern SDL_Rect trainButtonPos[3][3];
extern SDL_Rect exitButtonPos;

//��Ⱦ�ض���ɫ���ض�λ�õ�����
void renderText(SDL_Renderer * ren, TTF_Font* font, SDL_Color color, SDL_Rect dst, wchar_t * str)
{
	//���ֱ���ת����
	SDL_Surface* textSurface = TTF_RenderUNICODE_Blended(font, str, color);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(ren, textSurface);

	SDL_RenderCopy(ren, textTexture, NULL, &dst);

	SDL_DestroyTexture(textTexture); textTexture = NULL;
	SDL_FreeSurface(textSurface); textSurface = NULL;
}

void drawUI(SDL_Renderer * ren, SDL_Texture * button, SDL_Texture *banner, TTF_Font* font)
{
	//Ŀ�����
	SDL_Rect dst;

	//��ɫ
	SDL_Color ColorBlack;
	ColorBlack.r = 0;
	ColorBlack.g = 0;
	ColorBlack.b = 0;
	ColorBlack.a = 255;

	//���Ʊ���
	dst.x = 0; dst.y = 0;
	dst.w = USERVIEW_WIDTH; dst.h = 150;
	SDL_RenderCopy(ren, banner, NULL, &dst);

	//�������ʱ��
	dst.y += dst.h;
	dst.h = 50;
	wchar_t tempWcstr[50];
	swprintf(tempWcstr, 50, L"����ʱ�䣺%d",processTime);
	renderText(ren, font, ColorBlack, dst, tempWcstr);

	//����������ֲ���
	dst.x = 0; dst.y += dst.h;
	dst.w = WINDOW_WIDTH / 6; dst.h = 50;
	TTF_SetFontStyle(font, TTF_STYLE_UNDERLINE);
	renderText(ren, font, ColorBlack, dst, L"������Կ���");

	//������԰�ť����
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

		//Ĭ��Ϊ��ɫ
		SDL_RenderCopy(ren, button, &buttonClip[i][0], &dst);
	}
	switch (strategy)
	{
	case ALTERNATIVE:SDL_RenderCopy(ren, button, &buttonClip[0][2], &strategyButtonPos[0]); break;
	case FAST_FIRST:SDL_RenderCopy(ren, button, &buttonClip[1][2], &strategyButtonPos[1]); break;
	case MANUAL:SDL_RenderCopy(ren, button, &buttonClip[2][2], &strategyButtonPos[2]); break;
	}

	//��������ֲ���
	dst.x = 0; dst.y += BUTTON_HEIGHT * 2;
	dst.w = WINDOW_WIDTH / 6; dst.h = 50;
	TTF_SetFontStyle(font, TTF_STYLE_UNDERLINE);
	renderText(ren, font, ColorBlack, dst, L"��״̬�����");

	//���Ƹ�����Ӧ��ť
	TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
	for (int trainID = 0; trainID < trainNum; ++trainID)
	{
		//����
		dst.x = USERVIEW_WIDTH / 4; dst.y += 50;
		dst.w = USERVIEW_WIDTH / 2; dst.h = 50;
		swprintf(tempWcstr, 50, L"��%c", 'A' + trainID);
		renderText(ren, font, ColorBlack, dst, tempWcstr);

		//״̬
		dst.y += dst.h;
		dst.x = 0;
		dst.w = USERVIEW_WIDTH;
		swprintf(tempWcstr, 50, L"״̬��%s",
			trains[trainID].status == RUN ? L"������" :
			trains[trainID].status == PAUSE_COMMON ? L"��ͣ-�ȴ��������" :
			trains[trainID].status == PAUSE_STATION ? L"��ͣ-ͣ��վ��" :
			trains[trainID].status == PAUSE_ANY ? L" ��ͣ-�˹���ͣ" : L"����");
		renderText(ren, font, ColorBlack, dst, tempWcstr);

		//��ť
		dst.y += dst.h;
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

	////��������������
	//dst.x = 0;
	//dst.y += dst.h + BUTTON_HEIGHT;
	//dst.w = WINDOW_WIDTH / 6;
	//dst.h = 50;
	//renderText(ren, font, ColorBlack, dst, L"����������");

	//dst.y += dst.h;
	//renderText(ren, font, ColorBlack, dst, L"__________");

	//�����˳���ť
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


