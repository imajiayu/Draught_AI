#pragma once
#ifndef __AI_DRAUGHT__H__
#define __AI_DRAUGHT__H__

// board information
#define BOARD_SIZE 8
#define EMPTY 0
#define MY_FLAG 2
#define MY_KING 4
#define ENEMY_FLAG 1
#define ENEMY_KING 3

// bool
typedef int BOOL;
#define TRUE 1
#define FALSE 0
#define MAX_STEP 15
#define START "START"
#define PLACE "PLACE"
#define TURN "TURN"
#define END "END"
#define IDLE "IDLE"
#define MINE 0
#define ENEMY 1

struct Command
{
	int x[MAX_STEP];
	int y[MAX_STEP];
	int numStep;
};

int engine();
void loop();
#endif


