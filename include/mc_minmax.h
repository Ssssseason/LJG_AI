#ifndef MC_MINMAX_H
#define MC_MINMAX_H
#include "common.h"
#include "timer.hpp"
// action mc_minmax(Role player, Bitboard board, int depth, eval evaluate,Timer t);
double mc_alphabeta(Role player, double alpha, double beta, int depth, Bitboard board, action ac, int p, eval evaluate);
int minmax_search(Role player, Bitboard board, int depth, int p, eval evaluate);
#endif // !MC_MINMAX_H
