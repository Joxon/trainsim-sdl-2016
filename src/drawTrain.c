#include <SDL.h>
#include "drawSDL.h"
#include "var.h"

extern int trainNum;
extern struct train train[MAX_TRAIN];
extern struct block railway[MAX_RAIL][MAX_RAIL_LENGTH];

extern SDL_Rect trainClip[MAX_TRAIN];

void drawTrain(SDL_Renderer * ren, SDL_Texture * trainTexture)
{
	for (int trainID = 0; trainID < trainNum; ++trainID)
	{
		SDL_Rect dst;
		dst.x = railway[trainID][train[trainID].position].x*BLOCK_SIZE;
		dst.y = railway[trainID][train[trainID].position].y*BLOCK_SIZE;
		dst.w = BLOCK_SIZE;
		dst.h = BLOCK_SIZE;

		if (train[trainID].direction == NORMAL)
			switch (railway[trainID][train[trainID].position].direction)
			{
			case SOUTHWEST:SDL_RenderCopyEx(ren, trainTexture, &trainClip[trainID], &dst, -135, NULL, SDL_FLIP_NONE); break;
			case WEST:SDL_RenderCopyEx(ren, trainTexture, &trainClip[trainID], &dst, -90, NULL, SDL_FLIP_NONE); break;
			case NORTHWEST:SDL_RenderCopyEx(ren, trainTexture, &trainClip[trainID], &dst, -45, NULL, SDL_FLIP_NONE); break;
			case NORTH:SDL_RenderCopy(ren, trainTexture, &trainClip[trainID], &dst); break;
			case NORTHEAST:SDL_RenderCopyEx(ren, trainTexture, &trainClip[trainID], &dst, 45, NULL, SDL_FLIP_NONE); break;
			case EAST:SDL_RenderCopyEx(ren, trainTexture, &trainClip[trainID], &dst, 90, NULL, SDL_FLIP_NONE); break;
			case SOUTHEAST:SDL_RenderCopyEx(ren, trainTexture, &trainClip[trainID], &dst, 135, NULL, SDL_FLIP_NONE); break;
			case SOUTH:SDL_RenderCopyEx(ren, trainTexture, &trainClip[trainID], &dst, 0, NULL, SDL_FLIP_HORIZONTAL); break;
			}
		else if (train[trainID].direction == REVERSE)
			switch (railway[trainID][train[trainID].position].direction)
			{
			case SOUTHWEST:SDL_RenderCopyEx(ren, trainTexture, &trainClip[trainID], &dst, 45, NULL, SDL_FLIP_NONE); break;
			case WEST:SDL_RenderCopyEx(ren, trainTexture, &trainClip[trainID], &dst, 90, NULL, SDL_FLIP_NONE); break;
			case NORTHWEST:SDL_RenderCopyEx(ren, trainTexture, &trainClip[trainID], &dst, 135, NULL, SDL_FLIP_NONE); break;
			case NORTH:SDL_RenderCopyEx(ren, trainTexture, &trainClip[trainID], &dst, 0, NULL, SDL_FLIP_HORIZONTAL); break;
			case NORTHEAST:SDL_RenderCopyEx(ren, trainTexture, &trainClip[trainID], &dst, -135, NULL, SDL_FLIP_NONE); break;
			case EAST:SDL_RenderCopyEx(ren, trainTexture, &trainClip[trainID], &dst, -90, NULL, SDL_FLIP_NONE); break;
			case SOUTHEAST:SDL_RenderCopyEx(ren, trainTexture, &trainClip[trainID], &dst, -45, NULL, SDL_FLIP_NONE); break;
			case SOUTH:SDL_RenderCopy(ren, trainTexture, &trainClip[trainID], &dst); break;
			}
		//SDL_RenderPresent(ren);//µ÷ÊÔ¿ÉÓÃ
	}
}

