#include<stdlib.h>
#include<time.h>

#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>

#include "drawSDL.h"
#include "var.h"

extern int railNum;

extern struct train train[MAX_TRAIN];
extern struct block railway[MAX_RAIL][MAX_RAIL_LENGTH];

extern SDL_Rect blockClip[BLOCK_ROW][BLOCK_COLUMN];
extern SDL_Rect buttonClip[BUTTON_ROW][BUTTON_COLUMN];

//������Ϣ��0Ϊid��12Ϊ����ʼĩ��34Ϊ����ʼĩ
int commonInfo[MAX_RAIL][5];

void drawRailway(SDL_Renderer * ren, SDL_Texture * block, TTF_Font* font)
{
	//ƽ�̱�����ֻϣ�������������һ��
	static bool backgroundFirstDrawn = false;
	static int backgroundClips[WINDOW_HEIGHT / BLOCK_SIZE][WINDOW_WIDTH / BLOCK_SIZE];
	if (!backgroundFirstDrawn)
	{
		//�״λ���ʹ�������
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
		//��������ʹ���ѱ��������
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

	//�״λ��ƹ���ı��
	static bool railwayFirstDrawn = false;
	if (!railwayFirstDrawn)
	{
		//�趨������ʼ�㣨���Ͻ�Ϊ��㣩
		railway[0][0].x = WINDOW_WIDTH / BLOCK_SIZE / 2;
		railway[0][0].y = WINDOW_HEIGHT / BLOCK_SIZE / 2;
	}

	if (railway[0][0].common != railway[0][train[0].railwayLength - 1].common)
		drawCrossBlock(0, 0, ren, block);
	else
		drawNormalBlock(0, 0, ren, block);
#ifdef DEBUG
	SDL_RenderPresent(ren);
#endif // DEBUG

	//��ʼ����Ч
	if (!railwayFirstDrawn)
	{
		SDL_Color foreground;
		foreground.a = 255;
		foreground.b = 0;
		foreground.g = 0;
		foreground.r = 0;
		SDL_Color background;
		background.a = 255;
		background.b = 182;
		background.g = 227;
		background.r = 239;
		SDL_Rect dst;
		dst.x = 0;
		dst.y = 0;
		dst.w = 300;
		dst.h = 75;
		SDL_Surface* textSurface = TTF_RenderUNICODE_Shaded(font, L"��ʼ����...", foreground, background);
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(ren, textSurface);
		SDL_RenderCopy(ren, textTexture, NULL, &dst);

		SDL_DestroyTexture(textTexture); textTexture = NULL;
		SDL_FreeSurface(textSurface); textSurface = NULL;
	}

	//���������������������������
	for (int blockID = 1; blockID < train[0].railwayLength; blockID++)
	{
		if (railway[0][blockID].common != 0 && (railway[0][blockID - 1].common == 0 || railway[0][blockID + 1].common == 0))
			drawCrossBlock(0, blockID, ren, block);
		else
			drawNormalBlock(0, blockID, ren, block);

		if (!railwayFirstDrawn)
		{
			SDL_RenderPresent(ren);
			SDL_Delay(1000 / 24);
		}

#ifdef DEBUG
		SDL_RenderPresent(ren);
#endif // DEBUG
	}

	//�����������
	//��ȡ������Ϣ
	for (int railID = 1; railID < railNum; railID++)
	{
		for (int blockID = 0; blockID < train[0].railwayLength; blockID++)
		{
			if (blockID == commonInfo[railID - 1][3])
			{
				railway[railID][commonInfo[railID - 1][2]].x = railway[0][commonInfo[railID - 1][3]].x;
				railway[railID][commonInfo[railID - 1][2]].y = railway[0][commonInfo[railID - 1][3]].y;
			}
			if (blockID == commonInfo[railID - 1][4])
			{
				railway[railID][commonInfo[railID - 1][1]].x = railway[0][commonInfo[railID - 1][4]].x;
				railway[railID][commonInfo[railID - 1][1]].y = railway[0][commonInfo[railID - 1][4]].y;
			}
		}
	}
	//��ʼ����
	for (int railID = 1; railID < railNum; railID++)
		for (int blockID = commonInfo[railID - 1][2]; blockID != commonInfo[railID - 1][1]; blockID++)
		{
			if (railway[railID][blockID].common == 0)
				drawNormalBlock(railID, blockID, ren, block);
			else
				if (blockID == commonInfo[railID - 1][2])
				{
					if (commonInfo[railID - 1][2] != train[railID].railwayLength - 1)
					{
						switch (railway[railID][blockID].direction)
						{
						case SOUTHWEST:
							railway[railID][blockID + 1].x = railway[railID][blockID].x;
							railway[railID][blockID + 1].y = railway[railID][blockID].y - 1;
							break;
						case WEST:
							railway[railID][blockID + 1].x = railway[railID][blockID].x;
							railway[railID][blockID + 1].y = railway[railID][blockID].y - 1;
							break;
						case NORTHWEST:
							railway[railID][blockID + 1].y = railway[railID][blockID].y;
							railway[railID][blockID + 1].x = railway[railID][blockID].x + 1;
							break;
						case NORTH:
							railway[railID][blockID + 1].y = railway[railID][blockID].y;
							railway[railID][blockID + 1].x = railway[railID][blockID].x + 1;
							break;
						case NORTHEAST:
							railway[railID][blockID + 1].x = railway[railID][blockID].x;
							railway[railID][blockID + 1].y = railway[railID][blockID].y + 1;
							break;
						case EAST:
							railway[railID][blockID + 1].x = railway[railID][blockID].x;
							railway[railID][blockID + 1].y = railway[railID][blockID].y + 1;
							break;
						case SOUTHEAST:
							railway[railID][blockID + 1].y = railway[railID][blockID].y;
							railway[railID][blockID + 1].x = railway[railID][blockID].x - 1;
							break;
						case SOUTH:
							railway[railID][blockID + 1].y = railway[railID][blockID].y;
							railway[railID][blockID + 1].x = railway[railID][blockID].x - 1;
							break;
						}
					}
					else
					{
						switch (railway[railID][blockID].direction)
						{
						case SOUTHWEST:
							railway[railID][0].x = railway[railID][blockID].x;
							railway[railID][0].y = railway[railID][blockID].y - 1;
							break;
						case WEST:
							railway[railID][0].x = railway[railID][blockID].x;
							railway[railID][0].y = railway[railID][blockID].y - 1;
							break;
						case NORTHWEST:
							railway[railID][0].y = railway[railID][blockID].y;
							railway[railID][0].x = railway[railID][blockID].x + 1;
							break;
						case NORTH:
							railway[railID][0].y = railway[railID][blockID].y;
							railway[railID][0].x = railway[railID][blockID].x + 1;
							break;
						case NORTHEAST:
							railway[railID][0].x = railway[railID][blockID].x;
							railway[railID][0].y = railway[railID][blockID].y + 1;
							break;
						case EAST:
							railway[railID][0].x = railway[railID][blockID].x;
							railway[railID][0].y = railway[railID][blockID].y + 1;
							break;
						case SOUTHEAST:
							railway[railID][0].y = railway[railID][blockID].y;
							railway[railID][0].x = railway[railID][blockID].x - 1;
							break;
						case SOUTH:
							railway[railID][0].y = railway[railID][blockID].y;
							railway[railID][0].x = railway[railID][blockID].x - 1;
							break;
						}
					}
				}
			if (blockID == train[railID].railwayLength - 1)
				blockID = -1;

			if (!railwayFirstDrawn)
			{
				//��ʼ����Ч
				SDL_RenderPresent(ren);
				SDL_Delay(1000 / 24);
			}

#ifdef DEBUG
			SDL_RenderPresent(ren);
#endif // DEBUG
		}

	//�����û����Ч��
	railwayFirstDrawn = true;

	//������
	for (int subRailID = 1; subRailID < railNum; subRailID++)
		for (int mainBlockID = commonInfo[subRailID - 1][3] + 1,
			subBlockID = commonInfo[subRailID - 1][2] - 1;
			mainBlockID < commonInfo[subRailID - 1][4];
			mainBlockID++, subBlockID--)
		{
			if (mainBlockID >= train[0].railwayLength)
				mainBlockID = 0;
			if (mainBlockID < 0)
				mainBlockID = train[0].railwayLength - 1;
			if (subBlockID >= train[subRailID].railwayLength)
				subBlockID = 0;
			if (subBlockID < 0)
				subBlockID = train[subRailID].railwayLength - 1;
			railway[subRailID][subBlockID] = railway[0][mainBlockID];
		}

	//��ʾ�û����Ե����ͼ
	if (railwayFirstDrawn)
	{
		SDL_Color foreground;
		foreground.a = 255;
		foreground.b = 0;
		foreground.g = 0;
		foreground.r = 255;
		SDL_Color background;
		background.a = 255;
		background.b = 182;
		background.g = 227;
		background.r = 239;
		SDL_Rect dst;
		dst.x = 0;
		dst.y = 0;
		dst.w = 500;
		dst.h = 75;
		SDL_Surface* textSurface = TTF_RenderUNICODE_Shaded(font, L"�����ͼ���޸Ĺ��λ�ã�", foreground, background);
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(ren, textSurface);
		SDL_RenderCopy(ren, textTexture, NULL, &dst);

		SDL_DestroyTexture(textTexture); textTexture = NULL;
		SDL_FreeSurface(textSurface); textSurface = NULL;
	}


}

//���ƹ������/����
static void drawCrossBlock(int railID, int blockID, SDL_Renderer * ren, SDL_Texture * tex)
{
	SDL_Rect pos;
	pos.x = railway[railID][blockID].x * BLOCK_SIZE;
	pos.y = railway[railID][blockID].y * BLOCK_SIZE;
	pos.h = BLOCK_SIZE;
	pos.w = BLOCK_SIZE;

	//���
	if ((blockID != 0 && railway[railID][blockID - 1].common == 0) || (blockID == 0 && railway[railID][train[railID].railwayLength - 1].common == 0))
	{
		judgeCommon(railID, blockID);
		switch (railway[railID][blockID].direction)
		{
		case WEST:
			SDL_RenderCopy(ren, tex, &blockClip[0][12], &pos);
			railway[railID][blockID + 1].x = railway[railID][blockID].x;
			railway[railID][blockID + 1].y = railway[railID][blockID].y - 1;
			break;
		case NORTH:
			SDL_RenderCopy(ren, tex, &blockClip[0][13], &pos);
			railway[railID][blockID + 1].y = railway[railID][blockID].y;
			railway[railID][blockID + 1].x = railway[railID][blockID].x + 1;
			break;
		case EAST:
			SDL_RenderCopy(ren, tex, &blockClip[0][9], &pos);
			railway[railID][blockID + 1].x = railway[railID][blockID].x;
			railway[railID][blockID + 1].y = railway[railID][blockID].y + 1;
			break;
		case SOUTH:
			SDL_RenderCopy(ren, tex, &blockClip[0][16], &pos);
			railway[railID][blockID + 1].y = railway[railID][blockID].y;
			railway[railID][blockID + 1].x = railway[railID][blockID].x - 1;
			break;
		}
		for (int k = 1; k < 3; k++)
		{
			if (commonInfo[k - 1][3] == blockID)
			{
				railway[k - 1][commonInfo[k - 1][2]].x = railway[railID][blockID].x;
				railway[k - 1][commonInfo[k - 1][2]].y = railway[railID][blockID].y;
			}
			if (commonInfo[k - 1][4] == blockID)
			{
				railway[k - 1][commonInfo[k - 1][1]].x = railway[railID][blockID].x;
				railway[k - 1][commonInfo[k - 1][1]].y = railway[railID][blockID].y;
			}
		}
	}
	else//����
	{
		judgeCommon(railID, blockID);
		switch (railway[railID][blockID].direction)
		{
		case WEST:
			SDL_RenderCopy(ren, tex, &blockClip[0][10], &pos);
			railway[railID][blockID + 1].x = railway[railID][blockID].x;
			railway[railID][blockID + 1].y = railway[railID][blockID].y - 1;
			break;
		case NORTH:
			SDL_RenderCopy(ren, tex, &blockClip[0][14], &pos);
			railway[railID][blockID + 1].y = railway[railID][blockID].y;
			railway[railID][blockID + 1].x = railway[railID][blockID].x + 1;
			break;
		case EAST:
			SDL_RenderCopy(ren, tex, &blockClip[0][11], &pos);
			railway[railID][blockID + 1].x = railway[railID][blockID].x;
			railway[railID][blockID + 1].y = railway[railID][blockID].y + 1;
			break;
		case SOUTH:
			SDL_RenderCopy(ren, tex, &blockClip[0][15], &pos);
			railway[railID][blockID + 1].y = railway[railID][blockID].y;
			railway[railID][blockID + 1].x = railway[railID][blockID].x - 1;
			break;
		}

	}
}

//������ͨ���
static void drawNormalBlock(int railID, int blockID, SDL_Renderer * ren, SDL_Texture * tex)
{
	SDL_Rect pos;
	pos.x = railway[railID][blockID].x * BLOCK_SIZE;
	pos.y = railway[railID][blockID].y * BLOCK_SIZE;
	pos.h = BLOCK_SIZE;
	pos.w = BLOCK_SIZE;
	if (blockID != train[railID].railwayLength - 1)
	{
		switch (railway[railID][blockID].direction)
		{
		case SOUTHWEST:
			SDL_RenderCopy(ren, tex, &blockClip[0][4], &pos);
			railway[railID][blockID + 1].x = railway[railID][blockID].x;
			railway[railID][blockID + 1].y = railway[railID][blockID].y - 1;
			break;
		case WEST:
			SDL_RenderCopy(ren, tex, &blockClip[0][1], &pos);
			railway[railID][blockID + 1].x = railway[railID][blockID].x;
			railway[railID][blockID + 1].y = railway[railID][blockID].y - 1;
			break;
		case NORTHWEST:
			SDL_RenderCopy(ren, tex, &blockClip[0][2], &pos);
			railway[railID][blockID + 1].y = railway[railID][blockID].y;
			railway[railID][blockID + 1].x = railway[railID][blockID].x + 1;
			break;
		case NORTH:
			SDL_RenderCopy(ren, tex, &blockClip[0][0], &pos);
			railway[railID][blockID + 1].y = railway[railID][blockID].y;
			railway[railID][blockID + 1].x = railway[railID][blockID].x + 1;
			break;
		case NORTHEAST:
			SDL_RenderCopy(ren, tex, &blockClip[0][3], &pos);
			railway[railID][blockID + 1].x = railway[railID][blockID].x;
			railway[railID][blockID + 1].y = railway[railID][blockID].y + 1;
			break;
		case EAST:
			SDL_RenderCopy(ren, tex, &blockClip[0][1], &pos);
			railway[railID][blockID + 1].x = railway[railID][blockID].x;
			railway[railID][blockID + 1].y = railway[railID][blockID].y + 1;
			break;
		case SOUTHEAST:
			SDL_RenderCopy(ren, tex, &blockClip[0][5], &pos);
			railway[railID][blockID + 1].y = railway[railID][blockID].y;
			railway[railID][blockID + 1].x = railway[railID][blockID].x - 1;
			break;
		case SOUTH:
			SDL_RenderCopy(ren, tex, &blockClip[0][0], &pos);
			railway[railID][blockID + 1].y = railway[railID][blockID].y;
			railway[railID][blockID + 1].x = railway[railID][blockID].x - 1;
			break;
		}
	}
	else
	{
		switch (railway[railID][blockID].direction)
		{
		case SOUTHWEST:
			SDL_RenderCopy(ren, tex, &blockClip[0][4], &pos);
			railway[railID][0].x = railway[railID][blockID].x;
			railway[railID][0].y = railway[railID][blockID].y - 1;
			break;
		case WEST:
			SDL_RenderCopy(ren, tex, &blockClip[0][1], &pos);
			railway[railID][0].x = railway[railID][blockID].x;
			railway[railID][0].y = railway[railID][blockID].y - 1;
			break;
		case NORTHWEST:
			SDL_RenderCopy(ren, tex, &blockClip[0][2], &pos);
			railway[railID][0].y = railway[railID][blockID].y;
			railway[railID][0].x = railway[railID][blockID].x + 1;
			break;
		case NORTH:
			SDL_RenderCopy(ren, tex, &blockClip[0][0], &pos);
			railway[railID][0].y = railway[railID][blockID].y;
			railway[railID][0].x = railway[railID][blockID].x + 1;
			break;
		case NORTHEAST:
			SDL_RenderCopy(ren, tex, &blockClip[0][3], &pos);
			railway[railID][0].x = railway[railID][blockID].x;
			railway[railID][0].y = railway[railID][blockID].y + 1;
			break;
		case EAST:
			SDL_RenderCopy(ren, tex, &blockClip[0][1], &pos);
			railway[railID][0].x = railway[railID][blockID].x;
			railway[railID][0].y = railway[railID][blockID].y + 1;
			break;
		case SOUTHEAST:
			SDL_RenderCopy(ren, tex, &blockClip[0][5], &pos);
			railway[railID][0].y = railway[railID][blockID].y;
			railway[railID][0].x = railway[railID][blockID].x - 1;
			break;
		case SOUTH:
			SDL_RenderCopy(ren, tex, &blockClip[0][0], &pos);
			railway[railID][0].y = railway[railID][blockID].y;
			railway[railID][0].x = railway[railID][blockID].x - 1;
			break;
		}
	}
}

//��ȡ������Ϣ
static void judgeCommon(int extRailID, int extBlockID)
{
	int id;
	for (int railID = 1; railID < railNum; railID++)
		for (int blockID = 0; blockID < train[railID].railwayLength; blockID++)
		{
			if (blockID == 0)
			{
				if (railway[railID][blockID].common == railway[extRailID][extBlockID].common && railway[railID][train[railID].railwayLength - 1].common == 0 && railway[extRailID][extBlockID + 1].common == 0)//���
				{
					id = railID;
					commonInfo[id - 1][0] = id;
					commonInfo[id - 1][1] = blockID;
					commonInfo[id - 1][4] = extBlockID;
					break;
				}
			}
			else
			{
				if (railway[railID][blockID].common == railway[extRailID][extBlockID].common && railway[railID][blockID - 1].common == 0 && railway[extRailID][extBlockID + 1].common == 0)//���
				{
					id = railID;
					commonInfo[id - 1][0] = id;
					commonInfo[id - 1][1] = blockID;
					commonInfo[id - 1][4] = extBlockID;
					break;
				}
			}
			if (railway[railID][blockID].common == railway[extRailID][extBlockID].common && railway[railID][blockID + 1].common == 0 && railway[extRailID][extBlockID - 1].common == 0)//����
			{
				id = railID;
				commonInfo[id - 1][0] = id;
				commonInfo[id - 1][2] = blockID;
				commonInfo[id - 1][3] = extBlockID;
				break;
			}
		}
}

//����վ��
void drawStations(SDL_Renderer* ren, SDL_Texture* block)
{
	//����վ��
	for (int railID = 0; railID < railNum; ++railID)
		for (int blockID = 0; blockID < train[railID].railwayLength; ++blockID)
		{
			if (railway[railID][blockID].station == 1)
			{
				SDL_Rect dst;
				dst.x = railway[railID][blockID].x*BLOCK_SIZE;
				dst.y = railway[railID][blockID].y*BLOCK_SIZE;
				dst.w = BLOCK_SIZE;
				dst.h = BLOCK_SIZE;
				switch (railway[railID][blockID].direction)
				{
				case WEST:SDL_RenderCopy(ren, block, &blockClip[2][1], &dst); break;
				case NORTH:SDL_RenderCopy(ren, block, &blockClip[2][0], &dst); break;
				case EAST:SDL_RenderCopy(ren, block, &blockClip[2][1], &dst); break;
				case SOUTH:SDL_RenderCopy(ren, block, &blockClip[2][0], &dst); break;
				default:SDL_RenderCopy(ren, block, &blockClip[2][0], &dst); break;
				}
			}
			//SDL_RenderPresent(ren);//���Կ���
		}
}