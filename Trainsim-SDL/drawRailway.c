#include<SDL.h>
#include<SDL_image.h>
#include<stdexcept>
#include<string>
#include "drawSDL.h"
#include"var.h"

#define WIDTH 30;

extern struct train train[MAX_TRAIN];
extern struct block railway[MAX_RAIL][MAX_RAIL_LENGTH];
extern SDL_Rect clip[BLOCK_ROW][BLOCK_COLUMN];

void drawRailway(SDL_Window * win,SDL_Renderer * ren)
{
	SDL_Texture * tex = NULL;
	int x, y;
	int inf[3];
	SDL_Rect rail;
	tex = IMG_LoadTexture(ren, "../leb/ pic.jpg");
	rail.h = WIDTH;
	rail.w = WIDTH;

	railway[0][0].x = 15 * WIDTH;
	railway[0][0].y = 10 * WIDTH;
	//中央轨道（有两条公共轨道）
	for (int i = 0; i < train[0].railwayLength; i++)
	{
		if (i == 0)
		{
			if (railway[0][i].common == railway[0][train[i].railwayLength - 1].common)
			{
				crossblock(0, i, ren, tex);
			}
			else
			{
				ablock(0, i, ren, tex);
			}
		}
		else
		{
			if (railway[0][i].common != 0 && (railway[0][i - 1].common==0 || railway[0][i + 1].common == 0) )
			{
				judge_com(0, i, inf);
				crossblock(0, i, inf, ren, tex);
			}
			else
			{
				ablock(0, i, ren, tex);
			}
		}

	}


}
//公共入口/出口
void crossblock(int j, int i,int * inf , SDL_Renderer * ren, SDL_Texture * tex)
{
	SDL_Rect pos;
	pos.x = railway[j][i].x * WIDTH;
	pos.y = railway[j][i].y * WIDTH;
	pos.h = pos.w = WIDTH;
	if (railway[j][i - 1].common = 0)//较小
	{
		switch (railway[j][i].direction)
		{
		case WEST:
			SDL_RenderCopy(ren, tex, &clip[12][0], &pos);
			railway[j][i + 1].y = railway[j][i].y - 1;
			break;
		case NORTH:
			SDL_RenderCopy(ren, tex, &clip[13][0], &pos);
			railway[j][i + 1].x = railway[j][i].x + 1;
			break;
		case EAST:
			SDL_RenderCopy(ren, tex, &clip[9][0], &pos);
			railway[j][i + 1].y = railway[j][i].y + 1;
			break;
		case SOUTH:
			SDL_RenderCopy(ren, tex, &clip[16][0], &pos);
			railway[j][i + 1].x = railway[j][i].x - 1;
			break;
		}
	}
	else//较大
	{
		switch (railway[j][i].direction)
		{
		case WEST:
			SDL_RenderCopy(ren, tex, &clip[10][0], &pos);
			railway[j][i + 1].y = railway[j][i].y - 1;
			break;
		case NORTH:
			SDL_RenderCopy(ren, tex, &clip[14][0], &pos);
			railway[j][i + 1].x = railway[j][i].x + 1;
			break;
		case EAST:
			SDL_RenderCopy(ren, tex, &clip[11][0], &pos);
			railway[j][i + 1].y = railway[j][i].y + 1;
			break;
		case SOUTH:
			SDL_RenderCopy(ren, tex, &clip[15][0], &pos);
			railway[j][i + 1].x = railway[j][i].x - 1;
			break;
		}
	}
}
//普通轨道
void ablock(int j, int i, SDL_Renderer * ren, SDL_Texture * tex)
{
	SDL_Rect pos;
	pos.x = railway[j][i].x * WIDTH;
	pos.y = railway[j][i].y * WIDTH;
	pos.h = pos.w = WIDTH;
	switch(railway[j][i].direction)
	{
	case SOUTHWEST:
		SDL_RenderCopy(ren, tex, &clip[4][0], &pos);
		railway[j][i + 1].y = railway[j][i].y - 1;
		break;
	case WEST:
		SDL_RenderCopy(ren, tex, &clip[1][0], &pos);
		railway[j][i + 1].y = railway[j][i].y - 1;
		break;
	case NORTHWEST:
		SDL_RenderCopy(ren, tex, &clip[2][0], &pos);
		railway[j][i + 1].x = railway[j][i].x + 1;
		break;
	case NORTH:
		SDL_RenderCopy(ren, tex, &clip[0][0], &pos);
		railway[j][i + 1].x = railway[j][i].x + 1;
		break;
	case NORTHEAST:
		SDL_RenderCopy(ren, tex, &clip[3][0], &pos);
		railway[j][i + 1].y = railway[j][i].y + 1;
		break;
	case EAST:
		SDL_RenderCopy(ren, tex, &clip[1][0], &pos);
		railway[j][i + 1].y = railway[j][i].y + 1;
		break;
	case SOUTHEAST:
		SDL_RenderCopy(ren, tex, &clip[5][0], &pos);
		railway[j][i + 1].x = railway[j][i].x - 1;
		break;
	case SOUTH:
		SDL_RenderCopy(ren, tex, &clip[0][0], &pos);
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