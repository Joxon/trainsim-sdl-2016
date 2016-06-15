#ifndef DRAWSDL_H
#define DRAWSDL_H
#include <SDL.h>
#include <SDL_ttf.h>

void drawRailway(SDL_Window * win, SDL_Renderer * ren, SDL_Texture * block);
void drawTrain(SDL_Window * win, SDL_Renderer * ren, SDL_Texture * train);
void drawUI(SDL_Window * win, SDL_Renderer * ren, SDL_Texture * button, SDL_Texture * banner, TTF_Font * font);

static void drawCrossBlock(int j, int i, SDL_Renderer * ren, SDL_Texture * tex);
static void drawNormalBlock(int j, int i, SDL_Renderer * ren, SDL_Texture * tex);
static void judge_com(int j, int i);

#endif