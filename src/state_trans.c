#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>

#include "var.h"
#include "state_trans.h"

extern unsigned int trainNum;
extern struct train trains[MAX_TRAIN];
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
				if (rail[i][tra->position + (int)pow(-1, trains[j].direction + 1)* j].station != 0)
					sum++;
				if (sum > 0)
					break;
			}
			if (sum > 0) {
				if (tra->pausetime != 0) {
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
							rail[i][tra->position + (int)tra->speed].last = -1;
							flag = 0;
							break;
						}
					}
				}
				else {
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
							rail[i][tra->position - (int)tra->speed].last = i;
							tra->status = RUN;
							flag = rail[i][tra->position - (int)tra->speed].common;
							break;
						}
						else if (flag == rail[i][tra->position - (int)tra->speed].common) {
							state = STOP;
							tra->speed = 0;
							tra->status = PAUSE_COMMON;
							flag = 0;
							break;
						}
					}
				}
		}
			else if (strategy == FAST_FIRST) {
				if (tra->type < trains[m].type) {
					state = RUN;
					break;
				}
				else if (tra->type > trains[m].type) {
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
	int pos=tra->position,posi;

	//在探测点判断公共轨道是否有车
	//顺时针
	if (tra->direction == NORMAL)
	{
		//在公共轨道前自动暂停的情况
		if (tra->speed == 0 && tra->status == PAUSE_COMMON)
		{
			//当前秒
			pos += (int)trainSpeed[i];
			if(pos >= tra->railwayLength)
				pos = pos+ tra->railwayLength;
			for (j = 0; j < trainNum; j++)
				if (railway[j][trains[j].position].common
					== rail[i][pos].common)
					x++;
			//下一秒
			if (x == 0)
				for (j = 0; j < trainNum; j++)
					if (trains[j].speed == 0 && trains[j].status == PAUSE_COMMON&&strategy != 3)
					{
						if (trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trainSpeed[j] > 0 && trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trainSpeed[j] < trains[j].railwayLength)
							posi = trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trainSpeed[j];
						else if (trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trainSpeed[j] >= trains[j].railwayLength)
							posi = trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trainSpeed[j] - trains[j].railwayLength;
						else
							posi = trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trainSpeed[j] + trains[j].railwayLength;
						if (railway[j][posi].common
							== rail[i][pos].common)
							y++;
						if (j != i)
							m = j;
					}
					else {
						if (trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trains[j].speed > 0 && trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trains[j].speed < trains[j].railwayLength)
							posi = trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trains[j].speed;
						else if (trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trains[j].speed >= trains[j].railwayLength)
							posi = trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trains[j].speed - trains[j].railwayLength;
						else
							posi = trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trains[j].speed + trains[j].railwayLength;
						if (railway[j][posi].common
						== rail[i][pos].common) {
						y++;
						if (j != i)
							m = j;
					    }
					}
		}
		//一般情况
		if (rail[i][tra->position + (int)tra->speed].common != 0)
		{
			//当前秒
			pos +=(int)tra->speed;
			if (pos >= tra->railwayLength)
				pos = pos - tra->railwayLength;
			for (j = 0; j < trainNum; j++)
				if (railway[j][trains[j].position].common
					== rail[i][pos].common)
					x++;
			//下一秒
			if (x == 0)
				for (j = 0; j < trainNum; j++)
					if (trains[j].speed == 0 && trains[j].status == PAUSE_COMMON&&strategy != 3)
					{
						if (trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trainSpeed[j] > 0 && trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trainSpeed[j] < trains[j].railwayLength)
							posi = trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trainSpeed[j];
						else if (trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trainSpeed[j] >= trains[j].railwayLength)
							posi = trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trainSpeed[j] - trains[j].railwayLength;
						else
							posi = trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trainSpeed[j] + trains[j].railwayLength;
						if (railway[j][posi].common
							== rail[i][pos].common)
							y++;
						if (j != i)
							m = j;
					}
					else {
						if (trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trains[j].speed > 0 && trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trains[j].speed < trains[j].railwayLength)
							posi = trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trains[j].speed;
						else if (trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trains[j].speed >= trains[j].railwayLength)
							posi = trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trains[j].speed - trains[j].railwayLength;
						else
							posi = trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trains[j].speed + trains[j].railwayLength;
						if (railway[j][posi].common
						== rail[i][pos].common) {
						y++;
						if (j != i)
							m = j;
					    }
					}

		}
	}

	//逆时针
	else if (tra->direction == REVERSE)
	{
		//在公共轨道前自动暂停的情况
		if (tra->speed == 0 && tra->status == PAUSE_COMMON)
		{
			pos -= (int)trainSpeed[i];
			if (pos <0)
				pos = pos + tra->railwayLength+1;
			//当前秒
			for (j = 0; j < trainNum; j++)
				if (railway[j][trains[j].position].common
					== rail[i][pos].common)
					x++;
			//下一秒
			if (x == 0)
				for (j = 0; j < trainNum; j++)
					if (trains[j].speed == 0 && trains[j].status == PAUSE_COMMON&&strategy != 3)
					{   
						if (trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trainSpeed[j] > 0 && trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trainSpeed[j] < trains[j].railwayLength)
							posi = trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trainSpeed[j];
						else if (trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trainSpeed[j] >= trains[j].railwayLength)
							posi = trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trainSpeed[j] - trains[j].railwayLength;
						else
							posi = trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trainSpeed[j] + trains[j].railwayLength ;
						if (railway[j][posi].common
							== rail[i][pos].common)
							y++;
						if (j != i)
							m = j;
					}
					else {
						if (trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trains[j].speed > 0 && trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trains[j].speed < trains[j].railwayLength)
							posi = trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trains[j].speed;
						else if (trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trains[j].speed >= trains[j].railwayLength)
							posi = trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trains[j].speed - trains[j].railwayLength;
						else
							posi = trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trains[j].speed + trains[j].railwayLength;
						if (railway[j][trains[j].position - (int)trains[j].speed].common
						== rail[i][pos].common) {
						y++;
						if (j != i)
							m = j;
					    }
					}
		}
		//下一秒
		if (rail[i][tra->position - (int)tra->speed].common != 0)
		{
			pos -= (int)tra->speed;
			if (pos<0)
				pos = pos + tra->railwayLength+1;
			//当前秒
			for (j = 0; j < trainNum; j++)
				if (railway[j][trains[j].position].common
					== rail[i][pos].common)
					x++;
			//下一秒
			if (x == 0)
				for (j = 0; j < trainNum; j++)
					if (trains[j].speed == 0 && trains[j].status == PAUSE_COMMON&&strategy != 3)
					{
						if (trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trainSpeed[j] > 0 && trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trainSpeed[j] < trains[j].railwayLength)
							posi = trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trainSpeed[j];
						else if (trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trainSpeed[j] >= trains[j].railwayLength)
							posi = trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trainSpeed[j] - trains[j].railwayLength;
						else
							posi = trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trainSpeed[j] + trains[j].railwayLength;
						if (railway[j][posi].common
							== rail[i][pos].common)
							y++;
						if (j != i)
							m = j;
					}
					else{
						if (trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trains[j].speed > 0 && trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trains[j].speed < trains[j].railwayLength)
							posi = trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trains[j].speed;
						else if (trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trains[j].speed >= trains[j].railwayLength)
							posi = trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trains[j].speed - trains[j].railwayLength;
						else
							posi = trains[j].position + (int)pow(-1, trains[j].direction + 1)* (int)trains[j].speed + trains[j].railwayLength;
						if (railway[j][posi].common
						== rail[i][pos].common) {
						y++;
						if (j != i)
							m = j;
					    }
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
