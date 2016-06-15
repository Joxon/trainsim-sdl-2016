#ifndef DRAWSDL_H
#define DRAWSDL_H
#include <SDL.h>

void drawRailway(SDL_Window * win, SDL_Renderer * ren, SDL_Texture * block);
void drawTrain();
void drawUI();

static void drawCrossBlock(int j, int i, SDL_Renderer * ren, SDL_Texture * tex);
static void drawNormalBlock(int j, int i, SDL_Renderer * ren, SDL_Texture * tex);
static void judge_com(int j, int i);

#endif