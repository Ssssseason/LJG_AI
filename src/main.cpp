#include<vector>
#include <ctime>
#include<cmath>
#include<algorithm>
#include<cstdio>
#include <cstring>
#include "bitboard.hpp"
unsigned char Bitboard::cntOfByte[256] = {0};
unsigned char Bitboard::roxanneWeights[64] = { 0 };
unsigned char Bitboard::indices[64] = { 0 };
using namespace std;

const int dx[8] = { 0,1,0,-1,1,1,-1,-1 };
const int dy[8] = { 1,0,-1,0,1,-1,1,-1 };

double ucb_c = 1.414;

class state {
public:
	char data[8][8];


	int check_valid(int player, int x, int y) {
		int i;
		for (i = 0; i < 8; i++) {
			int nx = x + dx[i], ny = y + dy[i];
			for (; nx >= 0 && nx < 8 && ny >= 0 && ny < 8; nx += dx[i], ny += dy[i]) {
				if (data[nx][ny] != 3 - player) break;
			}
			if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8
				&& !(nx == x + dx[i] && ny == y + dy[i])
				&& data[nx][ny] == player) {
				break;
			}
		}
		return i != 8;
	}
	void reverse(int player, int x, int y) {
		data[x][y] = player;
		vector<int> dir;
		for (int i = 0; i < 8; i++) {
			int nx = x + dx[i], ny = y + dy[i];
			for (; nx >= 0 && nx < 8 && ny >= 0 && ny < 8; nx += dx[i], ny += dy[i]) {
				if (data[nx][ny] != 3 - player) break;
			}
			if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8
				&& !(nx == x + dx[i] && ny == y + dy[i])
				&& data[nx][ny] == player) {
				dir.push_back(i);
			}
		}
		for (auto i : dir) {
			int nx = x + dx[i], ny = y + dy[i];
			for (; nx >= 0 && nx < 8 && ny >= 0 && ny < 8; nx += dx[i], ny += dy[i]) {
				if (data[nx][ny] == 3 - player) data[nx][ny] = player;
				else break;
			}
		}
		for (auto i : dir) {
			int nx = x + dx[i], ny = y + dy[i];
			for (; nx >= 0 && nx < 8 && ny >= 0 && ny < 8; nx += dx[i], ny += dy[i]) {
				if (data[nx][ny] == 3) data[nx][ny] = player;
				else if (data[nx][ny] == player) break;
			}
		}
	}

	state* get_newstate(int player, int x, int y) {
		if (data[x][y] != 0)
			return NULL;
		if (check_valid(player, x, y)) {
			state* tmp = new state();
			memcpy(tmp->data, data, 64);
			tmp->reverse(player, x, y);
			return tmp;
		}
		return NULL;
	}

	int is_end() {
		int count[3] = { 0,0,0 };
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (data[i][j] == 0 && (check_valid(1, i, j) || check_valid(2, i, j))) return 0;
				if (data[i][j] != 0) count[data[i][j]]++;
			}
		}
		return count[1] > count[2] ? 1 : (count[1] == count[2] ? 3 : 2);
	}

	int random_search(int player) {
		state tmp;
		int tplayer = player;
		memcpy(tmp.data, data, 64);
		while (!tmp.is_end()) {
			vector<pair<int, int>> actions;
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					if (tmp.data[i][j] == 0) {
						if (tmp.check_valid(tplayer, i, j)) {
							actions.push_back(make_pair(i, j));
						}
					}
				}
			}
			if (actions.size()) {
				pair<int, int>& a = actions[rand() % actions.size()];
				tmp.reverse(tplayer, a.first, a.second);
			}
			tplayer = 3 - tplayer;
		}
		return tmp.is_end();


	}
};

class node {
public:
	vector<node*> childs;
	node* parent;
	int visited_time;
	int wins;
	int player;
	state* cur_state;
	vector<state*> next_states;
	vector<int> next_visited;
	vector<pair<int, int>> actions;
public:
	static double ucb(const node* c, int parent_visited) {
		return 1.0*c->wins / c->visited_time + ucb_c * sqrt(2 * log(parent_visited) / (c->visited_time));
	}

	node(state* s, int player, node* parent)
		:cur_state(s), player(player), visited_time(0), wins(0), parent(parent) {

		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				state* t = cur_state->get_newstate(3 - player, i, j);
				if (t != NULL) {
					next_states.push_back(t);
					next_visited.push_back(0);
					actions.push_back(make_pair(i, j));
				}
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
			childs.push_back(new node(cur_state, player, this));
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
					childs.push_back(new node(next_states[i], 3 - player, this));
					return childs.back();
				}
			}
		}
	}
};

pair<int, int> MCTS(state* s, int player, int iter) {
	node* root = new node(s, player, NULL);
	if (root->next_states.size() == 0) return make_pair(-1, -1);
	if (root->next_states.size() == 1) return root->actions[0];
	for (int i = 0; i < iter; i++) {
		root->visited_time++;
		node* c = root, *cp;
		/*expand or select*/
		while ((cp = c->expand()) == NULL) {
			c = c->select();
			c->visited_time++;
		}
		cp->visited_time++;
		int wins = cp->cur_state->random_search(3 - cp->player);
		while (cp != root) {
			if (cp->player == wins) cp->wins++;
			cp = cp->parent;
		}
	}
	node* best = root->select();
	for (unsigned int i = 0; i < root->childs.size(); i++) {
		if (best == root->childs[i]) {
			return root->actions[i];
		}
	}
};

void print_chess(state* chess) {
	printf("   ");
	for (int i = 0; i < 8; i++)printf(" %d ", i + 1);
	printf("\n");
	for (int i = 0; i < 8; i++) {
		printf(" %d ", i + 1);
		for (int j = 0; j < 8; j++) {
			if (chess->data[i][j] == 0) {
				printf("|_|");
			}
			else if (chess->data[i][j] == 1) {
				printf(" X ");
			}
			else if (chess->data[i][j] == 2) {
				printf(" O ");
			}
		}
		printf("\n");
	}
}

int main() {
	Bitboard::init();
	clock_t start, end;
	int num = 1000, i;

	i = num;
	start = clock();
	while (i--) {
		Bitboard b(0x810000000, 0x1008000000);
		//b.printBoard();
		Role tplayer = BLACK;
		int idx;
		uint64_t actions;
		while (!b.hasEnded()) {
			actions = b.getActions(tplayer);
			//printf("Turn: %s\n", tplayer == BLACK ? "Black" : "White");
			if (!actions) {
				tplayer = tplayer == BLACK ? WHITE : BLACK;
				continue;
			}
			while (!((actions >> (idx = rand() % 64)) & 1));
			b.takeAction(tplayer, (uint64_t)1 << idx);
			//b.printBoard();
			//b.evaluate();
			//printf("\n");
			tplayer = tplayer == BLACK ? WHITE : BLACK;
		}
		//printf("Black: %d\n", b.getScore(BLACK));
		//printf("White: %d\n", b.getScore(WHITE));
	}
	end = clock();
	printf("%d\n", end - start);

	i = num;
	state chess;
	memset(chess.data, 0, 64);
	chess.data[3][3] = 2;
	chess.data[4][4] = 2;
	chess.data[3][4] = 1;
	chess.data[4][3] = 1;
	int human_role = 1;
	int machine_role = 2;
	start = clock();
	while (i--) {
		state tmp;
		int tplayer = 1;
		memcpy(tmp.data, chess.data, 64);
		while (!tmp.is_end()) {
			vector<pair<int, int>> actions;
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					if (tmp.data[i][j] == 0) {
						if (tmp.check_valid(tplayer, i, j)) {
							actions.push_back(make_pair(i, j));
						}
					}
				}
			}
			if (actions.size()) {
				pair<int, int>& a = actions[rand() % actions.size()];
				tmp.reverse(tplayer, a.first, a.second);
			}
			tplayer = 3 - tplayer;
		}
		int wins = chess.is_end();
		//if (wins == human_role) printf("human wins\n");
		//else if (wins == machine_role) printf("machine wins\n");
		//else printf("tie\n");
	}
	end = clock();
	printf("%d\n", end - start);


	//state chess;
	//memset(chess.data, 0, 64);
	//chess.data[3][3] = 2;
	//chess.data[4][4] = 2;
	//chess.data[3][4] = 1;
	//chess.data[4][3] = 1;

	//int human_role = 1;
	//int machine_role = 2;

	//int turn = 1;

	//print_chess(&chess);
	//while (!chess.is_end()) {
	//	if (turn == human_role) {
	//		int x, y;

	//		while (1) {
	//			printf("human input: ");
	//			scanf("%d%d", &x, &y);
	//			x -= 1, y -= 1;
	//			if (chess.data[x][y] != 0 || !chess.check_valid(human_role, x, y)) {
	//				printf("invalid input\n");
	//			}
	//			else break;
	//		}
	//		chess.reverse(human_role, x, y);
	//		printf("human action: %d %d \n", x + 1, y + 1);
	//		print_chess(&chess);
	//	}
	//	else {
	//		pair<int, int> a = MCTS(&chess, human_role, 1000);
	//		if (a.first == -1 && a.second == -1) printf("machine jump one turn\n");
	//		else {
	//			printf("machine action: %d %d \n", a.first + 1, a.second + 1);
	//			chess.reverse(machine_role, a.first, a.second);
	//			print_chess(&chess);
	//		}
	//	}
	//	turn = 3 - turn;
	//}
	//int wins = chess.is_end();
	//if (wins == human_role) printf("human wins\n");
	//else if (wins == machine_role) printf("machine wins\n");
	//else printf("tie\n");
	system("pause");
	return 0;
}