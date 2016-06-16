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

int trainNum;                                 //������
int railNum;                                  //�������
struct train train[MAX_TRAIN];                         //������
float        trainSpeed[MAX_TRAIN];                    //���ٶ�
struct block railway[MAX_RAIL][MAX_RAIL_LENGTH];       //����볤������

int strategy = 0; //1��2��3�ֱ��Ӧ"�������"��"�쳵���Ȳ���"��"�˹�����"
int inputMode = FROM_KEYBOARD; //1��2�ֱ��Ӧ"���ļ���������"��"�Ӽ��̶�������"
int processTime = 0; //����ʱ��
int commandTime = 0;
char         command;

FILE *logPtr = NULL; //��־�ļ�ָ��
FILE *commandPtr = NULL; //�����ļ�ָ��
FILE *outPtr = NULL; //����ļ�ָ��

SDL_Rect trainClip[MAX_TRAIN];
SDL_Rect blockClip[BLOCK_ROW][BLOCK_COLUMN];
SDL_Rect buttonClip[BUTTON_ROW][BUTTON_COLUMN];
SDL_Rect strategyButtonPos[3];
SDL_Rect trainButtonPos[3][3];
SDL_Rect exitButtonPos;

//void errorFromFile()
//{
//	/*�ļ��������*/
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
//		//��Ⱦ�û�������棬ע������userViewport�����Ͻ�Ϊ��ͼ���
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
	//��ʼ���𳵺͹��
	init();

versionSelect:
	system("cls");
	printf("��ϣ�����Ǹ��汾��\n");
	printf("1��������+ͼ�λ�\n");
	printf("2����������\n");
	printf("q���˳�\n");
	printf("������ѡ��");

	char version = _getche();
	if (version == '1')
	{
		//SDL��ʼ��
		SDL_Init(SDL_INIT_VIDEO);
		IMG_Init(IMG_INIT_PNG);
		TTF_Init();

		//�������ں���Ⱦ��
		SDL_Window* window = SDL_CreateWindow("Trainsim", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
		SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

		//��������
		//  ���� = 42�� = 14.82����
		//	С�� = 36�� = 12.70����
		//	һ�� = 26�� = 9.17����
		//	Сһ = 24�� = 8.47����
		//	���� = 22�� = 7.76����
		//	С�� = 18�� = 6.35����
		//	���� = 16�� = 5.64����
		//	С�� = 15�� = 5.29����
		//	�ĺ� = 14�� = 4.94����
		//	С�� = 12�� = 4.23����
		//	��� = 10.5�� = 3.70����
		//	С�� = 9�� = 3.18����
		//	���� = 7.5�� = 2.56����
		//	С�� = 6.5�� = 2.29����
		//	�ߺ� = 5.5�� = 1.94����
		//	�˺� = 5�� = 1.76����
		TTF_Font* font = TTF_OpenFont(".\\resources\\font.ttf", 30);

		//���ػ�����
		SDL_Texture* trainTexture = IMG_LoadTexture(renderer, ".\\resources\\trains.png");
		for (int i = 0; i < MAX_TRAIN; ++i)
		{
			trainClip[i].x = i*BLOCK_SIZE;
			trainClip[i].y = 0;
			trainClip[i].w = BLOCK_SIZE;
			trainClip[i].h = BLOCK_SIZE;
		}

		//���ع��������
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


		//���ر�����
		SDL_Texture* bannerTexture = IMG_LoadTexture(renderer, ".\\resources\\banner.png");

		//���ذ�ť����
		SDL_Texture* buttonsTexture = IMG_LoadTexture(renderer, ".\\resources\\buttons.png");
		for (int y = 0; y < BUTTON_ROW; ++y)
			for (int x = 0; x < BUTTON_COLUMN; ++x)
			{
				buttonClip[y][x].x = x*BUTTON_WIDTH;
				buttonClip[y][x].y = y*BUTTON_HEIGHT;
				buttonClip[y][x].w = BUTTON_WIDTH;
				buttonClip[y][x].h = BUTTON_HEIGHT;
			}

		//�𳵺͹������
		SDL_Rect trainViewport;
		trainViewport.x = WINDOW_WIDTH / 6;
		trainViewport.y = 0;
		trainViewport.h = WINDOW_HEIGHT;
		trainViewport.w = WINDOW_WIDTH * 5 / 6;

		//�û��������
		SDL_Rect userViewport;
		userViewport.x = 0;
		userViewport.y = 0;
		userViewport.h = WINDOW_HEIGHT;
		userViewport.w = WINDOW_WIDTH * 5 / 6;

		//�û������߳�
		//struct drawUIArgs drawUIArgs;
		//drawUIArgs.bannerTexture = bannerTexture;
		//drawUIArgs.buttonsTexture = buttonsTexture;
		//drawUIArgs.font = font;
		//drawUIArgs.renderer = renderer;
		//drawUIArgs.userViewport = userViewport;
		//SDL_Thread* drawUIThreadHandle = SDL_CreateThread(drawUIFunc, "drawUIThread", (void*)(&drawUIArgs));

		//��ѭ��
		bool quit = false;
		while (!quit)
		{
			int i;
			//��������
			if (inputMode == FROM_FILE)
			{
				for (i = 0; i < trainNum; ++i)
					if (processTime == train[i].startTime)
						train[i].speed = trainSpeed[i];     //������ʱ�̷����ٶ�
				getInputFromFile();
			}
			else if (inputMode == FROM_KEYBOARD)
				for (i = 0; i < trainNum; ++i)
					if (processTime == train[i].startTime)
						train[i].speed = trainSpeed[i];     //������ʱ�̷����ٶ�

			//״̬�任
			for (i = 0; i < trainNum; ++i)
				trans(&train[i], railway, i);

			//����̨���ļ����
			printConsoleAndFile();

			//���ƶ�
			for (i = 0; i < trainNum; ++i)
			{
				changeDirection(&train[i], railway, i);
				changePosition(&train[i]);
			}

			//���ñ���Ϊ��ɫ
			SDL_SetRenderDrawColor(renderer, 239, 227, 182, 255);
			SDL_RenderClear(renderer);

			//��Ⱦ����ͻ𳵣�����trainViewport�����Ͻ�Ϊ��ͼ���
			SDL_RenderSetViewport(renderer, &trainViewport);
			drawRailway(renderer, blocksTexture);
			drawTrain(renderer, trainTexture);
			drawStations(renderer, blocksTexture);

			//��Ⱦ�û�������棬ע������userViewport�����Ͻ�Ϊ��ͼ���
			SDL_RenderSetViewport(renderer, &userViewport);
			drawUI(renderer, buttonsTexture, bannerTexture, font);

			//�����¼�
			SDL_Event e;
			while (SDL_PollEvent(&e))
			{
				//������Ͻ��˳�
				if (e.type == SDL_QUIT) quit = true;
			}

			//����������
			int x, y;
			SDL_GetMouseState(&x, &y);

			//����������԰�ť
			for (int i = 0; i < 3; ++i)
				//�������ڰ�ť��Χ��
				if ((x > strategyButtonPos[i].x) &&
					(x < strategyButtonPos[i].x + strategyButtonPos[i].w) &&
					(y > strategyButtonPos[i].y) &&
					(y < strategyButtonPos[i].y + strategyButtonPos[i].h))
				{
					//��ť��ʾΪԤ��״̬
					SDL_RenderCopy(renderer, buttonsTexture, &buttonClip[i][2], &strategyButtonPos[i]);
					//���������
					if (e.type == SDL_MOUSEBUTTONDOWN)
					{
						//��ť��ʾΪ����״̬
						SDL_RenderCopy(renderer, buttonsTexture, &buttonClip[i][0], &strategyButtonPos[i]);
						//�����޸Ĳ���
						strategy = i + ALTERNATIVE;
					}
				}

			//��������𳵰�ť
			for (int trainID = 0; trainID < trainNum; ++trainID)
				for (int buttonID = 0; buttonID < 3; ++buttonID)
					if ((x > trainButtonPos[trainID][buttonID].x) &&
						(x < trainButtonPos[trainID][buttonID].x + trainButtonPos[trainID][buttonID].w) &&
						(y > trainButtonPos[trainID][buttonID].y) &&
						(y < trainButtonPos[trainID][buttonID].y + trainButtonPos[trainID][buttonID].h))
					{
						//��ť��ʾΪԤ��״̬
						SDL_RenderCopy(renderer, buttonsTexture, &buttonClip[buttonID + 3][2], &trainButtonPos[trainID][buttonID]);
						//���������
						if (e.type == SDL_MOUSEBUTTONDOWN)
						{
							//��ť��ʾΪ����״̬
							SDL_RenderCopy(renderer, buttonsTexture, &buttonClip[buttonID + 3][0], &trainButtonPos[trainID][buttonID]);
							//������Ӧ�Ĳ���
							switch (buttonID)
							{
							case 0://��ͣ
								if (train[trainID].speed != 0)
								{
									trainSpeed[trainID] = train[trainID].speed;
									train[trainID].speed = 0;
								}
								break;
							case 1://�ָ�
								if (train[trainID].speed == 0)
									train[trainID].speed = trainSpeed[trainID];
								break;
							case 2://����
								if (train[trainID].direction == NORMAL)
									train[trainID].direction = REVERSE;
								else if (train[trainID].direction == REVERSE)
									train[trainID].direction = NORMAL;
								break;
							}
						}
					}

			//�˳���ť
			if ((x > exitButtonPos.x) &&
				(x < exitButtonPos.x + exitButtonPos.w) &&
				(y > exitButtonPos.y) &&
				(y < exitButtonPos.y + exitButtonPos.h))
			{
				//��ť��ʾΪԤ��״̬
				SDL_RenderCopy(renderer, buttonsTexture, &buttonClip[6][2], &exitButtonPos);
				//���������
				if (e.type == SDL_MOUSEBUTTONDOWN)
				{
					//��ť��ʾΪ����״̬
					SDL_RenderCopy(renderer, buttonsTexture, &buttonClip[6][0], &exitButtonPos);
					quit = true;
				}
			}

			//��ɻ��ƺ����
			SDL_RenderPresent(renderer);

			//ʱ��Ƭ�ƽ�
			processTime++;
			SDL_Delay(1000 / 24);
		}


		//�ͷ���Դ
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
			//������ʽ���ռ�������
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
			//��������
			if (inputMode == FROM_FILE)
			{
				for (i = 0; i < trainNum; ++i)
					if (processTime == train[i].startTime)
						train[i].speed = trainSpeed[i];     //������ʱ�̷����ٶ�
				getInputFromFile();
			}
			else if (inputMode == FROM_KEYBOARD)
				for (i = 0; i < trainNum; ++i)
					if (processTime == train[i].startTime)
						train[i].speed = trainSpeed[i];     //������ʱ�̷����ٶ�

			//״̬�任
			for (i = 0; i < trainNum; ++i)
				trans(&train[i], railway, i);

			//���
			printConsoleAndFile();

			//���ƶ�
			for (i = 0; i < trainNum; ++i)
			{
				changeDirection(&train[i], railway, i);
				changePosition(&train[i]);
			}

			//ʱ��Ƭ�ƽ�
			++processTime;
			Sleep(1000);
		}
	}
	else if (version == 'q') return 0;
	else 
	{
		printf("���������������ԡ�");
		_getch();
		goto versionSelect;
	}

	return 0;
}

