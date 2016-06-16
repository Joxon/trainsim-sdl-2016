#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <process.h>

#include <SDL.h>
//#include <SDL_thread.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "input.h"
#include "output.h"
#include "state_trans.h"
#include "control.h"
#include "drawSDL.h"

int trainNum;                                 //火车数量
int railNum;                                  //轨道数量
struct train train[MAX_TRAIN];                         //火车上限
float        trainSpeed[MAX_TRAIN];                    //火车速度
struct block railway[MAX_RAIL][MAX_RAIL_LENGTH];       //轨道与长度上限

int strategy = 0; //1，2，3分别对应"交替策略"，"快车优先策略"，"人工控制"
int inputMode = FROM_KEYBOARD; //1，2分别对应"从文件读入命令"，"从键盘读入命令"
int processTime = 0; //程序时钟
int commandTime = 0;
char         command;

FILE *logPtr = NULL; //日志文件指针
FILE *commandPtr = NULL; //命令文件指针
FILE *outPtr = NULL; //输出文件指针

SDL_Rect trainClip[MAX_TRAIN];
SDL_Rect blockClip[BLOCK_ROW][BLOCK_COLUMN];
SDL_Rect buttonClip[BUTTON_ROW][BUTTON_COLUMN];
SDL_Rect strategyButtonPos[3];
SDL_Rect trainButtonPos[3][3];
SDL_Rect exitButtonPos;

//void errorFromFile()
//{
//	/*文件输入错误*/
//	system("cls");
//	printf("ERROR data detected! Please check init.txt.\n"
//		"Press any key to exit...");
//	_getch();
//	exit(EXIT_FAILURE);
//}


//struct drawUIArgs
//{
//	SDL_Renderer* renderer;
//	SDL_Texture* buttonsTexture;
//	SDL_Texture* bannerTexture;
//	TTF_Font* font;
//	SDL_Rect userViewport;
//};
//
//int drawUIFunc(void* args)
//{
//	struct drawUIArgs* argsConverted = (struct drawUIArgs*)args;
//	while (!quit)
//	{
//		//渲染用户输入界面，注意是以userViewport的左上角为绘图零点
//		SDL_RenderSetViewport(argsConverted->renderer, &argsConverted->userViewport);
//		drawUI(argsConverted->renderer,
//			argsConverted->buttonsTexture,
//			argsConverted->bannerTexture,
//			argsConverted->font);
//		SDL_RenderPresent(argsConverted->renderer);
//	}
//
//	return 0;
//}

int main(int argc, char* argv[])
{
	//初始化火车和轨道
	init();

versionSelect:
	system("cls");
	printf("你希望用那个版本？\n");
	printf("1：命令行+图形化\n");
	printf("2：仅命令行\n");
	printf("q：退出\n");
	printf("请输入选择：");

	char version = _getche();
	if (version == '1')
	{
		//SDL初始化
		SDL_Init(SDL_INIT_VIDEO);
		IMG_Init(IMG_INIT_PNG);
		TTF_Init();

		//创建窗口和渲染器
		SDL_Window* window = SDL_CreateWindow("Trainsim", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
		SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

		//加载字体
		//  初号 = 42磅 = 14.82毫米
		//	小初 = 36磅 = 12.70毫米
		//	一号 = 26磅 = 9.17毫米
		//	小一 = 24磅 = 8.47毫米
		//	二号 = 22磅 = 7.76毫米
		//	小二 = 18磅 = 6.35毫米
		//	三号 = 16磅 = 5.64毫米
		//	小三 = 15磅 = 5.29毫米
		//	四号 = 14磅 = 4.94毫米
		//	小四 = 12磅 = 4.23毫米
		//	五号 = 10.5磅 = 3.70毫米
		//	小五 = 9磅 = 3.18毫米
		//	六号 = 7.5磅 = 2.56毫米
		//	小六 = 6.5磅 = 2.29毫米
		//	七号 = 5.5磅 = 1.94毫米
		//	八号 = 5磅 = 1.76毫米
		TTF_Font* font = TTF_OpenFont(".\\resources\\font.ttf", 30);

		//加载火车纹理
		SDL_Texture* trainTexture = IMG_LoadTexture(renderer, ".\\resources\\trains.png");
		for (int i = 0; i < MAX_TRAIN; ++i)
		{
			trainClip[i].x = i*BLOCK_SIZE;
			trainClip[i].y = 0;
			trainClip[i].w = BLOCK_SIZE;
			trainClip[i].h = BLOCK_SIZE;
		}

		//加载轨道块纹理
		SDL_Texture* blocksTexture = IMG_LoadTexture(renderer, ".\\resources\\blocks.png");
		for (int i = 0; i < MAX_TRAIN; ++i)
			for (int j = 0; j < BLOCK_COLUMN; ++j)
			{
				blockClip[i][j].x = j*BLOCK_SIZE;
				blockClip[i][j].y = i*BLOCK_SIZE;
				blockClip[i][j].w = BLOCK_SIZE;
				blockClip[i][j].h = BLOCK_SIZE;
			}

#ifdef BLOCK_TEST
		for (int i = 0; i < BLOCK_ROW; ++i)
			for (int j = 0; j < BLOCK_COLUMN; ++j)
			{
				SDL_Rect pos;
				pos.x = j*BLOCK_SIZE;
				pos.y = i*BLOCK_SIZE;
				pos.w = BLOCK_SIZE;
				pos.h = BLOCK_SIZE;
				SDL_RenderCopy(renderer, blocksTexture, &blockClip[i][j], &pos);
			}
		SDL_RenderPresent(renderer);
#endif


		//加载标题栏
		SDL_Texture* bannerTexture = IMG_LoadTexture(renderer, ".\\resources\\banner.png");

		//加载按钮纹理
		SDL_Texture* buttonsTexture = IMG_LoadTexture(renderer, ".\\resources\\buttons.png");
		for (int y = 0; y < BUTTON_ROW; ++y)
			for (int x = 0; x < BUTTON_COLUMN; ++x)
			{
				buttonClip[y][x].x = x*BUTTON_WIDTH;
				buttonClip[y][x].y = y*BUTTON_HEIGHT;
				buttonClip[y][x].w = BUTTON_WIDTH;
				buttonClip[y][x].h = BUTTON_HEIGHT;
			}

		//火车和轨道界面
		SDL_Rect trainViewport;
		trainViewport.x = WINDOW_WIDTH / 6;
		trainViewport.y = 0;
		trainViewport.h = WINDOW_HEIGHT;
		trainViewport.w = WINDOW_WIDTH * 5 / 6;

		//用户输入界面
		SDL_Rect userViewport;
		userViewport.x = 0;
		userViewport.y = 0;
		userViewport.h = WINDOW_HEIGHT;
		userViewport.w = WINDOW_WIDTH * 5 / 6;

		//用户界面线程
		//struct drawUIArgs drawUIArgs;
		//drawUIArgs.bannerTexture = bannerTexture;
		//drawUIArgs.buttonsTexture = buttonsTexture;
		//drawUIArgs.font = font;
		//drawUIArgs.renderer = renderer;
		//drawUIArgs.userViewport = userViewport;
		//SDL_Thread* drawUIThreadHandle = SDL_CreateThread(drawUIFunc, "drawUIThread", (void*)(&drawUIArgs));

		//主循环
		bool quit = false;
		while (!quit)
		{
			int i;
			//控制命令
			if (inputMode == FROM_FILE)
			{
				for (i = 0; i < trainNum; ++i)
					if (processTime == train[i].startTime)
						train[i].speed = trainSpeed[i];     //到启动时刻返还速度
				getInputFromFile();
			}
			else if (inputMode == FROM_KEYBOARD)
				for (i = 0; i < trainNum; ++i)
					if (processTime == train[i].startTime)
						train[i].speed = trainSpeed[i];     //到启动时刻返还速度

			//状态变换
			for (i = 0; i < trainNum; ++i)
				trans(&train[i], railway, i);

			//控制台与文件输出
			printConsoleAndFile();

			//火车移动
			for (i = 0; i < trainNum; ++i)
			{
				changeDirection(&train[i], railway, i);
				changePosition(&train[i]);
			}

			//设置背景为土色
			SDL_SetRenderDrawColor(renderer, 239, 227, 182, 255);
			SDL_RenderClear(renderer);

			//渲染轨道和火车，是以trainViewport的左上角为绘图零点
			SDL_RenderSetViewport(renderer, &trainViewport);
			drawRailway(renderer, blocksTexture);
			drawTrain(renderer, trainTexture);
			drawStations(renderer, blocksTexture);

			//渲染用户输入界面，注意是以userViewport的左上角为绘图零点
			SDL_RenderSetViewport(renderer, &userViewport);
			drawUI(renderer, buttonsTexture, bannerTexture, font);

			//处理事件
			SDL_Event e;
			while (SDL_PollEvent(&e))
			{
				//点击右上角退出
				if (e.type == SDL_QUIT) quit = true;
			}

			//获得鼠标坐标
			int x, y;
			SDL_GetMouseState(&x, &y);

			//历遍各个策略按钮
			for (int i = 0; i < 3; ++i)
				//如果鼠标在按钮范围内
				if ((x > strategyButtonPos[i].x) &&
					(x < strategyButtonPos[i].x + strategyButtonPos[i].w) &&
					(y > strategyButtonPos[i].y) &&
					(y < strategyButtonPos[i].y + strategyButtonPos[i].h))
				{
					//按钮显示为预备状态
					SDL_RenderCopy(renderer, buttonsTexture, &buttonClip[i][2], &strategyButtonPos[i]);
					//如果点击鼠标
					if (e.type == SDL_MOUSEBUTTONDOWN)
					{
						//按钮显示为按下状态
						SDL_RenderCopy(renderer, buttonsTexture, &buttonClip[i][0], &strategyButtonPos[i]);
						//触发修改策略
						strategy = i + ALTERNATIVE;
					}
				}

			//历遍各个火车按钮
			for (int trainID = 0; trainID < trainNum; ++trainID)
				for (int buttonID = 0; buttonID < 3; ++buttonID)
					if ((x > trainButtonPos[trainID][buttonID].x) &&
						(x < trainButtonPos[trainID][buttonID].x + trainButtonPos[trainID][buttonID].w) &&
						(y > trainButtonPos[trainID][buttonID].y) &&
						(y < trainButtonPos[trainID][buttonID].y + trainButtonPos[trainID][buttonID].h))
					{
						//按钮显示为预备状态
						SDL_RenderCopy(renderer, buttonsTexture, &buttonClip[buttonID + 3][2], &trainButtonPos[trainID][buttonID]);
						//如果点击鼠标
						if (e.type == SDL_MOUSEBUTTONDOWN)
						{
							//按钮显示为按下状态
							SDL_RenderCopy(renderer, buttonsTexture, &buttonClip[buttonID + 3][0], &trainButtonPos[trainID][buttonID]);
							//进行相应的操作
							switch (buttonID)
							{
							case 0://暂停
								if (train[trainID].speed != 0)
								{
									trainSpeed[trainID] = train[trainID].speed;
									train[trainID].speed = 0;
								}
								break;
							case 1://恢复
								if (train[trainID].speed == 0)
									train[trainID].speed = trainSpeed[trainID];
								break;
							case 2://反向
								if (train[trainID].direction == NORMAL)
									train[trainID].direction = REVERSE;
								else if (train[trainID].direction == REVERSE)
									train[trainID].direction = NORMAL;
								break;
							}
						}
					}

			//退出按钮
			if ((x > exitButtonPos.x) &&
				(x < exitButtonPos.x + exitButtonPos.w) &&
				(y > exitButtonPos.y) &&
				(y < exitButtonPos.y + exitButtonPos.h))
			{
				//按钮显示为预备状态
				SDL_RenderCopy(renderer, buttonsTexture, &buttonClip[6][2], &exitButtonPos);
				//如果点击鼠标
				if (e.type == SDL_MOUSEBUTTONDOWN)
				{
					//按钮显示为按下状态
					SDL_RenderCopy(renderer, buttonsTexture, &buttonClip[6][0], &exitButtonPos);
					quit = true;
				}
			}

			//完成绘制后呈现
			SDL_RenderPresent(renderer);

			//时间片推进
			processTime++;
			SDL_Delay(1000 / 24);
		}


		//释放资源
		TTF_CloseFont(font); font = NULL;
		SDL_DestroyTexture(trainTexture); trainTexture = NULL;
		SDL_DestroyTexture(bannerTexture); bannerTexture = NULL;
		SDL_DestroyTexture(blocksTexture); blocksTexture = NULL;
		SDL_DestroyTexture(buttonsTexture); buttonsTexture = NULL;
		SDL_DestroyRenderer(renderer); renderer = NULL;
		SDL_DestroyWindow(window); window = NULL;

		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
	}
	else if (version == '2')
	{
		while (1)
		{
			//非阻塞式接收键盘输入
			if (_kbhit())
			{
			retry:
				fflush(stdin);
				command = _getche();
				if (command == 'i')
					getInputFromKeyboard();
				else if (command == 's')
				{
					printf("\nPress any key to resume..");
					_getch();
					continue;
				}
				else if (command == 'q')
					break;
				else
				{
					printf("\nUnknown command! Please retry.\n");
					goto retry;
				}
			}

			int i;
			//控制命令
			if (inputMode == FROM_FILE)
			{
				for (i = 0; i < trainNum; ++i)
					if (processTime == train[i].startTime)
						train[i].speed = trainSpeed[i];     //到启动时刻返还速度
				getInputFromFile();
			}
			else if (inputMode == FROM_KEYBOARD)
				for (i = 0; i < trainNum; ++i)
					if (processTime == train[i].startTime)
						train[i].speed = trainSpeed[i];     //到启动时刻返还速度

			//状态变换
			for (i = 0; i < trainNum; ++i)
				trans(&train[i], railway, i);

			//输出
			printConsoleAndFile();

			//火车移动
			for (i = 0; i < trainNum; ++i)
			{
				changeDirection(&train[i], railway, i);
				changePosition(&train[i]);
			}

			//时间片推进
			++processTime;
			Sleep(1000);
		}
	}
	else if (version == 'q') return 0;
	else 
	{
		printf("输入错误。任意键重试。");
		_getch();
		goto versionSelect;
	}

	return 0;
}

