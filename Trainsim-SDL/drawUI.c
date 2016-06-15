#include<SDL.h>
#include<SDL_ttf.h>
#include"drawSDL.h"
#include"var.h"

extern int trainNum;
extern struct train train[MAX_TRAIN];
extern SDL_Rect buttonClip[BUTTON_ROW][BUTTON_COLUMN];

//��ť״̬ö��
enum buttonState
{
	//��겻�ڰ�ť��Χ��
	BUTTON_MOUSE_OUT,
	//����ڰ�ť��Χ�ڰ���
	BUTTON_MOUSE_DOWN,
	//����ڰ�ť��Χ�ڰ��º��ͷ�
	BUTTON_MOUSE_UP,
	//��ť״̬����3��
	BUTTON_STATE_TOTAL = 3,
};

//��Ⱦ�ض���ɫ���ض�λ�õ�����
void renderText(SDL_Renderer * ren, TTF_Font* font, SDL_Color Color, SDL_Rect dst, wchar_t * str)
{
	//���ֱ���ת����
	SDL_Surface* textSurface = TTF_RenderUNICODE_Blended(font, str, Color);
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
	dst.w = WINDOW_WIDTH / 6; dst.h = 150;
	SDL_RenderCopy(ren, banner, NULL, &dst);

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
		dst.w = BUTTON_WIDTH; dst.h = BUTTON_HEIGHT;
		SDL_RenderCopy(ren, button, &buttonClip[i][1], &dst);
	}

	//��������ֲ���
	dst.x = 0; dst.y += BUTTON_HEIGHT * 2;
	dst.w = WINDOW_WIDTH / 6; dst.h = 50;
	TTF_SetFontStyle(font, TTF_STYLE_UNDERLINE);
	renderText(ren, font, ColorBlack, dst, L"��״̬�����");

	//���Ƹ�����Ӧ��ť
	TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
	//A����
	dst.x = WINDOW_WIDTH / 6 / 4; dst.y += 50;
	dst.w = WINDOW_WIDTH / 6 / 2; dst.h = 50;
	renderText(ren, font, ColorBlack, dst, L"��A");
	//A��ť
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
	renderText(ren, font, ColorBlack, dst, L"��B");
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
	renderText(ren, font, ColorBlack, dst, L"��C");
	dst.y += dst.h + BUTTON_HEIGHT;
	for (int i = 0; i < 3; ++i)
	{
		dst.x = (WINDOW_WIDTH / 6 - BUTTON_WIDTH) / 2 * i;
		dst.w = BUTTON_WIDTH; dst.h = BUTTON_HEIGHT;
		SDL_RenderCopy(ren, button, &buttonClip[i + 3][1], &dst);
	}

	//��������������
	dst.x = 0; dst.y += dst.h + BUTTON_HEIGHT;;
	dst.w = WINDOW_WIDTH / 6 ; dst.h = 50;
	renderText(ren, font, ColorBlack, dst, L"����������");
	dst.y += dst.h;
	renderText(ren, font, ColorBlack, dst, L"__________");

	//�����˳���ť
	dst.x = (WINDOW_WIDTH / 6 - BUTTON_WIDTH) / 2;
	dst.y = WINDOW_HEIGHT - BUTTON_HEIGHT;
	dst.w = BUTTON_WIDTH; dst.h = BUTTON_HEIGHT;
	SDL_RenderCopy(ren, button, &buttonClip[6][1], &dst);
}


