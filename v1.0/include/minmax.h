#ifndef MINMAX_H
#define MINMAX_H
#include "common.h"
action minmax(Role player, Bitboard board,int depth,eval evaluate);
double alphabeta(Role player, double alpha, double beta, int depth, Bitboard board, action ac, eval evaluate);
#endif