#include "mc_minmax.h"
#include <time.h>
#include <stdlib.h>
#include <vector>
const int p = 2;

static double alphabeta(Role player, double alpha, double beta, int depth, Bitboard board, action ac, eval evaluate) {
	board.takeAction(change_player(player), ac);
	if (board.hasEnded()) {
		std::pair<int, int> sc = board.getPieces();
		Role wins = sc.first > sc.second ? BLACK : WHITE;
		if (wins == BLACK) return infinity;
		else return -infinity;
	}
	int random = rand() % p;
	if (random == p - 1) return evaluate(board);
	action actions = board.getActions(player);
	if (actions == 0) {
		return evaluate(board);
	}
	depth -= 1;
	if (depth <= 0) return board.evaluate();
	if (player == BLACK) {
		for (int i = 0; i < 64; i++) {
			action act = actions & (((uint64_t)1) << i);
			if (act) {
				alpha = std::max(alpha, alphabeta(change_player(player), alpha, beta, depth, board, act, evaluate));
				if (alpha > beta) break;
			}
		}
		return alpha;
	}
	else {
		for (int i = 0; i < 64; i++) {
			action act = actions & (((uint16_t)1) << i);
			if (act) {
				beta = std::min(beta, alphabeta(change_player(player), alpha, beta, depth, board, act, evaluate));
				if (beta < alpha) break;
			}
		}
		return beta;
	}
}

action mc_minmax(Role player, Bitboard board, int depth, eval evaluate, int iteration, Timer t) {
	//srand(time(NULL));
	action actions = board.getActions(player);
	if (actions == 0) return 0;
	std::vector<int> choice_count(64, 0);
	for (int k = 0; k < iteration && t.getTimeLeft() > 0; k++) {
		double alpha = -infinity;
		double beta = infinity;
		//int depth = DEPTH;
		action res = 0;
		if (player == BLACK) {
			for (int i = 0; i < 64; i++) {
				action act = actions & (((uint64_t)1) << i);
				if (act) {
					double val = alphabeta(change_player(player), alpha, beta, depth, board, act, evaluate);
					if (val > alpha || val == alpha && (res == 0 || Bitboard::getRoxannePriority(act) < Bitboard::getRoxannePriority(res))) {
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
					double val = alphabeta(change_player(player), alpha, beta, depth, board, act, evaluate);
					if (val < beta || val == beta && (res == 0 || Bitboard::getRoxannePriority(act) < Bitboard::getRoxannePriority(res))) {
						beta = val;
						res = act;
					}
				}
			}
		}
		choice_count[Bitboard::scanForward(res)]++;
	}
	int max_count = 0;
	int idx;
	for (int i = 0; i < 64; i++) {
		if (choice_count[i] > max_count) {
			max_count = choice_count[i];
			idx = i;
		}
	}
	return encode_action(idx / 8, idx % 8);
}