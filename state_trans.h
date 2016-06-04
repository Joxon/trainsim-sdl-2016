#ifndef TRAIN_SIMULATION_STATE_TRANS_H
#define TRAIN_SIMULATION_STATE_TRANS_H

#include "var.h"

void trans(struct train *tra, struct block rail[][MAX_RAIL_LENGTH], int i);
void changeDirection(struct train *tra, struct block rail[][MAX_RAIL_LENGTH], int i);
void changePosition(struct train *tra);
int  judgeCommonTrack(struct train *tra, struct block rail[][MAX_RAIL_LENGTH], int i);

#endif //TRAIN_SIMULATION_STATE_TRANS_H
