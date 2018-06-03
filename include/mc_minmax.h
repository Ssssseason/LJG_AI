#ifndef MC_MINMAX_H
#define MC_MINMAX_H
#include "common.h"
#include "timer.hpp"
action mc_minmax(Role player, Bitboard board, int depth, eval evaluate,int iteration, Timer t);
#endif // !MC_MINMAX_H
