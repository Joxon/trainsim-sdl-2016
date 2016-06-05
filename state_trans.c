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
extern int inputMode;
static int   clock=0 ,      flag = 0;
int m,sum=0;
static char ch;


void trans(struct train *tra, struct block rail[][MAX_RAIL_LENGTH], int i)
{
    int state = STOP,j;
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
				state = PAUSE;
				if (tra->speed != 0) {
					tra->position += j;
					tra->status = PAUSE;
					trainSpeed[i] = tra->speed;
					tra->speed = 0;
				}
			}
				
			else 
                state = RUN;
            
		}
	case PAUSE:
		clock++;
		if (clock == tra->pausetime) {
			tra->speed = trainSpeed[i];
			tra->status = RUN;
			state = RUN;
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
			if (inputMode == 1) {
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
			else if (inputMode == 2) {
				if (tra->speed>train[m].speed) {
					state = RUN;
					break;
				}
			}
			else if (inputMode == 3) {
				printf("Please choose which one to run(%c or %c):", 'A' + i, 'A' + m);
				if ((ch = getchar()) == 'A' + 'i') {
					state = RUN;
					tra->status = PAUSE_COMMON;
					break;
				}
				else {
					state = STOP;
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
            strcpy(tra->dir, "east");
        else if (rail[i][tra->position].direction == WEST)
            strcpy(tra->dir, "north");
        else if (rail[i][tra->position].direction == SOUTH)
            strcpy(tra->dir, "west");
        else if (rail[i][tra->position].direction == EAST)
            strcpy(tra->dir, "south");
        else;
    }
    else if (tra->direction == REVERSE)
    {
        if (rail[i][tra->position].direction == NORTH)
            strcpy(tra->dir, "west");
        else if (rail[i][tra->position].direction == WEST)
            strcpy(tra->dir, "south");
        else if (rail[i][tra->position].direction == SOUTH)
            strcpy(tra->dir, "east");
        else if (rail[i][tra->position].direction == EAST)
            strcpy(tra->dir, "north");
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
    
    //��̽����жϹ�������Ƿ��г�
    //˳ʱ��
    if (tra->direction == NORMAL)
    {
        //�ڹ������ǰ�Զ���ͣ�����
        if (tra->speed == 0 && tra->status == PAUSE_COMMON)
        {
            //��ǰ��
            for (j = 0; j < trainNum; j++)
                if (railway[j][train[j].position].common
                    == rail[i][tra->position + (int)trainSpeed[i]].common)
                    x++;
            //��һ��
            if (x == 0)
                for (j = 0; j < trainNum; j++)
                    if (train[j].speed == 0 && train[j].status == PAUSE_COMMON)
                    {
                        if (railway[j][train[j].position + (int)trainSpeed[j]].common
                            == rail[i][tra->position + (int)tra->speed].common)
                            y++;
                    }
                    else if (railway[j][train[j].position + (int)train[j].speed].common
                        == rail[i][tra->position + (int)trainSpeed[i]].common)
                        y++;
        }
        //һ�����
        if (rail[i][tra->position + (int)tra->speed].common != 0)
        {
            //��ǰ��
            for (j = 0; j < trainNum; j++)
                if (railway[j][train[j].position].common
                    == rail[i][tra->position + (int)trainSpeed[i]].common)
                    x++;
            //��һ��
            if (x == 0)
                for (j = 0; j < trainNum; j++)
                    if (train[j].speed == 0 && train[j].status == PAUSE_COMMON)
                    {
                        if (railway[j][train[j].position + (int)trainSpeed[j]].common
                            == rail[i][tra->position + (int)tra->speed].common)
                            y++;
                    }
                    else if (railway[j][train[j].position + (int)train[j].speed].common
                        == rail[i][tra->position + (int)tra->speed].common)
                        y++;

        }
    }

    //��ʱ��
    else if (tra->direction == REVERSE)
    {
        //�ڹ������ǰ�Զ���ͣ�����
        if (tra->speed == 0 && tra->status == PAUSE_COMMON)
        {
            //��ǰ��
            for (j = 0; j < trainNum; j++)
                if (railway[j][train[j].position].common
                    == rail[i][tra->position - (int)trainSpeed[i]].common)
                    x++;
            //��һ��
            if (x == 0)
                for (j = 0; j < trainNum; j++)
                    if (train[j].speed == 0 && train[j].status == PAUSE_COMMON)
                    {
                        if (railway[j][train[j].position - (int)trainSpeed[j]].common
                            == rail[i][tra->position - (int)tra->speed].common)
                            y++;
                    }
                    else if (railway[j][train[j].position - (int)train[j].speed].common
                        == rail[i][tra->position - (int)tra->speed].common)
                        y++;
        }
        //��һ��
        if (rail[i][tra->position + (int)tra->speed].common != 0)
        {
            //��ǰ��
            for (j = 0; j < trainNum; j++)
                if (railway[j][train[j].position].common
                    == rail[i][tra->position - (int)trainSpeed[i]].common)
                    x++;
            //��һ��
            if (x == 0)
                for (j = 0; j < trainNum; j++)
                    if (train[j].speed == 0 && train[j].status == PAUSE_COMMON)
                    {
                        if (railway[j][train[j].position - (int)trainSpeed[j]].common
                            == rail[i][tra->position - (int)tra->speed].common)
                            y++;
                    }
                    else if (railway[j][train[j].position - (int)train[j].speed].common
                        == rail[i][tra->position - (int)trainSpeed[i]].common)
                        y++;

        }
    }

    if (x >= 1)//��ʾ��������г�
        return 1;
    else if (y > 1)//��ʾͬʱ��������������빫�����
        return 2;
    else
        return 0;
}
