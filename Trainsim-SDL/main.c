#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <process.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "output.h"
#include "state_trans.h"
#include "control.h"

int trainNum;                                 //������
int railNum;                                  //�������
struct train train[MAX_TRAIN];                         //������
float        trainSpeed[MAX_TRAIN];                    //���ٶ�
struct block railway[MAX_RAIL][MAX_RAIL_LENGTH];       //����볤������

int strategy = 0; //1��2��3�ֱ��Ӧ"�������"��"�쳵���Ȳ���"��"�˹�����"
int inputMode = FROM_KEYBOARD; //1��2�ֱ��Ӧ"���ļ���������"��"�Ӽ��̶�������"
unsigned int processTime = 0; //����ʱ��
unsigned int commandTime = 0;
char         command;

FILE *logPtr = NULL; //��־�ļ�ָ��
FILE *commandPtr = NULL; //�����ļ�ָ��
FILE *outPtr = NULL; //����ļ�ָ��

#define WINDOW_WIDTH 1400
#define WINDOW_HEIGHT 900
#define BLOCK_SIZE 60
#define BLOCK_ROW 3
#define BLOCK_COLUMN 21

//void errorFromFile();
void initFromFile();

int main(int argc, char* args[])
{
	//��ʼ���𳵺͹��
	initFromFile();

	//SDL��ʼ��
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("Trainsim", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

	SDL_Rect trainViewport;
	trainViewport.h = WINDOW_HEIGHT;
	trainViewport.w = WINDOW_WIDTH * 5 / 6;
	trainViewport.x = 0;
	trainViewport.y = 0;

	SDL_Rect userViewport;
	userViewport.h = WINDOW_HEIGHT;
	userViewport.w = WINDOW_WIDTH / 6;
	userViewport.x = WINDOW_WIDTH * 5 / 6;
	userViewport.y = 0;

	SDL_Renderer* blockRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Texture* blocks = IMG_LoadTexture(blockRenderer, "blocks.png");
	SDL_Rect clip[BLOCK_ROW][BLOCK_COLUMN];
	for (int i = 0; i < BLOCK_ROW; ++i)
		for (int j = 0; j < BLOCK_COLUMN; ++j)
		{
			clip[i][j].x = i*BLOCK_SIZE;
			clip[i][j].y = j*BLOCK_SIZE;
			clip[i][j].w = BLOCK_SIZE;
			clip[i][j].h = BLOCK_SIZE;
		}

	SDL_Renderer* trainRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_Renderer* userRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


	bool quit = false;
	SDL_Event e;
	while (!quit)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT) quit = true;

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
			print();

			//���ƶ�
			for (i = 0; i < trainNum; ++i)
			{
				changeDirection(&train[i], railway, i);
				changePosition(&train[i]);
			}

			//ʱ��Ƭ�ƽ�
			++processTime;
		}
	}

	SDL_DestroyTexture(blocks);
	SDL_DestroyRenderer(blockRenderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

//void errorFromFile()
//{
//	/*�ļ��������*/
//	system("cls");
//	printf("ERROR data detected! Please check init.txt.\n"
//		"Press any key to exit...");
//	_getch();
//	exit(EXIT_FAILURE);
//}

void initFromFile()
{
	FILE *fp;
	int id;
	char ch;

	fp = fopen(".\\txt\\init.txt", "r");
	if (fp != NULL)
	{
		/*����ѡ��*/
		fscanf(fp, "strategy=%d\n", &strategy);
		//if (strategy != ALTERNATIVE ||
		//	strategy != FAST_FIRST ||
		//	strategy != MANUAL)
		//	errorFromFile();

		/*�𳵳�ʼ��*/
		fscanf(fp, "train.num=%d\n", &trainNum);
		//if (trainNum < 0) errorFromFile();
		for (id = 0; id < trainNum; ++id)
		{
			fscanf(fp, "train%c.speed=%f st=%d sp=%d dir=%d type=%d pt=%d\n",
				&ch, &trainSpeed[id], &train[id].startTime, &train[id].startPoint,
				&train[id].direction, &train[id].type, &train[id].pausetime);
			//if (train[id].speed < 0 || train[id].startTime < 0 || train[id].startPoint < 0 ||
			//	train[id].direction != NORMAL || train[id].direction != REVERSE ||
			//	train[id].type != FAST || train[id].type != SLOW || train[id].pausetime < 0)
			//	errorFromFile();
			train[id].position = train[id].startPoint;
			train[id].status = WAIT;
		}


		/*�����ʼ��*/
		fscanf(fp, "railway.num=%d\n", &railNum);
		//if (railNum < 0) errorFromFile();
		for (id = 0; id < railNum; ++id)
		{
			/*�����ʼ���������趨*/
			int length, southwest, northwest, northeast, southeast;
			fscanf(fp, "railway%c.len=%d sw=%d nw=%d ne=%d se=%d\n",
				&ch, &length, &southwest, &northwest, &northeast,
				&southeast);
			//if (length < 0 || southeast < 0 || northeast < 0 || northwest < 0 || southeast < 0) errorFromFile();
			train[id].railwayLength = length;
			int blockid = 0;
			while (blockid <= northwest)
			{
				railway[id][blockid].direction = WEST;
				railway[id][blockid].station = 0;
				++blockid;
			}
			while (blockid <= northeast)
			{
				railway[id][blockid].direction = NORTH;
				railway[id][blockid].station = 0;
				++blockid;
			}
			while (blockid <= southeast)
			{
				railway[id][blockid].direction = EAST;
				railway[id][blockid].station = 0;
				++blockid;
			}
			while (blockid < length)
			{
				railway[id][blockid].direction = SOUTH;
				railway[id][blockid].station = 0;
				++blockid;
			}

			/*�����ʼ�����������*/
			int common_count, common_ID, start, end;
			fscanf(fp, "cm=%d\n", &common_count);
			//if (common_count < 0) errorFromFile();
			int commomid = 1;
			while (commomid++ <= common_count)
			{
				fscanf(fp, "id=%d %d %d\n", &common_ID, &start, &end);
				//if (common_count <= 0 || start < 0 || end < 0) errorFromFile();
				for (blockid = start; blockid < end; ++blockid)
				{
					railway[id][blockid].common = common_ID;
					railway[id][blockid].last = -1;
				}
			}

			/*�����ʼ����ͣ����*/
			int stationCount, stationID = 1, stationPoint;
			fscanf(fp, "sn=%d\n", &stationCount);
			//if (stationCount < 0) errorFromFile();
			while (stationID++ <= stationCount)
			{
				fscanf(fp, "%d\n", &stationPoint);
				//if (stationPoint < 0) errorFromFile();
				railway[id][stationPoint].station = 1;
			}

			//���ƹ����ʼ���֣���ֹԽ����ʣ�i�ķ�Χ���ٶȶ���
			for (int i = 0; i < 10; ++i)
				railway[id][length + i] = railway[id][i];
		}
		fclose(fp);
		printf("init successful!");
	}

	else
	{
		printf("Cannot open init.txt. Press any key to exit...");
		_getch();
		exit(EXIT_FAILURE);
	}
}