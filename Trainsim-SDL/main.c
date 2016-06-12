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

//#define DEBUG_TRANS       //���Կ���
#define FALSE_THREAD        //α�߳̿���

int trainNum;                                 //������
int railNum;                                  //�������
struct train train[MAX_TRAIN];                         //������
float        trainSpeed[MAX_TRAIN];                    //���ٶ�
struct block railway[MAX_RAIL][MAX_RAIL_LENGTH];       //����볤������

int strategy = 0; //1��2��3�ֱ��Ӧ"�������"��"�쳵���Ȳ���"��"�˹�����"
int inputMode = 0; //1��2�ֱ��Ӧ"���ļ���������"��"�Ӽ��̶�������"
unsigned int processTime = 0; //����ʱ��
unsigned int commandTime = 0;
char         command;

FILE *logPtr = NULL; //��־�ļ�ָ��
FILE *commandPtr = NULL; //�����ļ�ָ��
FILE *outPtr = NULL; //����ļ�ָ��

int main(void)
{
	//��ʼ���𳵺͹��
	init();

	while (1)
	{
#ifdef FALSE_THREAD
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
#endif

#ifdef DEBUG_TRANS
		srand((unsigned)time(NULL));
		for (int i = 0; i < 3; ++i)
			train[i].position = rand() % 10;
#else
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
#endif
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
		Sleep(1000);
	}
	return 0;
}
