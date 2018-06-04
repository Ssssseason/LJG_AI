#include <vector>
#include <math.h>
#include "mcts.h"
#include <stdlib.h>
#include <cmath>
#include "mc_minmax.h"
double ucb_c = 1.4;

class node {
public:
	std::vector<node*> childs;
	node* parent;
	int visited_time;
	double wins;
	Role player;
	Bitboard cur_state;
	std::vector<Bitboard> next_states;
	std::vector<int> next_visited;
	std::vector<action> actions;
public:
	static double ucb(const node* c, int parent_visited) {
		return 1.0*c->wins / c->visited_time + ucb_c * sqrt(2 * log(parent_visited) / (c->visited_time));
	}

	node(Bitboard state, Role player, node* parent)
		:cur_state(state), player(player), visited_time(0), wins(0), parent(parent) {
		action bitactions = state.getActions(change_player(player));
		for (int i = 0; i < 64; i++) {
			action act = bitactions & (((uint64_t)1) << i);
			if (act) {
				Bitboard ns = state;
				ns.takeAction(change_player(player),act);
				next_states.push_back(ns);
				next_visited.push_back(0);
				actions.push_back(act);
			}
		}
	}

	node* select() {
		int vt = visited_time;
		return *max_element(childs.begin(), childs.end(), [vt](const node* a, const node* b) {
			return  ucb(a, vt) < ucb(b, vt);
		});
	}

	node* expand() {
		if (cur_state.hasEnded()) return NULL;
		if (next_states.size() == 0) {
			if (childs.size()) return NULL;
			node* parent = this;
			childs.push_back(new node(cur_state, change_player(player), parent));
			return childs[0];
		}
		int noexpand = 0;
		for (unsigned int i = 0; i < next_visited.size(); i++) {
			if (next_visited[i] == 0) noexpand++;
		}
		if (noexpand == 0) return NULL;
		int idx = rand() % noexpand;
		for (unsigned int i = 0; i < next_visited.size(); i++) {
			if (next_visited[i] == 0) {
				if (idx--) continue;
				else {
					next_visited[i] = 1;
					node* parent = this;
					childs.push_back(new node(next_states[i], change_player(player), parent));
					return childs.back();
				}
			}
		}
        return childs.back();//没有返回值 随便写了一个
	}
    
};

void delete_node(node* n) {
	for (auto c : n->childs) {
		delete_node(c);
	}
	delete n;
}

double eval_test(Bitboard b) {
	return Bitboard::evaluateCombine(b);
}

int mc_minmax_search(Bitboard b, Role player) {
	double res = minmax_search(player, b, 6, eval_test);
	return res>0?1:(res==0?-1:0);
}

int random_search(Bitboard b, Role player) {
	while (!b.hasEnded()) {
		std::vector<action> actions;
		action bitactions = b.getActions(player);
		for (int i = 0; i < 64; i++) {
			action act = bitactions & (((uint64_t)1) << i);
			if (act) {
				actions.push_back(act);
			}
		}
		if (actions.size()) {
			action a = actions[rand() % actions.size()];
			b.takeAction(player, a);
		}
		player = change_player(player);
	}
	std::pair<int, int> sc = b.getPieces();
	int bc = sc.first;
	int wc = sc.second;
	return bc - wc;
}

action mcts(Bitboard board, Role player, int iterations, Timer t) {
	srand(time(NULL));
	player = change_player(player);
	node* root = new node(board, player, NULL);
	if (root->next_states.size() == 0) return 0;
	if (root->next_states.size() == 1) return root->actions[0];
	int i;
	for (i = 0; i < iterations && t.getTimeLeft() > 0; i++) {
		root->visited_time++;
		node* c = root, *cp;
		/*expand or select*/
		while (1) {
			if (c->cur_state.hasEnded()) break;
			cp = c->expand();
			if (cp == NULL) {
				cp = c->select();
				cp->visited_time++;
				c = cp;
			}
			else {
				c = cp;
				break;
			}
		}
		/*while ((cp = c->expand()) == NULL) {
			c = c->select();
			c->visited_time++;
		}*/
		c->visited_time++;
		//double wins = mc_minmax_search(cp->cur_state, change_player(cp->player));
		double wins = random_search(c->cur_state, change_player(c->player));
		while (c != root) {
			if (c->player == BLACK && wins > 0) c->wins += 1;
			else if (c->player == WHITE && wins < 0) c->wins += 1;
			c = c->parent;
		}
	}
	/*node* best = root->select();
	action res;
	for (unsigned int i = 0; i < root->childs.size(); i++) {
		if (best == root->childs[i]) {
			res = root->actions[i];
		}
	}*/
	printf("ours : %d\n", i);
	double max_wins=-infinity;
	action res;
	for (int i = 0; i < root->childs.size(); i++) {
		node* t = root->childs[i];
		double twin = 1.0*t->wins / t->visited_time;
		if (twin > max_wins) {
			max_wins = twin;
			res = root->actions[i];
		}
	}
	delete_node(root);
	return res;
}
