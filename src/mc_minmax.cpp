#include "mc_minmax.h"
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <random>
//const int p = 4;

double mc_alphabeta(Role player, double alpha, double beta, int depth, Bitboard board, action ac, int p, eval evaluate) {
	if(ac)
		board.takeAction(change_player(player), ac);
	// board.takeAction(change_player(player), ac);
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

	int random = rand() % p;

	if (random == p - 1) return evaluate(board);
	action actions = board.getActions(player);
	if (actions == 0) {
		return mc_alphabeta(change_player(player), alpha, beta, depth, board, 0, p, evaluate);
		// return evaluate(board);
	}
	depth -= 1;
	if (depth <= 0) return evaluate(board);
	if (player == BLACK) {
		alpha = -INF;
		for (int i = 0; i < 64; i++) {
			action act = actions & (((uint64_t)1) << i);
			if (act) {
				double val = mc_alphabeta(change_player(player), alpha, beta, depth, board, act, p, evaluate);
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
				double val = mc_alphabeta(change_player(player), alpha, beta, depth, board, act, p, evaluate);
				if (val < beta) 
					beta = val;
				if(val < alpha)
					return val;
			}
		}
		return beta;
	}
}

// action mc_minmax(Role player, Bitboard board, int depth, eval evaluate, Timer t) {
// 	srand(time(NULL));
// 	action actions = board.getActions(player);
// 	if (actions == 0) return 0;
// 	std::vector<int> choice_count(64, 0);
// 	for (int k = 0; t.getTimeLeft() > 0; k++) {
// 		double alpha = -INF;
// 		double beta = INF;
// 		//int depth = DEPTH;
// 		action res = 0;
// 		if (player == BLACK) {
// 			for (int i = 0; i < 64; i++) {
// 				action act = actions & (((uint64_t)1) << i);
// 				if (act) {
// 					double val = mc_alphabeta(change_player(player), alpha, beta, depth, board, act, evaluate);
// 					if (val > alpha || val == alpha && (res == 0 || Bitboard::getRoxannePriority(act) > Bitboard::getRoxannePriority(res))) {
// 						alpha = val;
// 						res = act;
// 					}
// 				}
// 			}
// 		}
// 		else {
// 			for (int i = 0; i < 64; i++) {
// 				action act = actions & (((uint64_t)1) << i);
// 				if (act) {
// 					double val = mc_alphabeta(change_player(player), alpha, beta, depth, board, act, evaluate);
// 					if (val < beta || val == beta && (res == 0 || Bitboard::getRoxannePriority(act) > Bitboard::getRoxannePriority(res))) {
// 						beta = val;
// 						res = act;
// 					}
// 				}
// 			}
// 		}
// 		choice_count[Bitboard::scanForward(res)]++;
// 	}
// 	int max_count = 0;
// 	int idx;
// 	for (int i = 0; i < 64; i++) {
// 		if (choice_count[i] > max_count) {
// 			max_count = choice_count[i];
// 			idx = i;
// 		}
// 	}
// 	return encode_action(idx / 8, idx % 8);
// }

int minmax_search(Role player, Bitboard board, int depth, int p, eval evaluate) {
	double alpha = -INF;
	double beta = INF;
	double value = 0;
	if (board.hasEnded()) {
		std::pair<int, int> sc = board.getPieces();
		if (sc.first > sc.second) return 1;
		else if (sc.first < sc.second) return -1;
		else return 0;
	}

	action actions = board.getActions(player);

	if (actions == 0) {
		double val = mc_alphabeta(change_player(player), alpha, beta, depth, board, 0, p, evaluate);
		if (val > 0) return 1;
		else if (val < 0) return -1;
		else return 0;
	}

	srand(time(NULL));
	//int depth = DEPTH;
	action res = 0;
	if (player == BLACK) {
		for (int i = 0; i < 64; i++) {
			action act = actions & (((uint64_t)1) << i);
			if (act) {
				double val = mc_alphabeta(change_player(player), alpha, beta, depth, board, act, p, evaluate);
				value += val;
				if (val > alpha || val == alpha && (res == 0 || Bitboard::getRoxannePriority(act) > Bitboard::getRoxannePriority(res))) {
					alpha = val;
					res = act;
				}
			}
		}
		if (alpha > 0) return 1;
		else if (alpha < 0) return -1;
		else return 0;
	}
	else {
		for (int i = 0; i < 64; i++) {
			action act = actions & (((uint64_t)1) << i);
			if (act) {
				double val = mc_alphabeta(change_player(player), alpha, beta, depth, board, act, p, evaluate);
				value += val;
				if (val < beta || val == beta && (res == 0 || Bitboard::getRoxannePriority(act) > Bitboard::getRoxannePriority(res))) {
					beta = val;
					res = act;
				}
			}
		}
		if (beta > 0) return 1;
		else if (beta < 0) return -1;
		else return 0;
		// return beta;
	}
	//assert(res);
	//return res;

	return value > 0? 1:(value<0? -1: 0);
}

