#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include "output.h"
#include "var.h"

/* 颜色对应值：
 * 0=黑色 1=蓝色 2=绿色 3=湖蓝色 4=红色 5=紫色 6=黄色 7=白色 8=灰色　　
 * 9=亮蓝色 10=亮绿色 11=亮青色 12=亮红色 13=亮紫色 14=亮黄色 15=亮白色
 */

#define GREEN 10
#define CYAN 11
#define RED 12
#define WHITE 15

extern int          trainNum;
extern int          railNum;
extern struct train train[MAX_TRAIN];
extern struct block railway[MAX_RAIL][MAX_RAIL_LENGTH];

extern unsigned short strategy;
extern unsigned short inputMode;
extern int            processTime;

extern FILE *logPtr;
extern FILE *outPtr;

void print()
{
	printPerSec();
	printTrans();

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
	printf("\n=================================================================\n"
		"[ i: Input | s: Stop | q: Exit ]\n"
		"[ Enter your choice ]:");
}

static void printPerSec()
{
    if (logPtr == NULL)
    {
        logPtr = fopen(".\\txt\\log.txt", "w");
        if (logPtr == NULL)
        {
            logprintf("log file cannot be opened! Exiting...");
            exit(EXIT_FAILURE);
        }
        goto print;
    }
    else
    {
    print:
        system("cls");
        int id;
        //标题与综合信息
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
        printf(
            "=================================================================\n"
            "===================== ");

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), CYAN);
		printf("Train Simulation");

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
        printf(
            " ==========================\n"
            "|                                                               |\n");

		logprintf("|------------------------- ");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), CYAN);
		logprintf("General");

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
        logprintf(
            " -----------------------------|\n"
            "|                                                               |\n"
        );

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREEN);
        logprintf(
            "|                Clock = %-3ds                                   |\n"
            "|           Input Mode = %-20s                   |\n"
            "|             Strategy = %-20s                   |\n",
            processTime,
            inputMode == FROM_FILE ? "From file" : "From keyboard",
            strategy == 1 ? "Alternative mode" :
            strategy == 2 ? "Fast-first mode" : "Manual mode");

        //火车信息
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
        logprintf(
            "|                                                               |\n"
            "|--------------------- ");

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), CYAN);
		logprintf("Train Status");

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
        logprintf(
            " ----------------------------|\n"
            "|                                                               |\n");

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREEN);
		for (id = 0; id < trainNum; ++id)
			logprintf("| ID: %c | Speed: %-3.1f | Dir: %-5s | Pos: %c[%2d][%6s] | Status: %-s\n",
				'A' + id,
				train[id].speed,
				train[id].dir,
				'A' + id, train[id].position,
				railway[id][train[id].position].common != 0 ? "Commom" : "Normal",
				train[id].status == RUN ? "Running" :
				train[id].status == PAUSE_COMMON ? "Paused/common" :
				train[id].status == PAUSE_ANY ? "Paused/user" :
				train[id].status == WAIT ? "Waiting" : "ERROR");

        //轨道信息
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
        logprintf(
            "|                                                               |\n"
            "|-------------------- ");

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), CYAN);
		logprintf("Railway Status");

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
        logprintf(
            " ---------------------------|\n"
            "|                                                               |");

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREEN);
		for (id = 0; id < railNum; ++id)
		{
			logprintf("\n|/ Railway ID: %c | Length:%d", 'A' + id, train[id].railwayLength);
			int block, lastID = 0;
			for (block = 0; railway[id][block].direction != 0; ++block)
				if (railway[id][block].common != 0 && railway[id][block].common != lastID)
				{
					int blockForward;
					for (blockForward = block; railway[id][blockForward].common == railway[id][block].common;
						++blockForward);
					logprintf("\n| Common ID: %d | stp: %d edp: %d", railway[id][block].common, block, blockForward);
					lastID = railway[id][block].common;
				}
		}
		//共轨状态
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), CYAN);
		logprintf("\n|/ Common Tracks:\n");

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREEN);
		for (id = 0; id < trainNum; ++id)
			if (railway[id][train[id].position].common != 0)
				logprintf("| Common ID: %d | Status: %c using\n", railway[id][train[id].position].common, 'A' + id);
	}
}

static void printTrans()
{
	static int   flag = 0;
	static int   pre_point[3];
	static float pre_speed[3];
	int          i, j, k;

	if (flag == 0)
	{
		for (i = 0; i < 3; ++i)
		{
			pre_point[i] = train[i].startPoint;
			pre_speed[i] = train[i].speed;
		}
		flag = 1;
	}

	if (outPtr == NULL)
	{
		outPtr = fopen(".\\txt\\output.txt", "w");
		if (outPtr == NULL)
		{
			outprintf("output file cannot be opened! Exiting...");
			exit(EXIT_FAILURE);
		}
		goto print;
	}
	else
	{
	print:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREEN);
		//状态变化
		//A
		if (railway[0][train[0].position].common != railway[0][pre_point[0]].common)
		{
			if (railway[0][pre_point[0]].common == 0)
			{
				for (k = 0, j = 0; railway[1][j].direction != 0 && k == 0; j++)
					if (railway[1][j].common == railway[0][train[0].position].common)
						k++;
				if (k != 0)
					outprintf("T=%d; TrainA; Trans: A->A-B\n", processTime);
				else
					outprintf("T=%d; TrainA; Trans: A->A-C\n", processTime);
			}
			else
			{
				for (k = 0, j = 0; railway[1][j].direction != 0 && k == 0; j++)
					if (railway[1][j].common == railway[0][pre_point[0]].common)
						k++;
				if (k != 0)
					outprintf("T=%d; TrainA; Trans: A-B->A\n", processTime);
				else
					outprintf("T=%d; TrainA; Trans: A-C->A\n", processTime);
			}
		}
		//B
		if (railway[1][train[1].position].common != railway[1][pre_point[1]].common)
		{
			if (railway[1][pre_point[1]].common == 0)
			{
				for (k = 0, j = 0; railway[0][j].direction != 0 && k == 0; j++)
					if (railway[0][j].common == railway[1][train[1].position].common)
						k++;
				if (k != 0)
					outprintf("T=%d; TrainB; Trans: B->A-B\n", processTime);
				else
					outprintf("T=%d; TrainB; Trans: B->B-C\n", processTime);
			}
			else
			{
				for (k = 0, j = 0; railway[0][j].direction != 0 && k == 0; j++)
					if (railway[0][j].common == railway[1][pre_point[1]].common)
						k++;
				if (k != 0)
					outprintf("T=%d; TrainB; Trans: A-B->B\n", processTime);
				else
					outprintf("T=%d; TrainB; Trans: B-C->B\n", processTime);
			}
		}
		//C
		if (railway[2][train[2].position].common != railway[2][pre_point[2]].common)
		{
			if (railway[2][pre_point[2]].common == 0)
			{
				for (k = 0, j = 0; railway[0][j].direction != 0 && k == 0; j++)
					if (railway[0][j].common == railway[2][train[2].position].common)
						k++;
				if (k != 0)
					outprintf("T=%d; TrainC; Trans: C->A-C\n", processTime);
				else
					outprintf("T=%d; TrainC; Trans: C->B-C\n", processTime);
			}
			else
			{
				for (k = 0, j = 0; railway[0][j].direction != 0 && k == 0; j++)
					if (railway[0][j].common == railway[2][pre_point[2]].common)
						k++;
				if (k != 0)
					outprintf("T=%d; TrainC; Trans: A-C->C\n", processTime);
				else
					outprintf("T=%d; TrainC; Trans: B-C->C\n", processTime);
			}
		}
		//speed changing
		for (i = 0; i < 3; i++)
		{
			if (pre_speed[i] != train[i].speed)
			{
				if (pre_speed[i] == 0)
					outprintf("T=%d; Train%c; Start\n", processTime, 'A' + i);
				else
				{
					if (train[i].speed == 0)
						outprintf("T=%d; Train%c; Stop\n", processTime, 'A' + i);
					else
					{
						if (pre_speed[i] > train[i].speed)
							outprintf("T=%d; Train%c; Speed Down\n", processTime, 'A' + i);
						else
							outprintf("T=%d; Train%c; Speed Up\n", processTime, 'A' + i);
					}
				}
			}
		}
		for (i = 0; i < 3; i++)
		{
			pre_point[i] = train[i].position;
			pre_speed[i] = train[i].speed;
		}
	}
}

static int logprintf(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	if (logPtr != NULL)
		vfprintf(logPtr, format, args);
	return vprintf(format, args);
}

static int outprintf(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	if (outPtr != NULL)
		vfprintf(outPtr, format, args);
	return vprintf(format, args);
}
