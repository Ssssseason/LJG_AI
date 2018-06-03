#include <vector>
#include "mcts.h"

double ucb_c = 1.414;

class node {
public:
	std::vector<node*> childs;
	node* parent;
	int visited_time;
	int wins;
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
		if (next_states.size() == 0) {
			if (childs.size()) return NULL;
			childs.push_back(new node(cur_state, change_player(player), this));
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
					childs.push_back(new node(next_states[i], change_player(player), this));
					return childs.back();
				}
			}
		}
	}
};

void delete_node(node* n) {
	for (auto c : n->childs) {
		delete_node(c);
	}
	delete n;
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
	int bc = b.getScore(BLACK);
	int wc = b.getScore(WHITE);
	if (bc < wc) return WHITE;
	else if (bc > wc) return BLACK;
	else return -1;
}

action mcts(Bitboard board, Role player, int iterations) {
	player = change_player(player);
	node* root = new node(board, player, NULL);
	if (root->next_states.size() == 0) return 0;
	if (root->next_states.size() == 1) return root->actions[0];
	for (int i = 0; i < iterations; i++) {
		root->visited_time++;
		node* c = root, *cp;
		/*expand or select*/
		while ((cp = c->expand()) == NULL) {
			c = c->select();
			c->visited_time++;
		}
		cp->visited_time++;
		int wins = random_search(cp->cur_state, change_player(cp->player));
		while (cp != root) {
			if (cp->player == wins) cp->wins++;
			cp = cp->parent;
		}
	}
	node* best = root->select();
	action res;
	for (unsigned int i = 0; i < root->childs.size(); i++) {
		if (best == root->childs[i]) {
			res = root->actions[i];
		}
	}
	delete_node(root);
	return res;
}