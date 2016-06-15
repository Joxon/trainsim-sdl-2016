#include<stdlib.h>
#include<time.h>
#include<SDL.h>
#include<SDL_image.h>
#include "drawSDL.h"
#include "var.h"

extern struct train train[MAX_TRAIN];
extern struct block railway[MAX_RAIL][MAX_RAIL_LENGTH];

extern SDL_Rect blockClip[BLOCK_ROW][BLOCK_COLUMN];
extern SDL_Rect buttonClip[BUTTON_ROW][BUTTON_COLUMN];

void drawRailway(SDL_Window * win, SDL_Renderer * ren, SDL_Texture * block)
{
	//平铺背景，只希望背景随机生成一次
	static bool backgroundFirstDrawn = false;
	static int backgroundClips[WINDOW_HEIGHT / BLOCK_SIZE][WINDOW_WIDTH / BLOCK_SIZE];
	if (!backgroundFirstDrawn)
	{
		//首次绘制使用随机数
		srand((unsigned)time(NULL));
		for (int y = 0; y < WINDOW_HEIGHT / BLOCK_SIZE; ++y)
			for (int x = 0; x < WINDOW_WIDTH / BLOCK_SIZE; ++x)
			{
				SDL_Rect dst;
				dst.y = y*BLOCK_SIZE;
				dst.x = x*BLOCK_SIZE;
				dst.w = BLOCK_SIZE;
				dst.h = BLOCK_SIZE;
				int randomNum = 10 + rand() % 8;
				backgroundClips[y][x] = randomNum;
				SDL_RenderCopy(ren, block, &blockClip[2][randomNum], &dst);
			}
		backgroundFirstDrawn = true;
	}
	else
	{
		//后续绘制使用已保存的数据
		for (int y = 0; y < WINDOW_HEIGHT / BLOCK_SIZE; ++y)
			for (int x = 0; x < WINDOW_WIDTH / BLOCK_SIZE; ++x)
			{
				SDL_Rect dst;
				dst.y = y*BLOCK_SIZE;
				dst.x = x*BLOCK_SIZE;
				dst.w = BLOCK_SIZE;
				dst.h = BLOCK_SIZE;
				SDL_RenderCopy(ren, block, &blockClip[2][backgroundClips[y][x]], &dst);
			}
	}

	//设定绘制起始点（左上角为零点）
	railway[0][0].x = 10;
	railway[0][0].y = 10;
	if (railway[0][0].common != railway[0][train[0].railwayLength - 1].common)
		drawCrossBlock(0, 0, ren, block);
	else
		drawNormalBlock(0, 0, ren, block);
	//SDL_RenderPresent(ren);//调试可用

	//中央轨道（有两条公共轨道）
	for (int i = 1; i < train[0].railwayLength; i++)
	{
		if (railway[0][i].common != 0 && (railway[0][i - 1].common == 0 || railway[0][i + 1].common == 0))
			drawCrossBlock(0, i, ren, block);
		else
			drawNormalBlock(0, i, ren, block);

		//SDL_RenderPresent(ren);//调试可用
	}
}

//绘制公共入口/出口
static void drawCrossBlock(int j, int i, SDL_Renderer * ren, SDL_Texture * tex)
{
	SDL_Rect pos;
	pos.x = railway[j][i].x * BLOCK_SIZE;
	pos.y = railway[j][i].y * BLOCK_SIZE;
	pos.h = BLOCK_SIZE;
	pos.w = BLOCK_SIZE;
	//较小
	if (railway[j][i - 1].common == 0)
	{
		switch (railway[j][i].direction)
		{
		case WEST:
			SDL_RenderCopy(ren, tex, &blockClip[0][12], &pos);
			railway[j][i + 1].x = railway[j][i].x;
			railway[j][i + 1].y = railway[j][i].y - 1;
			break;
		case NORTH:
			SDL_RenderCopy(ren, tex, &blockClip[0][13], &pos);
			railway[j][i + 1].y = railway[j][i].y;
			railway[j][i + 1].x = railway[j][i].x + 1;
			break;
		case EAST:
			SDL_RenderCopy(ren, tex, &blockClip[0][9], &pos);
			railway[j][i + 1].x = railway[j][i].x;
			railway[j][i + 1].y = railway[j][i].y + 1;
			break;
		case SOUTH:
			SDL_RenderCopy(ren, tex, &blockClip[0][16], &pos);
			railway[j][i + 1].y = railway[j][i].y;
			railway[j][i + 1].x = railway[j][i].x - 1;
			break;
		}
	}
	else//较大
	{
		switch (railway[j][i].direction)
		{
		case WEST:
			SDL_RenderCopy(ren, tex, &blockClip[0][10], &pos);
			railway[j][i + 1].x = railway[j][i].x;
			railway[j][i + 1].y = railway[j][i].y - 1;
			break;
		case NORTH:
			SDL_RenderCopy(ren, tex, &blockClip[0][14], &pos);
			railway[j][i + 1].y = railway[j][i].y;
			railway[j][i + 1].x = railway[j][i].x + 1;
			break;
		case EAST:
			SDL_RenderCopy(ren, tex, &blockClip[0][11], &pos);
			railway[j][i + 1].x = railway[j][i].x;
			railway[j][i + 1].y = railway[j][i].y + 1;
			break;
		case SOUTH:
			SDL_RenderCopy(ren, tex, &blockClip[0][15], &pos);
			railway[j][i + 1].y = railway[j][i].y;
			railway[j][i + 1].x = railway[j][i].x - 1;
			break;
		}
	}
}
//绘制普通轨道
static void drawNormalBlock(int j, int i, SDL_Renderer * ren, SDL_Texture * tex)
{
	SDL_Rect pos;
	pos.x = railway[j][i].x * BLOCK_SIZE;
	pos.y = railway[j][i].y * BLOCK_SIZE;
	pos.h = BLOCK_SIZE;
	pos.w = BLOCK_SIZE;
	switch (railway[j][i].direction)
	{
	case SOUTHWEST:
		SDL_RenderCopy(ren, tex, &blockClip[0][4], &pos);
		railway[j][i + 1].x = railway[j][i].x;
		railway[j][i + 1].y = railway[j][i].y - 1;
		break;
	case WEST:
		SDL_RenderCopy(ren, tex, &blockClip[0][1], &pos);
		railway[j][i + 1].x = railway[j][i].x;
		railway[j][i + 1].y = railway[j][i].y - 1;
		break;
	case NORTHWEST:
		SDL_RenderCopy(ren, tex, &blockClip[0][2], &pos);
		railway[j][i + 1].y = railway[j][i].y;
		railway[j][i + 1].x = railway[j][i].x + 1;
		break;
	case NORTH:
		SDL_RenderCopy(ren, tex, &blockClip[0][0], &pos);
		railway[j][i + 1].y = railway[j][i].y;
		railway[j][i + 1].x = railway[j][i].x + 1;
		break;
	case NORTHEAST:
		SDL_RenderCopy(ren, tex, &blockClip[0][3], &pos);
		railway[j][i + 1].x = railway[j][i].x;
		railway[j][i + 1].y = railway[j][i].y + 1;
		break;
	case EAST:
		SDL_RenderCopy(ren, tex, &blockClip[0][1], &pos);
		railway[j][i + 1].x = railway[j][i].x;
		railway[j][i + 1].y = railway[j][i].y + 1;
		break;
	case SOUTHEAST:
		SDL_RenderCopy(ren, tex, &blockClip[0][5], &pos);
		railway[j][i + 1].y = railway[j][i].y;
		railway[j][i + 1].x = railway[j][i].x - 1;
		break;
	case SOUTH:
		SDL_RenderCopy(ren, tex, &blockClip[0][0], &pos);
		railway[j][i + 1].y = railway[j][i].y;
		railway[j][i + 1].x = railway[j][i].x - 1;
		break;
	}
}
//输入公轨信息，和一个长度为3的int数组，把对应的另一条轨道编号和共轨入口/出口位置赋值给inf数组
//void judge_com(int j, int i,int * inf)
//{
//	int * inf;
//	for (int k = 0; k < 3; k++)
//	{
//		if (j == k)
//			continue;
//		else
//		{
//			for (int l = 0; l < train[k].railwayLength; l++)
//			{
//				if (railway[k][l].common == railway[j][i].common)
//				{
//					inf[0] = k;
//					if (railway[k][l].common == railway[j][i].common && railway[k][l - 1].common == 0)
//						inf[1] = l;
//					if (railway[k][l].common == railway[j][i].common && railway[k][l + 1].common == 0)
//						inf[2] = l;
//				}
//			}
//		}
//	}
//}