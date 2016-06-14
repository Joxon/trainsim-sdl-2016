#ifndef TRAIN_SIMULATION_OUTPUT_H
#define TRAIN_SIMULATION_OUTPUT_H

void printConsole();

static void printPerSec();
static void printTrans();

static int logprintf(const char *format, ...); //将周期输出同时输出到屏幕和log文件
static int outprintf(const char *format, ...); //将变化输出同时输出到屏幕和output文件

#endif //TRAIN_SIMULATION_OUTPUT_H
