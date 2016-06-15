#include<SDL.h>
#include<SDL_image.h>

#include "drawSDL.h"
#include "var.h"

#define WIDTH 60;

int rail_info[MAX_RAIL][5];

extern struct train train[MAX_TRAIN];
extern struct block railway[MAX_RAIL][MAX_RAIL_LENGTH];

extern SDL_Rect blockClip[BLOCK_ROW][BLOCK_COLUMN];
extern SDL_Rect buttonClip[BUTTON_ROW][BUTTON_COLUMN];

void drawRailway(SDL_Window * win, SDL_Renderer * ren, SDL_Texture * block)
{

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
	
	for (int k = 1; k < 3; k++)
	{
		for (int i = 0; i < train[0].railwayLength; i++)
		{
			if (i == rail_info[k - 1][3])
			{
				railway[k][rail_info[k - 1][2]].x = railway[0][rail_info[k - 1][3]].x;
				railway[k][rail_info[k - 1][2]].y = railway[0][rail_info[k - 1][3]].y;
			}
			if (i == rail_info[k - 1][4])
			{
				railway[k][rail_info[k - 1][1]].x = railway[0][rail_info[k - 1][4]].x;
				railway[k][rail_info[k - 1][1]].y = railway[0][rail_info[k - 1][4]].y;
			}
		
		}
	}

	for (int j = 1; j < 3; j++)
		for (int i = rail_info[j-1][2]; i != rail_info[j-1][1]; i++)
		{
			if (railway[j][i].common == 0)
				drawNormalBlock(j, i, ren, block);
			else
				if (i = rail_info[j - 1][2])
				{
					if (rail_info[j - 1][2] != train[j].railwayLength - 1)
					{
						switch (railway[j][i].direction)
						{
						case SOUTHWEST:
							railway[j][i + 1].x = railway[j][i].x;
							railway[j][i + 1].y = railway[j][i].y - 1;
							break;
						case WEST:
							railway[j][i + 1].x = railway[j][i].x;
							railway[j][i + 1].y = railway[j][i].y - 1;
							break;
						case NORTHWEST:
							railway[j][i + 1].y = railway[j][i].y;
							railway[j][i + 1].x = railway[j][i].x + 1;
							break;
						case NORTH:
							railway[j][i + 1].y = railway[j][i].y;
							railway[j][i + 1].x = railway[j][i].x + 1;
							break;
						case NORTHEAST:
							railway[j][i + 1].x = railway[j][i].x;
							railway[j][i + 1].y = railway[j][i].y + 1;
							break;
						case EAST:
							railway[j][i + 1].x = railway[j][i].x;
							railway[j][i + 1].y = railway[j][i].y + 1;
							break;
						case SOUTHEAST:
							railway[j][i + 1].y = railway[j][i].y;
							railway[j][i + 1].x = railway[j][i].x - 1;
							break;
						case SOUTH:
							railway[j][i + 1].y = railway[j][i].y;
							railway[j][i + 1].x = railway[j][i].x - 1;
							break;
						}
					}
					else
					{
						switch (railway[j][i].direction)
						{
						case SOUTHWEST:
							railway[j][0].x = railway[j][i].x;
							railway[j][0].y = railway[j][i].y - 1;
							break;
						case WEST:
							railway[j][0].x = railway[j][i].x;
							railway[j][0].y = railway[j][i].y - 1;
							break;
						case NORTHWEST:
							railway[j][0].y = railway[j][i].y;
							railway[j][0].x = railway[j][i].x + 1;
							break;
						case NORTH:
							railway[j][0].y = railway[j][i].y;
							railway[j][0].x = railway[j][i].x + 1;
							break;
						case NORTHEAST:
							railway[j][0].x = railway[j][i].x;
							railway[j][0].y = railway[j][i].y + 1;
							break;
						case EAST:
							railway[j][0].x = railway[j][i].x;
							railway[j][0].y = railway[j][i].y + 1;
							break;
						case SOUTHEAST:
							railway[j][0].y = railway[j][i].y;
							railway[j][0].x = railway[j][i].x - 1;
							break;
						case SOUTH:
							railway[j][0].y = railway[j][i].y;
							railway[j][0].x = railway[j][i].x - 1;
							break;
						}
					}
				}
			if (i == train[j].railwayLength - 1)
				i = -1;
			//SDL_RenderPresent(ren);//调试可用
		}
}
//绘制公共入口/出口
static void drawCrossBlock(int j, int i, SDL_Renderer * ren, SDL_Texture * tex)
{
	SDL_Rect pos;
	pos.x = railway[j][i].x * WIDTH;
	pos.y = railway[j][i].y * WIDTH;
	pos.h = WIDTH;
	pos.w = WIDTH;
	//入口
	if ((i != 0 && railway[j][i - 1].common == 0)||(i == 0 && railway[j][train[j].railwayLength - 1].common == 0))
	{
		judge_com(j, i);
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
		for (int k = 1; k < 3; k++)
		{
			if (rail_info[k-1][3] == i)
			{
				railway[k-1][rail_info[k-1][2]].x = railway[j][i].x;
				railway[k-1][rail_info[k-1][2]].y = railway[j][i].y;
			}
			if (rail_info[k - 1][4] == i)
			{
				railway[k - 1][rail_info[k - 1][1]].x = railway[j][i].x;
				railway[k - 1][rail_info[k - 1][1]].y = railway[j][i].y;
			}
		}
	}
	else//出口
	{
		judge_com(j, i);
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
	pos.x = railway[j][i].x * WIDTH;
	pos.y = railway[j][i].y * WIDTH;
	pos.h = WIDTH;
	pos.w = WIDTH;
	if (i != train[j].railwayLength - 1)
	{
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
	else
	{
		switch (railway[j][i].direction)
		{
		case SOUTHWEST:
			SDL_RenderCopy(ren, tex, &blockClip[0][4], &pos);
			railway[j][0].x = railway[j][i].x;
			railway[j][0].y = railway[j][i].y - 1;
			break;
		case WEST:
			SDL_RenderCopy(ren, tex, &blockClip[0][1], &pos);
			railway[j][0].x = railway[j][i].x;
			railway[j][0].y = railway[j][i].y - 1;
			break;
		case NORTHWEST:
			SDL_RenderCopy(ren, tex, &blockClip[0][2], &pos);
			railway[j][0].y = railway[j][i].y;
			railway[j][0].x = railway[j][i].x + 1;
			break;
		case NORTH:
			SDL_RenderCopy(ren, tex, &blockClip[0][0], &pos);
			railway[j][0].y = railway[j][i].y;
			railway[j][0].x = railway[j][i].x + 1;
			break;
		case NORTHEAST:
			SDL_RenderCopy(ren, tex, &blockClip[0][3], &pos);
			railway[j][0].x = railway[j][i].x;
			railway[j][0].y = railway[j][i].y + 1;
			break;
		case EAST:
			SDL_RenderCopy(ren, tex, &blockClip[0][1], &pos);
			railway[j][0].x = railway[j][i].x;
			railway[j][0].y = railway[j][i].y + 1;
			break;
		case SOUTHEAST:
			SDL_RenderCopy(ren, tex, &blockClip[0][5], &pos);
			railway[j][0].y = railway[j][i].y;
			railway[j][0].x = railway[j][i].x - 1;
			break;
		case SOUTH:
			SDL_RenderCopy(ren, tex, &blockClip[0][0], &pos);
			railway[j][0].y = railway[j][i].y;
			railway[j][0].x = railway[j][i].x - 1;
			break;
		}
	}
}
//输入公轨信息，和一个长度为3的int数组，把对应的另一条轨道编号和共轨入口/出口位置赋值给inf数组
void judge_com(int j, int i)
{
	int id;
	for (int k = 1; k < 3 /*暂时为3*/ ; k++)


		for (int l = 0; l < train[k].railwayLength; l++)
		{
			if (railway[k][l].common == railway[j][i].common && railway[k][l - 1].common == 0 && railway[j][i - 1].common == 0)//入口
			{
				id = k;
				rail_info[id - 1][0] = id;
				rail_info[id-1][1] = l;
				rail_info[id-1][3] = i;
				break;
			}
			if (railway[k][l].common == railway[j][i].common && railway[k][l + 1].common == 0 && railway[j][i + 1].common == 0)//出口
			{
				id = k;
				rail_info[id - 1][0] = id;
				rail_info[id-1][2] = l;
				rail_info[id-1][4] = i;
				break;
			}
		}
}