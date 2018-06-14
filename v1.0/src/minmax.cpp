#include <vector>
#include "minmax.h"

double alphabeta(Role player, double alpha, double beta, int depth, Bitboard board, action ac,eval evaluate) {
	if(ac)
		board.takeAction(change_player(player), ac);

	if (board.hasEnded()) {
		std::pair<int, int> sc = board.getPieces();

		int wins = -1;
		if (sc.first > sc.second) wins = BLACK;
		else if (sc.first < sc.second) wins = WHITE;
		else wins = -1;
		if (wins == BLACK) return INF;
		else if(wins == WHITE )return -INF;
		else return 0;

	}
	action actions = board.getActions(player);
	if (actions == 0) {
		return alphabeta(change_player(player), alpha, beta, depth, board, 0, evaluate);
	}
	depth -= 1;
	if (depth <= 0) return evaluate(board);
	if (player == BLACK) {
		alpha = -INF;
		for (int i = 0; i < 64; i++) {
			action act = actions & (((uint64_t)1) << i);
			if (act) {
				double val = alphabeta(change_player(player), alpha, beta, depth, board, act,evaluate);
				if (val > alpha) 
					alpha = val;
				if(val > beta)
					return val;
			}
		}
		return alpha;
	}
	else {
		beta = INF;
		for (int i = 0; i < 64; i++) {
			action act = actions & (((uint64_t)1) << i);
			if (act) {
				double val = alphabeta(change_player(player), alpha, beta, depth, board, act,evaluate);
				if (val < beta) 
					beta = val;
				if(val < alpha)
					return val;
			}
		}
		return beta;
	}
}

action minmax(Role player, Bitboard board, int depth,eval evaluate) {
	action actions = board.getActions(player);
	if (actions == 0) return 0;
	double alpha = -INF;
	double beta = INF;

	action res = 0;
	if (player == BLACK) {
		for (int i = 0; i < 64; i++) {
			action act = actions & (((uint64_t)1) << i);
			if (act) {
				double val = alphabeta(change_player(player), alpha, beta, depth, board, act,evaluate);
				if (val > alpha || val == alpha && (res == 0 || Bitboard::getRoxannePriority(act) > Bitboard::getRoxannePriority(res))) {
					alpha = val;
					res = act;
				}
			}
		}
	}
	else {
		for (int i = 0; i < 64; i++) {
			action act = actions & (((uint64_t)1) << i);
			if (act) {
				double val = alphabeta(change_player(player), alpha, beta, depth, board, act,evaluate);
				if (val < beta || val == beta && (res == 0 ||  Bitboard::getRoxannePriority(act) > Bitboard::getRoxannePriority(res))) {
					beta = val;
					res = act;
				}
			}
		}
	}
	assert(res);
	return res;
}
