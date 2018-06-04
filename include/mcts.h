#ifndef MCTS_H
#define MCTS_H
#include "common.h"
#include "timer.hpp"
action mcts(Bitboard board, Role player, int iterations, Timer t);

action MC_mct(Bitboard b, Role player, int iteration, Timer t);
#endif // !MCTS_
