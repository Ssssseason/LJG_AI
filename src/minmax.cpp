#include <vector>
#include "minmax.h"

double alphabeta(Role player, double alpha, double beta, int depth, Bitboard board, action ac,eval evaluate) {
	if(ac)
		board.takeAction(change_player(player), ac);
	// board.takeAction(change_player(player), ac);
	if (board.hasEnded()) {
		std::pair<int, int> sc = board.getPieces();
		Role wins = sc.first > sc.second ? BLACK : WHITE;
		if (wins == BLACK) return infinity;
		else return -infinity;
	}
	action actions = board.getActions(player);
	if (actions == 0) {
		return alphabeta(change_player(player), alpha, beta, depth, board, 0, evaluate);
		// return evaluate(board);
	}
	depth -= 1;
	if (depth <= 0) return evaluate(board);
	if (player == BLACK) {
		for (int i = 0; i < 64; i++) {
			action act = actions & (((uint64_t)1) << i);
			if (act) {
				alpha = std::max(alpha, alphabeta(change_player(player), alpha, beta, depth, board, act,evaluate));
				if (alpha > beta) break;
			}
		}
		return alpha;
	}
	else {
		for (int i = 0; i < 64; i++) {
			action act = actions & (((uint16_t)1) << i);
			if (act) {
				beta = std::min(beta, alphabeta(change_player(player), alpha, beta, depth, board, act,evaluate));
				if (beta < alpha) break;
			}
		}
		return beta;
	}
}

action minmax(Role player, Bitboard board, int depth,eval evaluate) {
	action actions = board.getActions(player);
	if (actions == 0) return 0;
	double alpha = -infinity;
	double beta = infinity;
	//int depth = DEPTH;
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