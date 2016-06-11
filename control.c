#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <process.h>
#include "control.h"
#include "var.h"

extern float        trainSpeed[MAX_TRAIN];
extern struct train train[MAX_TRAIN];
extern FILE         *commandPtr;
extern unsigned int processTime;
extern unsigned int commandTime;
extern unsigned int trainNum;

void getInputFromFile()
{
	if (commandPtr == NULL)
	{
		commandPtr = fopen(".\\txt\\command.txt", "r");

		if (commandPtr == NULL)
		{
			printf("command file cannot be opened! Exiting...");
			exit(EXIT_FAILURE);
		}
		goto read;
	}
	else
	{
	read:
		fscanf(commandPtr, "T=%d,", &commandTime);
		if (commandTime != processTime);
		else
		{
			char commandStr[20];
			fgets(commandStr, 20, commandPtr);
			handleCommand(commandStr);
		}
	}
}

void getInputFromKeyboard()
{
	char commandStr[20];
	printf("\nPlease input your command:");
	fgets(commandStr, 20, stdin);
	handleCommand(commandStr);
}

void handleCommand(char * commandStr)
{
	unsigned int commandElement, commandElementFw, trainID, speedElement;
	unsigned int commandLen = strlen(commandStr);
	char tempSpeed[MAX_TRAIN][10] = { {'\0'} };

	//把字母后的数字存到字符串中
	for (commandElementFw = 0, commandElement = 0; commandElement < commandLen; ++commandElement)
	{
		for (trainID = 0; trainID < trainNum; ++trainID)
			if (commandStr[commandElement] == 'A' + trainID)
				for (commandElementFw = commandElement + 1, speedElement = 0;
					commandStr[commandElementFw] != ' ' && commandElementFw < commandLen;
					++commandElementFw, ++speedElement)
					tempSpeed[trainID][speedElement] = commandStr[commandElementFw];
		commandElement = commandElementFw;
	}

	//把字符串转化为数字并赋给火车
	for (trainID = 0; trainID < trainNum; ++trainID)
		if (strlen(tempSpeed[trainID]) > 0)
		{
			if (atoi(tempSpeed[trainID]) == -1)
			{
				if (train[trainID].direction == NORMAL)
					train[trainID].direction = REVERSE;
				else if (train[trainID].direction == REVERSE)
					train[trainID].direction = NORMAL;
				continue;
			}

			train[trainID].speed = (float)atof(tempSpeed[trainID]);
			trainSpeed[trainID] = (float)atof(tempSpeed[trainID]);

			if (train[trainID].speed == 0)
				train[trainID].status = PAUSE_ANY;
		}
}