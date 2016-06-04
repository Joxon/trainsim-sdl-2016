#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include "input.h"
#include "var.h"

#define SLEEP_TIME_MS 1000

extern unsigned int trainNum;
extern unsigned int railNum;
extern struct train train[MAX_TRAIN];
extern float        trainSpeed[MAX_TRAIN];
extern struct block railway[MAX_RAIL][MAX_RAIL_LENGTH];

extern unsigned short strategy;
extern unsigned short inputMode;

void init()
{
    printf("             === Trainsim by C403 ===               ;###'                       \n"
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
            printf("\nWrong init mode! Please retry...\n");
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
    unsigned int  id;
    char ch;
    int length, southwest, northwest, northeast, southeast;
    int common_count, common_ID, start, end;

    if ((fp = fopen(".\\txt\\init.txt", "r")) != NULL)
    {
        /*火车初始化*/
        fscanf(fp, "train.num=%d\n", &trainNum);
        for (id = 0; id < trainNum; ++id)
        {
            fscanf(fp, "train%c.speed=%f st=%d sp=%d dir=%d type=%d\n",
                   &ch, &trainSpeed[id], &train[id].startTime, &train[id].startPoint,
                   &train[id].direction, &train[id].type);
            train[id].position = train[id].startPoint;
            train[id].status   = WAIT;
        }

        /*轨道初始化*/
        fscanf(fp, "railway.num=%d\n", &railNum);
        for (id = 0; id < railNum; ++id)
        {
            /*轨道初始化：长宽设定*/
            fscanf(fp, "railway%c.len=%d sw=%d nw=%d ne=%d se=%d\n",
                   &ch, &length, &southwest, &northwest, &northeast,
                   &southeast);
            train[id].railwayLength = length;
            int blockid             = 0;
            while (blockid <= northwest)
            {
                railway[id][blockid].direction = WEST;
                ++blockid;
            }
            while (blockid <= northeast)
            {
                railway[id][blockid].direction = NORTH;
                ++blockid;
            }
            while (blockid <= southeast)
            {
                railway[id][blockid].direction = EAST;
                ++blockid;
            }
            while (blockid < length)
            {
                railway[id][blockid].direction = SOUTH;
                ++blockid;
            }

            /*轨道初始化：公共轨道*/
            fscanf(fp, "cm=%d\n", &common_count);
            int commomid = 1;
            while (commomid++ <= common_count)
            {
                fscanf(fp, "id=%d %d %d\n", &common_ID, &start, &end);
                for (blockid = start; blockid < end; ++blockid)
                {
                    railway[id][blockid].common = common_ID;
                    railway[id][blockid].last   = -1;
                }
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
    unsigned int id;
    /*火车初始化*/
    printf("INITIALIZING TRAINS...\n"
           "How many trains do we have?\n"
           "(int > 0):");
    scanf("%d", &trainNum);
    fflush(stdin);
    for (id = 0; id < trainNum; ++id)
    {
        printf(
            "Please input train%c's\n"
            "speed (float => 0),\n"
            "start time (int => 0),\n"
            "start point (int => 0),\n"
            "direction (1 for normal or 2 for rev),\n"
            "type(1 for fast or 2 slow),\n"
            "separated by spaces:\n",
            'A' + id);
        scanf("%f %d %d %d %d",
              &train[id].speed, &train[id].startTime, &train[id].startPoint, &train[id].direction, &train[id].type);
        trainSpeed[id] = train[id].speed;
        fflush(stdin);
        printf("DONE...\n");
    }

    /*轨道初始化*/
    system("cls");
    printf("INITIALIZING RAILWAYS...\n"
           "How many railways do we have?\n"
           "(int > 0):");
    scanf("%d", &railNum);
    fflush(stdin);
    int length, southwest, northwest, northeast, southeast;
    int common_count, common_ID, start, end;
    for (id = 0; id < railNum; ++id)
    {
        /*轨道初始化：长宽设定*/
        printf("Please input railway%c's length (int > 0),\n"
               "SW point(int => 0),\n"
               "NW point(int => 0),\n"
               "NE point(int => 0),\n"
               "SE point(int => 0),\n"
               "separated by spaces:\n", 'A' + id);
        scanf("%d %d %d %d %d",
              &length, &southwest, &northwest, &northeast, &southeast);
        train[id].railwayLength = length;
        fflush(stdin);
        int blockid = 0;
        while (blockid <= northwest)
        {
            railway[id][blockid].direction = WEST;
            ++blockid;
        }
        while (blockid <= northeast)
        {
            railway[id][blockid].direction = NORTH;
            ++blockid;
        }
        while (blockid <= southeast)
        {
            railway[id][blockid].direction = EAST;
            ++blockid;
        }
        while (blockid < length)
        {
            railway[id][blockid].direction = SOUTH;
            ++blockid;
        }
        printf("DONE...\n");
        /*轨道初始化：公共轨道*/
        printf("How many common parts does the railway%c have?\n"
               "(int > 0):", 'A' + id);
        scanf("%d", &common_count);
        fflush(stdin);
        printf("Please input railway%c's"
               "common id (int > 0),\n"
               "start point (int => 0),\n"
               "end point (int => 0),\n"
               "separated by spaces in one line:\n", 'A' + id);
        int commomid = 1;
        while (commomid++ <= common_count)
        {
            scanf("%d %d %d", &common_ID, &start, &end);
            for (blockid = start; blockid <= end; ++blockid)
            {
                railway[id][blockid].common = common_ID;
                railway[id][blockid].last = -1;
            }
            printf("DONE...\n");
        }
    }

    printf("init successful!");
    Sleep(SLEEP_TIME_MS);
    system("cls");
}
