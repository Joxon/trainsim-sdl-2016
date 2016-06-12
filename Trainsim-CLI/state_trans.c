#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "state_trans.h"

extern unsigned int trainNum;
extern struct train train[MAX_TRAIN];
extern struct block railway[MAX_RAIL][MAX_RAIL_LENGTH];
extern float        trainSpeed[MAX_TRAIN];
extern int strategy;
static int clock = 0, flag = 0;
int m;
static char ch;


void trans(struct train *tra, struct block rail[][MAX_RAIL_LENGTH], int i)
{
	int state = STOP, j, sum = 0;
	switch (state)
	{
	case STOP:
		if (tra->speed == 0)
		{
			if (judgeCommonTrack(tra, rail, i) == 0 && tra->status == PAUSE_COMMON)
			{
				tra->speed = trainSpeed[i];
				state = RUN;
			}
			else if (tra->status == PAUSE_STATION) {
				state = PAUSE_STATION;
			}
			else
			{
				state = STOP;
				break;
			}
		}
		else {
			for (j = 0; j < tra->speed; j++) {
				if (rail[i][tra->position + j].station != 0)
					sum++;
				if (sum > 0)
					break;
			}
			if (sum > 0) {
				state = PAUSE_STATION;
				if (tra->speed != 0) {
					tra->position += j;
					tra->status = PAUSE_STATION;
					trainSpeed[i] = tra->speed;
					tra->speed = 0;
				}
				break;
			}
			else
				state = RUN;
		}

	case PAUSE_STATION:
		if (tra->status == PAUSE_STATION) {
			clock++;
			if (clock == tra->pausetime) {
				tra->speed = trainSpeed[i];
				tra->status = RUN;
				state = RUN;
				clock = 0;
			}
			break;
		}

	case RUN:
		if (tra->direction == NORMAL)
		{
			if (rail[i][tra->position + (int)tra->speed].common != 0 && rail[i][tra->position].common == 0)
				state = BUSY;
			else
			{
				state = RUN;
				tra->status = RUN;
				break;
			}
		}
		else
		{
			if (rail[i][tra->position - (int)tra->speed].common != 0 && rail[i][tra->position].common == 0)
				state = BUSY;
			else
			{
				state = RUN;
				tra->status = RUN;
				break;
			}
		}

	case BUSY:
		if (judgeCommonTrack(tra, rail, i) == 1)
		{
			state = STOP;
			tra->status = PAUSE_COMMON;
			trainSpeed[i] = tra->speed;
			tra->speed = 0;
			break;
		}
		else if (judgeCommonTrack(tra, rail, i) == 2)
		{
			if (strategy == ALTERNATIVE) {
			here:
				if (tra->direction == NORMAL) {
					if (rail[i][tra->position + (int)tra->speed].last == i) {
						state = STOP;
						tra->status = PAUSE_COMMON;
						trainSpeed[i] = tra->speed;
						tra->speed = 0;
						break;
					}
					else {
						if (flag == 0) {
							state = RUN;
							rail[i][tra->position + (int)tra->speed].last = i;
							tra->status = RUN;
							flag = rail[i][tra->position + (int)tra->speed].common;
							break;
						}
						else if (flag == rail[i][tra->position + (int)tra->speed].common) {
							state = STOP;
							tra->speed = 0;
							tra->status = PAUSE_COMMON;
							flag = 0;
							break;
						}
					}
				}
				else {
					if (rail[i][tra->position - (int)tra->speed].last == i) {
						state = STOP;
						tra->speed = 0;
						tra->status = STOP;
						break;
					}
					else {
						state = RUN;
						rail[i][tra->position - (int)tra->speed].last = i;
						tra->status = RUN;
						break;
					}
				}
			}
			else if (strategy == FAST_FIRST) {
				if (tra->type < train[m].type) {
					state = RUN;
					break;
				}
				else if (tra->type > train[m].type) {
					state = STOP;
					tra->status = PAUSE_COMMON;
					trainSpeed[i] = tra->speed;
					tra->speed = 0;
					break;
				}
				else {
					goto here;
				}
			}
			else if (strategy == MANUAL) {
				printf("Please choose which one to run(%c or %c):", 'A' + i, 'A' + m);
				if ((ch = getchar()) == 'A' + i) {
					state = RUN;
					tra->status = PAUSE_COMMON;
					break;
				}
				else {
					state = STOP;
					tra->status = PAUSE_COMMON;
					trainSpeed[i] = tra->speed;
					tra->speed = 0;
					break;
				}
			}
		}
		else
		{
			state = RUN;
			tra->status = RUN;
			break;
		}

	default:
		printf("Trans ERROR! Press any key to exit...");
		_getch();
		exit(EXIT_FAILURE);
	}

}

void changeDirection(struct train *tra, struct block rail[][MAX_RAIL_LENGTH], int i)
{
	if (tra->direction == NORMAL)
	{
		if (rail[i][tra->position].direction == NORTH)
			strcpy(tra->directionStr, "east");
		else if (rail[i][tra->position].direction == WEST)
			strcpy(tra->directionStr, "north");
		else if (rail[i][tra->position].direction == SOUTH)
			strcpy(tra->directionStr, "west");
		else if (rail[i][tra->position].direction == EAST)
			strcpy(tra->directionStr, "south");
		else;
	}
	else if (tra->direction == REVERSE)
	{
		if (rail[i][tra->position].direction == NORTH)
			strcpy(tra->directionStr, "west");
		else if (rail[i][tra->position].direction == WEST)
			strcpy(tra->directionStr, "south");
		else if (rail[i][tra->position].direction == SOUTH)
			strcpy(tra->directionStr, "east");
		else if (rail[i][tra->position].direction == EAST)
			strcpy(tra->directionStr, "north");
		else;
	}
}

void changePosition(struct train *tra)
{
	float position;

	if (tra->direction == NORMAL)
		position = tra->position + tra->speed;
	else
		position = tra->position - tra->speed;

	if (tra->direction == NORMAL && position >= tra->railwayLength)
		position = position - tra->railwayLength;
	else if (tra->direction == REVERSE && position < 0)
		position = position + tra->railwayLength;

	tra->position = (int)position;
}

int judgeCommonTrack(struct train *tra, struct block rail[][MAX_RAIL_LENGTH], int i)
{
	unsigned int j, x = 0, y = 0;

	//在探测点判断公共轨道是否有车
	//顺时针
	if (tra->direction == NORMAL)
	{
		//在公共轨道前自动暂停的情况
		if (tra->speed == 0 && tra->status == PAUSE_COMMON)
		{
			//当前秒
			for (j = 0; j < trainNum; j++)
				if (railway[j][train[j].position].common
					== rail[i][tra->position + (int)trainSpeed[i]].common)
					x++;
			//下一秒
			if (x == 0)
				for (j = 0; j < trainNum; j++)
					if (train[j].speed == 0 && train[j].status == PAUSE_COMMON&&strategy != 3)
					{
						if (railway[j][train[j].position + (int)trainSpeed[j]].common
							== rail[i][tra->position + (int)tra->speed].common)
							y++;
						if (j != i)
							m = j;
					}
					else if (railway[j][train[j].position + (int)train[j].speed].common
						== rail[i][tra->position + (int)trainSpeed[i]].common) {
						y++;
						if (j != i)
							m = j;
					}
		}
		//一般情况
		if (rail[i][tra->position + (int)tra->speed].common != 0)
		{
			//当前秒
			for (j = 0; j < trainNum; j++)
				if (railway[j][train[j].position].common
					== rail[i][tra->position + (int)trainSpeed[i]].common)
					x++;
			//下一秒
			if (x == 0)
				for (j = 0; j < trainNum; j++)
					if (train[j].speed == 0 && train[j].status == PAUSE_COMMON&&strategy != 3)
					{
						if (railway[j][train[j].position + (int)trainSpeed[j]].common
							== rail[i][tra->position + (int)tra->speed].common)
							y++;
						if (j != i)
							m = j;
					}
					else if (railway[j][train[j].position + (int)train[j].speed].common
						== rail[i][tra->position + (int)tra->speed].common) {
						y++;
						if (j != i)
							m = j;
					}

		}
	}

	//逆时针
	else if (tra->direction == REVERSE)
	{
		//在公共轨道前自动暂停的情况
		if (tra->speed == 0 && tra->status == PAUSE_COMMON)
		{
			//当前秒
			for (j = 0; j < trainNum; j++)
				if (railway[j][train[j].position].common
					== rail[i][tra->position - (int)trainSpeed[i]].common)
					x++;
			//下一秒
			if (x == 0)
				for (j = 0; j < trainNum; j++)
					if (train[j].speed == 0 && train[j].status == PAUSE_COMMON&&strategy != 3)
					{
						if (railway[j][train[j].position - (int)trainSpeed[j]].common
							== rail[i][tra->position - (int)tra->speed].common)
							y++;
						if (j != i)
							m = j;
					}
					else if (railway[j][train[j].position - (int)train[j].speed].common
						== rail[i][tra->position - (int)tra->speed].common) {
						y++;
						if (j != i)
							m = j;
					}
		}
		//下一秒
		if (rail[i][tra->position + (int)tra->speed].common != 0)
		{
			//当前秒
			for (j = 0; j < trainNum; j++)
				if (railway[j][train[j].position].common
					== rail[i][tra->position - (int)trainSpeed[i]].common)
					x++;
			//下一秒
			if (x == 0)
				for (j = 0; j < trainNum; j++)
					if (train[j].speed == 0 && train[j].status == PAUSE_COMMON&&strategy != 3)
					{
						if (railway[j][train[j].position - (int)trainSpeed[j]].common
							== rail[i][tra->position - (int)tra->speed].common)
							y++;
						if (j != i)
							m = j;
					}
					else if (railway[j][train[j].position - (int)train[j].speed].common
						== rail[i][tra->position - (int)trainSpeed[i]].common) {
						y++;
						if (j != i)
							m = j;
					}

		}
	}

	if (x >= 1)//表示公共轨道有车
		return 1;
	else if (y > 1)//表示同时有两辆车申请进入公共轨道
		return 2;
	else
		return 0;
}
