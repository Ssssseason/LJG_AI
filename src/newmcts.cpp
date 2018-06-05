#include "common.h"
#include "bitboard.hpp"
#include <vector>
#include <cmath>
#include <random>
#include "mcts.h"
#include "mc_minmax.h"
using namespace std;
double evaluate_combine(Bitboard b);
struct MC_node {
public:
	Bitboard b;
	action acted;//just played action
	vector<MC_node*> childs;
	MC_node* parent;
	int wins;
	int visited_times;
	vector<int> candidate_flags;
	vector<action> candidate_actions;
	Role player;//just played not current player
	MC_node(MC_node* parent, Role player, Bitboard b,action acted) :
		parent(parent), b(b), wins(0), visited_times(1),player(player),acted(acted){
		action actions = b.getActions(change_player(player));
		for (int i = 0; i < 64; i++) {
			action act = actions & ((uint64_t)1 << i);
			if (act) {
				candidate_flags.push_back(0);
				candidate_actions.push_back(act);
			}
		}
	}
	~MC_node() {
		for (auto c : childs) {
			delete c;
		}
	}
};

static const double ucb_c = 0.5;

MC_node* MC_select(MC_node* root) {
	if (root->childs.size() == 0) return NULL;
	if (root->childs.size() == 1) return root->childs[0];
	double max_score = -infinity;
	MC_node* res = NULL;
	for (auto c : root->childs) {
		double score = double(c->wins) / c->visited_times +
			ucb_c * sqrt(2 * log(root->visited_times) / double(c->visited_times));
		if (score > max_score) {
			max_score = score;
			res = c;
		}
	}
	return res;
}

MC_node* MC_expand(MC_node* root) {
	if (root->b.hasEnded()) return NULL;
	if (root->childs.size()==0 && root->candidate_actions.size() == 0) {
		root->childs.push_back(new MC_node(root, change_player(root->player), root->b,0));
		return root->childs.back();
	}
	else if (root->candidate_actions.size()) {
		for (int i = 0; i < root->candidate_flags.size(); i++) {
			if (root->candidate_flags[i] == 0) {
				root->candidate_flags[i] = 1;
				Bitboard tmp = root->b;
				tmp.takeAction(change_player(root->player), root->candidate_actions[i]);
				root->childs.push_back(new MC_node(root, change_player(root->player), tmp,root->candidate_actions[i]));
				return root->childs.back();
			}
		}
		return NULL;
	}
	return NULL;
}

int MC_random_search(Bitboard b, Role player) {
	random_device rd;
	mt19937 gen(rd());
	while (!b.hasEnded()) {
		vector<action> actions;
		action candidate = b.getActions(player);
		for (int i = 0; i < 64; i++) {
			action act = candidate & ((uint64_t)1 << i);
			if (act) {
				actions.push_back(act);
			}
		}
		if (actions.size() > 0) {
			uniform_int_distribution<> dis(0, actions.size() - 1);
			int idx = dis(gen);
			b.takeAction(player, actions[idx]);
		}
		player = change_player(player);
	}
	pair<int, int> sc = b.getPieces();
	if (sc.first > sc.second) return 1;
	else if (sc.first < sc.second) return -1;
	else return 0;
}


action MC_mct(Bitboard b, Role player, int depth, Timer t) {
	MC_node* root = new MC_node(NULL, change_player(player), b,0);
	if (root->candidate_actions.size() == 0) {
		delete root;
		return 0;
	}
	if (root->candidate_actions.size() == 1) {
		action res = root->candidate_actions[0];
		delete root;
		return res;
	}
	int i;
	for (i = 0; t.getTimeLeft()>0; i++) {
		root->visited_times++;
		MC_node* c = root, *tc;
		while (1) {
			if (c->b.hasEnded()) break;
			tc = MC_expand(c);
			if (tc == NULL) {
				tc = MC_select(c);
				c = tc;
				if (c == NULL) break;
				else c->visited_times++;
			}
			else {
				c = tc;
				break;
			}
		}
		if (c == NULL) continue;
		int wins;
		if(depth == 0)
			wins = MC_random_search(c->b, change_player(c->player));
		else if(depth == -1){
			double val = evaluate_combine(c->b);
			wins = val > 0? 1: (val < 0? -1:0);
		}
		else if(depth > 0)
			wins = minmax_search(change_player(c->player), c->b, depth, evaluate_combine);
		while (c != root) {
			if (c->player == BLACK&&wins==1) {
				c->wins++;
			}
			else if (c->player == WHITE && wins == -1) {
				c->wins++;
			}
			c = c->parent;
		}
	}
	printf("ours %d\n", i);
	action res = 0;
	double max_wins = -infinity;
	for (int i = 0; i < root->childs.size(); i++) {
		MC_node* tmp = root->childs[i];
		double tmp_wins = double(tmp->wins) / tmp->visited_times;
		if (tmp_wins > max_wins) {
			max_wins = tmp_wins;
			res = tmp->acted;
		}
	}
	delete root;
	return res;
}