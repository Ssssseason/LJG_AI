#ifndef MC_MINMAX_H
#define MC_MINMAX_H
#include "common.h"
#include "timer.hpp"
action mc_minmax(Role player, Bitboard board, int depth, eval evaluate,int iteration, Timer t);
double mc_alphabeta(Role player, double alpha, double beta, int depth, Bitboard board, action ac, eval evaluate);
double minmax_search(Role player, Bitboard board, int depth, eval evaluate);
#endif // !MC_MINMAX_H
