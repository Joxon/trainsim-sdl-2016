#ifndef DRAWSDL_H
#define DRAWSDL_H
#include <SDL.h>
#include <SDL_ttf.h>

void drawRailway(SDL_Renderer * ren, SDL_Texture * block);
static void drawCrossBlock(int railID, int blockID, SDL_Renderer * ren, SDL_Texture * tex);
static void drawNormalBlock(int railID, int blockID, SDL_Renderer * ren, SDL_Texture * tex);
static void judgeCommon(int extRailID, int extBlockID);
void drawStations(SDL_Renderer* ren, SDL_Texture* block);

void drawTrain(SDL_Renderer * ren, SDL_Texture * train);
void drawUI(SDL_Renderer * ren, SDL_Texture * button, SDL_Texture * banner, TTF_Font * font);



#endif