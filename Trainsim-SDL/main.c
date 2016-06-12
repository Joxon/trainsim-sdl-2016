#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <process.h>
#include "input.h"
#include "output.h"
#include "state_trans.h"
#include "control.h"

#ifdef DEBUG_TRANS
#include <stdlib.h>
#include <time.h>
#endif

//#define DEBUG_TRANS       //调试开关
#define FALSE_THREAD        //伪线程开关

int trainNum;                                 //火车数量
int railNum;                                  //轨道数量
struct train train[MAX_TRAIN];                         //火车上限
float        trainSpeed[MAX_TRAIN];                    //火车速度
struct block railway[MAX_RAIL][MAX_RAIL_LENGTH];       //轨道与长度上限

int strategy = 0; //1，2，3分别对应"交替策略"，"快车优先策略"，"人工控制"
int inputMode = 0; //1，2分别对应"从文件读入命令"，"从键盘读入命令"
unsigned int processTime = 0; //程序时钟
unsigned int commandTime = 0;
char         command;

FILE *logPtr = NULL; //日志文件指针
FILE *commandPtr = NULL; //命令文件指针
FILE *outPtr = NULL; //输出文件指针

int main(void)
{
	//初始化火车和轨道
	init();

	while (1)
	{
#ifdef FALSE_THREAD
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
#endif

#ifdef DEBUG_TRANS
		srand((unsigned)time(NULL));
		for (int i = 0; i < 3; ++i)
			train[i].position = rand() % 10;
#else
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
#endif
		//输出
		print();

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
	return 0;
}
