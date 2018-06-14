#include "common.h"
#include "bitboard.hpp"
#include <vector>
#include <cmath>
#include <random>
#include "minmax.h"
#include "mc_minmax.h"
#include "iteration_minmax.h"

using namespace std;
double evaluate_combine(Bitboard b);

static int imm_depth = 3;

struct IMM_node {
public:
	Bitboard b;
	action acted;//just played action
	vector<IMM_node*> childs;
	IMM_node* parent;
	double score;
	Role player;// playing player
	int p;
	IMM_node(IMM_node* parent, Role player, Bitboard b, action acted, int p) :
		parent(parent), b(b),score(0),player(player), acted(acted), p(p) {
	}
	~IMM_node() {
		for (auto c : childs) {
			delete c;
		}
	}
	static bool cmp(const IMM_node* a, const IMM_node* b) {
		return a->score < b->score;
	}
};

//static const double ucb_c = 0.5;

IMM_node* IMM_select(IMM_node* root) {
	while (1) {
		if (root->player == BLACK) {
			// max node;
			auto next = std::max_element(root->childs.begin(), root->childs.end(),IMM_node::cmp);
			if (next == root->childs.end()) break;
			else root = *next;
		}
		else {
			//min node
			auto next = std::min_element(root->childs.begin(), root->childs.end(),IMM_node::cmp);
			if (next == root->childs.end()) break;
			else root = *next;
		}
	}
	return root;
}


double IMM_minmax(IMM_node* root) {
	srand(time(NULL));
	int p = root->p;
	if (root->b.hasEnded()) {
		pair<int, int> sc = root->b.getPieces();
		if (sc.first > sc.second) root->score = INF;
		else if (sc.first < sc.second) root->score = -INF;
		else root->score = 0;
		return root->score;
	}
	Bitboard board = root->b;
	Role player = root->player;
	action actions = board.getActions(player);
	if (actions == 0) {
		IMM_node* tmp = new IMM_node(root, change_player(player), board, 0, p);
		root->childs.push_back(tmp);
		root->score = IMM_minmax(tmp);
		return root->score;
	}
	double alpha = -INF;
	double beta = INF;
	//int depth = DEPTH;
	//action res = 0;
	if (player == BLACK) {
		for (int i = 0; i < 64; i++) {
			action act = actions & (((uint64_t)1) << i);
			if (act) {
				double val;
				if(p > 0) 
					val = mc_alphabeta(change_player(player), alpha, beta, imm_depth, board, act, p, evaluate_combine);
				else if(p == 0)
					val = alphabeta(change_player(player), alpha, beta, imm_depth, board, act, evaluate_combine);
				if (val >= alpha) {
					alpha = val;
				}
				Bitboard tmpb = board;
				tmpb.takeAction(player, act);
				IMM_node* tmp = new IMM_node(root, change_player(player), tmpb, act, p);
				tmp->score = val;
				root->childs.push_back(tmp);
			}
		}
		root->score = alpha;
	}
	else {
		for (int i = 0; i < 64; i++) {
			action act = actions & (((uint64_t)1) << i);
			if (act) {
				double val;
				if(p > 0) 
					val = mc_alphabeta(change_player(player), alpha, beta, imm_depth, board, act, p, evaluate_combine);
				else if(p == 0)
					val = alphabeta(change_player(player), alpha, beta, imm_depth, board, act, evaluate_combine);
				if (val <= beta) {
					beta = val;
				}
				Bitboard tmpb = board;
				tmpb.takeAction(player, act);
				IMM_node* tmp = new IMM_node(root, change_player(player), tmpb, act, p);
				tmp->score = val;
				root->childs.push_back(tmp);
			}
		}
		root->score = beta;
	}
	return root->score;
}



action IMM_imm(Bitboard b, Role player, int depth, int p, Timer t) {
	imm_depth = depth;
	if (b.hasEnded()) return 0;
	action actions = b.getActions(player);
	if (actions == 0) return 0;
	int cnt = 0;
	action res = 0;
	for (int i = 0; i < 64; i++) {
		if (actions&((uint64_t)1 << i)) {
			cnt++;
			res = actions & ((uint64_t)1 << i);
		}
	}
	if (cnt == 1) return res;
	IMM_node* root = new IMM_node(NULL, player, b,0, p);
	int iterations = 0;
	for (iterations = 0; t.getTimeLeft() > 0; iterations++) {
		IMM_node* tmp = IMM_select(root);
		double val = IMM_minmax(tmp);
		while (tmp->parent) {
			IMM_node* p = tmp->parent;
			if (p->player == BLACK) {
				p->score = (*(std::max_element(p->childs.begin(), p->childs.end(), IMM_node::cmp)))->score;
			}
			else {
				p->score = (*(std::min_element(p->childs.begin(), p->childs.end(), IMM_node::cmp)))->score;
			}
			tmp = p;
		}
	}
	if (root->player == BLACK) {
		res = (*(std::max_element(root->childs.begin(), root->childs.end(), IMM_node::cmp)))->acted;
	}
	else {
		res = (*(std::min_element(root->childs.begin(), root->childs.end(), IMM_node::cmp)))->acted;
	}
	printf("IMM: %d\n", iterations);
	delete root;
	return res;
}
