#include<cstdio>
#include <ctime>

#include "common.h"
#include "bitboard.hpp"
#include "minmax.h"
#include "mcts.h"

unsigned char Bitboard::cntOfByte[256] = {0};
unsigned char Bitboard::roxanneWeights[64] = { 0 };
unsigned char Bitboard::indices[64] = { 0 };
using namespace std;

double evaluate_combine(Bitboard b) {
	return Bitboard::evaluateCombine(b);
}

action p1_engine(Role player, Bitboard b) {
	return minmax(player, b, 5,evaluate_combine);
}

action p2_engine(Role player, Bitboard b) {
	return mcts(b, player, 10000);
}

action run(machine f,Role player, Bitboard board){
	return f(player, board);
}

action human(Role player, Bitboard board){
	int x,y;
	scanf("%d%d",&x,&y);
	return encode_action(x-1,y-1);
}

int main() {
	srand(time(NULL));//for mcts
	Bitboard::init();

	clock_t bstart, bend, wstart, wend;
	clock_t btime, wtime;
	int num = 30;
	while (num--) {
		btime = 0;
		wtime = 0;

		Bitboard b(0x810000000, 0x1008000000);
		//b.printBoard();
		Role tplayer = BLACK;
		Role p1 = BLACK;
		char* p1_name = "minmax_5_combine";
		Role p2 = WHITE;
		char* p2_name = "mcts_1000";
		printf("BLACK: %s\t WHITE: %s\t\n", p1 == BLACK ? p1_name : p2_name, p1 == WHITE ? p1_name : p2_name);
		int idx;
		uint64_t act;
		while (!b.hasEnded()) {
			//b.printBoard();
			//printf("%s's turn\n", tplayer == p1 ? p1_name : p2_name);
			if (tplayer == p1) {
				bstart = clock();
				act = run(p1_engine, tplayer, b);
				bend = clock();
				btime += bend - bstart;
			}
			else {
				wstart = clock();
				act = run(p2_engine, tplayer, b);
				wend = clock();
				wtime += wend - wstart;
			}
			if (act) {
				b.takeAction(tplayer, act);
				std::pair<int, int> ta = decode_action(act);
				//printf("%s do : %d %d\n", tplayer == p1 ? p1_name : p2_name, ta.first + 1, ta.second + 1);
			}
			tplayer = change_player(tplayer);
		}
		//b.printBoard();
		std::pair<int, int> sc = b.getPieces();
		int bc = sc.first;
		int wc = sc.second;
		if (bc > wc) printf("BLACK WINS\n");
		else if (bc < wc) printf("WHITE WINS\n");
		else printf("TIE\n");
		printf("BLACK: %s\t WHITE: %s\t\n", p1 == BLACK ? p1_name : p2_name, p1 == WHITE ? p1_name : p2_name);
		printf("BLACK: %d\t WHITE: %d\t\n", bc, wc);
		printf("BLACK: %d\t WHITE: %d\t\n", btime, wtime);
	}


	num = 30;
	while (num--) {
		btime = 0;
		wtime = 0;

		Bitboard b(0x810000000, 0x1008000000);
		//b.printBoard();
		Role tplayer = BLACK;
		Role p2 = BLACK;
		char* p2_name = "minmax_5_combine";
		Role p1 = WHITE;
		char* p1_name = "mcts_1000";
		printf("BLACK: %s\t WHITE: %s\t\n", p1 == BLACK ? p1_name : p2_name, p1 == WHITE ? p1_name : p2_name);
		int idx;
		uint64_t act;
		while (!b.hasEnded()) {
			//b.printBoard();
			//printf("%s's turn\n", tplayer == p1 ? p1_name : p2_name);
			if (tplayer == p1) {
				bstart = clock();
				act = run(p1_engine, tplayer, b);
				bend = clock();
				btime += bend - bstart;
			}
			else {
				wstart = clock();
				act = run(p2_engine, tplayer, b);
				wend = clock();
				wtime += wend - wstart;
			}
			if (act) {
				b.takeAction(tplayer, act);
				std::pair<int, int> ta = decode_action(act);
				//printf("%s do : %d %d\n", tplayer == p1 ? p1_name : p2_name, ta.first + 1, ta.second + 1);
			}
			tplayer = change_player(tplayer);
		}
		//b.printBoard();
		std::pair<int, int> sc = b.getPieces();
		int bc = sc.first;
		int wc = sc.second;
		if (bc > wc) printf("BLACK WINS\n");
		else if (bc < wc) printf("WHITE WINS\n");
		else printf("TIE\n");
		printf("BLACK: %s\t WHITE: %s\t\n", p1 == BLACK ? p1_name : p2_name, p1 == WHITE ? p1_name : p2_name);
		printf("BLACK: %d\t WHITE: %d\t\n", bc, wc);
		printf("BLACK: %d\t WHITE: %d\t\n", btime, wtime);
	}


	system("pause");
	return 0;
}