#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include "input.h"
#include "var.h"

#define SLEEP_TIME_MS 1000

extern int trainNum;
extern int railNum;
extern struct train train[MAX_TRAIN];
extern float        trainSpeed[MAX_TRAIN];
extern struct block railway[MAX_RAIL][MAX_RAIL_LENGTH];

extern int strategy;
extern int inputMode;

void errorFromFile()
{
	/*文件输入错误*/
	system("cls");
	printf("ERROR data detected! Please check init.txt.\n"
		"Press any key to exit...");
	_getch();
	exit(EXIT_FAILURE);
};

void errorFromKeyboard()
{
	/*键盘输入错误*/
	printf("ERROR input detected! Press any key to retry.");
	_getch();
};

void init()
{
	printf(
		"             === Trainsim by C403 ===               ;###'                       \n"
		"             |   Jiannan Zheng      |               ,#+++.                      \n"
		"             |   Awesome Yuan       |                #+++                       \n"
		"             |    Joxon Chan        |                #+++`                      \n"
		"             =============================================:                     \n"
		"                   '++'    |Press any key to continue....| `                    \n"
		"                   @+#+    ==============================;::';'++#+             \n"
		"                   '+++                           ,;;:;;+';:,;';'''++           \n"
		"                   ++++                         ,::,,,,,,,;::;;;'::'''          \n"
		"                   ++++                        ,';:::,,,'',::';':'';';          \n"
		"                   #+#+   ::;+#+++++##+`      `'';;:::::;::,;';';;;'';          \n"
		"                   +#+;++;;:. '+''''''''#+    ;;+#+;:,:'+'#:,;'':;;:';          \n"
		"                ,,;::;+#'''';`:;#'''''''::+# ;:',.:+:,;:...+,:;'';:';;`         \n"
		"               :;,,,,;,:++'''';;;'''''+'''''+::#.`.+:,;@...;,,'';'';;;`         \n"
		"              :::::::::,,#++''';;;;;;+'+;';'+::@;,,':,:@,,,'.,+''''':;`         \n"
		"             ::+++::+#+:,:++''';;;;;;;':;:+++;,#::;:,,,'::::,,++''''::          \n"
		"             :+,`;:;'.`+,:+++''';;;;;;''+++++;:,:,;;:,:,:,,,,:++'''':;          \n"
		"             :',,#,:;,@@,::##''';;;;;;'+++'++';:::;++'';::::::#+'''':;          \n"
		"            ;:;::;,:;;::,::##'''';'';;;#+''++''';;';++:';;;;;;#+''''::          \n"
		"            ;:,,,;;;:,,,:;;##+'''''+;''##'+++'++++;::,,,'+'';++++''';:          \n"
		"            ;;::;+#;;;:::;;##++''''#'''##++++'++''++++++;;'';##++''';:          \n"
		"             ';;:::::;;;''+##++'''+#'''##++++++'''';';:;;;;'++#+++++;:          \n"
		"             +'';::::''''+###++++###+++##++++++++''+++';;;'###;;;;;;':          \n"
		"             .+;++++';''+####++'##########++'+++++';;::;'+####;;;;;;;:          \n"
		"              +''''+;;'+#@###+++######+####''''+#####+##@#####;;;;;;;;          \n"
		"          `    ++';;'+##@@##+'+@@#####+####''''+##+##@@###+##+;;;;;;;;          \n"
		"         ;` +@@#++###@@#####+'+'+'####+##@. +'''##+++++#+++##+;;;;;;;;'         \n"
		"         +;`:##@+++#####@#+++'''''####+##:+` '''+###+++++#+##'';;;;;;;'`        \n");
	_getch();
	char ch;
	/*初始化火车和轨道*/
initRetry:
	system("cls");
	printf("Initialization type:\n"
		"1:initialize from file.\n"
		"2:initialize from keyboard.\n"
		"q:exit.\n"
		"Input your choice:");
	ch = _getche();
	switch (ch)
	{
	case '1':
		initFromFile();
		Sleep(SLEEP_TIME_MS);
		break;
	case '2':
		system("cls");
		initFromKeyBoard();
		break;
	case 'q':
		system("cls");
		printf("Exiting...");
		Sleep(SLEEP_TIME_MS);
		exit(EXIT_SUCCESS);
	default:
		system("cls");
		printf("Wrong init mode! Please retry...\n");
		Sleep(SLEEP_TIME_MS);
		goto initRetry;
	}

	/*选择命令输入方式*/
inputRetry:
	system("cls");
	printf("Commands type:\n"
		"1:Commands from file.\n"
		"2:Commands from keyboard.\n"
		"q:exit.\n"
		"Input your choice:");
	ch = _getche();
	switch (ch)
	{
	case '1':
		printf("\nYou have selected to input commands from file.\n");
		inputMode = FROM_FILE;
		Sleep(SLEEP_TIME_MS);
		break;
	case '2':
		printf("\nYou have selected to input commands from keyboard.\n");
		inputMode = FROM_KEYBOARD;
		Sleep(SLEEP_TIME_MS);
		break;
	case 'q':
		system("cls");
		printf("Exiting...");
		Sleep(SLEEP_TIME_MS);
		exit(EXIT_SUCCESS);
	default:
		printf("\nWrong input mode! Please retry...");
		Sleep(SLEEP_TIME_MS);
		goto inputRetry;
	}
	system("cls");
}

static void initFromFile()
{
	FILE *fp;
	int id;
	char ch;

	//调试时打开文件不成功，请修改项目->属性->调试->工作目录
	fp = fopen(".\\txt\\init.txt", "r");
	if (fp != NULL)
	{
		/*策略选择*/
		fscanf(fp, "strategy=%d\n", &strategy);
		//if (strategy != ALTERNATIVE ||
		//	strategy != FAST_FIRST ||
		//	strategy != MANUAL)
		//	errorFromFile();

		/*火车初始化*/
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


		/*轨道初始化*/
		fscanf(fp, "railway.num=%d\n", &railNum);
		//if (railNum < 0) errorFromFile();
		for (id = 0; id < railNum; ++id)
		{
			/*轨道初始化：长宽设定*/
			int length, southwest, northwest, northeast, southeast;
			fscanf(fp, "railway%c.len=%d sw=%d nw=%d ne=%d se=%d\n",
				&ch, &length, &southwest, &northwest, &northeast, &southeast);
			//if (length < 0 || southeast < 0 || northeast < 0 || northwest < 0 || southeast < 0) errorFromFile();
			train[id].railwayLength = length;
			railway[id][southwest].direction = SOUTHWEST;
			railway[id][northwest].direction = NORTHWEST;
			railway[id][northeast].direction = NORTHEAST;
			railway[id][southeast].direction = SOUTHEAST;
			railway[id][length].direction = SOUTHEAST;
			int blockid = 0;
			for (blockid = southwest + 1; blockid < northwest; ++blockid)
			{
				railway[id][blockid].direction = WEST;
				railway[id][blockid].station = 0;
			}
			for (blockid = northwest + 1; blockid < northeast; ++blockid)
			{
				railway[id][blockid].direction = NORTH;
				railway[id][blockid].station = 0;
			}
			for (blockid = northeast + 1; blockid < southeast; ++blockid)
			{
				railway[id][blockid].direction = EAST;
				railway[id][blockid].station = 0;
			}
			for (blockid = southeast + 1; blockid < length; ++blockid)
			{
				railway[id][blockid].direction = SOUTH;
				railway[id][blockid].station = 0;
			}

			/*轨道初始化：公共轨道*/
			int common_count, common_ID, start, end;
			fscanf(fp, "cm=%d\n", &common_count);
			//if (common_count < 0) errorFromFile();
			int commomid = 1;
			while (commomid++ <= common_count)
			{
				fscanf(fp, "id=%d %d %d\n", &common_ID, &start, &end);
				//if (common_count <= 0 || start < 0 || end < 0) errorFromFile();
				for (blockid = start; blockid <= end; ++blockid)
				{
					if (blockid == length)
					{
						railway[id][0].common = common_ID;
						railway[id][0].last = -1;
					}
					else
					{
						railway[id][blockid].common = common_ID;
						railway[id][blockid].last = -1;
					}
				}
			}

			/*轨道初始化：停靠点*/
			int stationCount, stationID = 1, stationPoint;
			fscanf(fp, "sn=%d\n", &stationCount);
			//if (stationCount < 0) errorFromFile();
			while (stationID++ <= stationCount)
			{
				fscanf(fp, "%d\n", &stationPoint);
				//if (stationPoint < 0) errorFromFile();
				railway[id][stationPoint].station = 1;
			}

			//复制轨道起始部分，防止越界访问，i的范围视速度而定
			for (int i = 0; i < 10; ++i)
				railway[id][length + i] = railway[id][i];
		}
		fclose(fp);
		printf("\ninit successful!");
	}

	else
	{
		printf("\nCannot open init.txt. Press any key to exit...");
		_getch();
		exit(EXIT_FAILURE);
	}
}

static void initFromKeyBoard()
{
	/*策略选择*/
strategyRetry:
	system("cls");
	printf("SELECTING STRATEGY...\n"
		"Which strategy do you like?\n"
		"1:Alternative mode\n"
		"2:Fast-first mode\n"
		"3:Manual mode\n"
		"(int > 0):");
	scanf("%d", &strategy);
	fflush(stdin);
	if (strategy < ALTERNATIVE || strategy > MANUAL)
	{
		errorFromKeyboard();
		goto strategyRetry;
	}
	printf("DONE...");
	Sleep(SLEEP_TIME_MS);

	/*火车初始化*/
	int id;
trainRetry:
	system("cls");
	printf("INITIALIZING TRAINS...\n"
		"How many trains do we have?\n"
		"(int > 0):");
	scanf("%d", &trainNum);
	fflush(stdin);
	if (trainNum <= 0)
	{
		errorFromKeyboard();
		goto trainRetry;
	}

	for (id = 0; id < trainNum; ++id)
	{
		printf(
			"Please input train%c's\n"
			"speed (float => 0),\n"
			"start time (int => 0),\n"
			"start point (int => 0),\n"
			"direction (1 for normal or 2 for rev),\n"
			"type(1 for fast, 2 for slow),\n"
			"pausetime (int => 0),\n"
			"separated by spaces:\n",
			'A' + id);
		scanf("%f %d %d %d %d %d",
			&train[id].speed,
			&train[id].startTime,
			&train[id].startPoint,
			&train[id].direction,
			&train[id].type,
			&train[id].pausetime);
		fflush(stdin);

		if (train[id].speed < 0 ||
			train[id].startTime < 0 ||
			train[id].startPoint < 0 ||
			train[id].direction < 1 ||
			train[id].direction > 2 ||
			train[id].type < 1 ||
			train[id].type > 2 ||
			train[id].pausetime < 0)
		{
			errorFromKeyboard();
			goto trainRetry;
		}

		trainSpeed[id] = train[id].speed;
		printf("DONE...\n");
	}


	/*轨道初始化*/
railwayRetry:
	system("cls");
	printf("INITIALIZING RAILWAYS...\n"
		"How many railways do we have?\n"
		"(int > 0):");
	scanf("%d", &railNum);
	fflush(stdin);
	if (railNum < 0)
	{
		errorFromKeyboard();
		goto railwayRetry;
	}

	for (id = 0; id < railNum; ++id)
	{
		int length, southwest, northwest, northeast, southeast;
		int common_count, common_ID, start, end;
		/*轨道初始化：长宽设定*/
		printf("Please input railway%c's length (int > 0),\n"
			"SW point(int => 0),\n"
			"NW point(int > 0),\n"
			"NE point(int > 0),\n"
			"SE point(int > 0),\n"
			"separated by spaces:\n", 'A' + id);
		scanf("%d %d %d %d %d",
			&length, &southwest, &northwest, &northeast, &southeast);
		fflush(stdin);
		if (length < 0 || southeast < 0 || northeast < 0 || northwest < 0 || southeast < 0)
		{
			errorFromKeyboard();
			goto railwayRetry;
		}

		train[id].railwayLength = length;
		railway[id][southwest].direction = SOUTHWEST;
		railway[id][northwest].direction = NORTHWEST;
		railway[id][northeast].direction = NORTHEAST;
		railway[id][southeast].direction = SOUTHEAST;
		railway[id][length].direction = SOUTHEAST;
		int blockid = 0;
		for (blockid = southwest + 1; blockid < northwest; ++blockid)
		{
			railway[id][blockid].direction = WEST;
			railway[id][blockid].station = 0;
		}
		for (blockid = northwest + 1; blockid < northeast; ++blockid)
		{
			railway[id][blockid].direction = NORTH;
			railway[id][blockid].station = 0;
		}
		for (blockid = northeast + 1; blockid < southeast; ++blockid)
		{
			railway[id][blockid].direction = EAST;
			railway[id][blockid].station = 0;
		}
		for (blockid = southeast + 1; blockid < length; ++blockid)
		{
			railway[id][blockid].direction = SOUTH;
			railway[id][blockid].station = 0;
		}
		printf("DONE...\n");

		/*轨道初始化：公共轨道*/
		printf("How many common parts does the railway%c have?\n"
			"(int => 0):", 'A' + id);
		scanf("%d", &common_count);
		fflush(stdin);
		if (common_count < 0)
		{
			errorFromKeyboard();
			goto railwayRetry;
		}

		printf("Please input railway%c's"
			"common id (int > 0),\n"
			"start point (int => 0),\n"
			"end point (int => 0),\n"
			"separated by spaces in one line:\n", 'A' + id);
		int commomid = 1;
		while (commomid++ <= common_count)
		{
			scanf("%d %d %d", &common_ID, &start, &end);
			if (common_count <= 0 || start < 0 || end < 0)
			{
				errorFromKeyboard();
				goto railwayRetry;
			}

			for (blockid = start; blockid <= end; ++blockid)
			{
				if (blockid == length)
				{
					railway[id][0].common = common_ID;
					railway[id][0].last = -1;
				}
				else
				{
					railway[id][blockid].common = common_ID;
					railway[id][blockid].last = -1;
				}
				printf("DONE...\n");
			}

			/*轨道初始化：停靠点*/
			int stationCount, stationID = 1, stationPoint;
			printf("How many stations does the railway%c have?\n"
				"(int => 0):", 'A' + id);
			scanf("%d", &stationCount);
			fflush(stdin);
			if (stationCount < 0)
			{
				errorFromKeyboard();
				goto railwayRetry;
			}

			printf("Please input railway%c's"
				"station point (int => 0),\n"
				"separated by spaces in one line:\n", 'A' + id);
			while (stationID++ <= stationCount)
			{
				scanf("%d", &stationPoint);
				if (stationPoint < 0)
				{
					errorFromKeyboard();
					goto railwayRetry;
				}
				railway[id][stationPoint].station = 1;
			}


			//复制轨道起始部分，防止越界访问，i的范围视速度而定
			for (int i = 0; i < 10; ++i)
				railway[id][length + i] = railway[id][i];
		}

		printf("init successful!");
		Sleep(SLEEP_TIME_MS);
		system("cls");
	}
}

