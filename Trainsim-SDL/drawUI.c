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

void drawUI(SDL_Renderer * ren, SDL_Texture * button, SDL_Texture *banner, TTF_Font* font)
{
	//Ŀ�����
	SDL_Rect dst;

	//���Ʊ���
	dst.x = 0; dst.y = 0;
	dst.w = WINDOW_WIDTH / 6; dst.h = 150;
	SDL_RenderCopy(ren, banner, NULL, &dst);

	//���Ƹ���״̬����Ӧ��ť
	for (int train = 0; train < trainNum; ++train)
	{

	}
	//��������������
	//�����˳���ť
}


