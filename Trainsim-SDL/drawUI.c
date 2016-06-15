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

void drawUI(SDL_Renderer * ren, SDL_Texture * button, SDL_Texture *banner, TTF_Font* font)
{
	//目标矩形
	SDL_Rect dst;

	//绘制标题
	dst.x = 0; dst.y = 0;
	dst.w = WINDOW_WIDTH / 6; dst.h = 150;
	SDL_RenderCopy(ren, banner, NULL, &dst);

	//绘制各火车状态和相应按钮
	for (int train = 0; train < trainNum; ++train)
	{

	}
	//绘制输入命令栏
	//绘制退出按钮
}


