#include "core/uct.h"
#include <iostream>


#define MACHINE_TIME 4

#include<vector>
#include <ctime>
#include<cmath>
#include<algorithm>
#include<cstdio>
#include <cstring>
#include "common.h"
#include "bitboard.hpp"
#include "minmax.h"
#include "mcts.h"
#include <string>
#include "timer.hpp"
#include "mc_minmax.h"

action p1_engine(Role player, Bitboard b, Timer t); 

action p2_engine(Role player, Bitboard b, Timer t); 

action run(machine f, Role player, Bitboard board, Timer t);

void mainServer(int ai_seconds)
{
	int num = 10, i;
	bool first = true;

	i = num;
	while(i--){
		MatrixXd board;
		reversiInit(board);
		Bitboard::init();
		Bitboard b(0x810000000, 0x1008000000);

		bool turn(first);

		while (!reversiIsTerminal(board))
		{
			if (turn)
			{
				UCTTree uctt(board, first, Position(-1, -1));
				Position best_AI_pos;
				//std::printf("Now it is machine move\n");
				if (uctt.UCTSearch(ai_seconds, best_AI_pos))
				{
					//std::printf("machien choose to do ");
					//std::printf("AI (%d, %d)\n", best_AI_pos.x(), best_AI_pos.y());
					reversiStepOnPos(board, best_AI_pos, first);
					b.takeAction(first? BLACK:WHITE, encode_action(best_AI_pos.x(), best_AI_pos.y()));
					//b.printBoard();
				//std::cout << '\n' << board << std::endl;
				}
				else
				{
					//std::cout << "PASS!" << std::endl;
				}
				turn = !turn;
			}
			else
			{
				//std::cout << '\n' << board << std::endl;
				//int x, y;
				//// std::cin >> "your move:please enter x-pos and then y pos like \' 4 5 \'" >> std::endl;
				//// std::cin >> "x-pos ranges from 0 to 7. So is y-pos" >> std::endl;
				//std::printf("your move. Please enter your decision\n");
				//std::cin >> x >> y;
				//reversiStepOnPos(board, Position(x, y), !first);
				//std::cout << '\n' << board << std::endl;
				//turn = !turn;

				Timer t(ai_seconds);
				//action act = run(p1_engine,first?WHITE:BLACK, b, t);
				action act = run(p1_engine,first?WHITE:BLACK, b, t);
				if (act) {
					b.takeAction(first ? WHITE : BLACK, act);
					// b.printBoard();
					//printf("%lf\n", t.getTimeLeft());
					reversiStepOnPos(board, Position(decode_action(act)), !first);
				}
				turn = !turn;
			}
		}
		int black, white;
		reversiColorCount(board, black, white);
		printf("black: %d, white: %d\n", black, white);
	}

	i = num;
	first = false;
	while(i--){
		MatrixXd board;
		reversiInit(board);
		Bitboard::init();
		Bitboard b(0x810000000, 0x1008000000);

		bool turn(first);

		while (!reversiIsTerminal(board))
		{
			if (turn)
			{
				UCTTree uctt(board, first, Position(-1, -1));
				Position best_AI_pos;
				//std::printf("Now it is machine move\n");
				if (uctt.UCTSearch(ai_seconds, best_AI_pos))
				{
					//std::printf("machien choose to do ");
					//std::printf("AI (%d, %d)\n", best_AI_pos.x(), best_AI_pos.y());
					reversiStepOnPos(board, best_AI_pos, first);
					b.takeAction(first? BLACK:WHITE, encode_action(best_AI_pos.x(), best_AI_pos.y()));
					//b.printBoard();
				//std::cout << '\n' << board << std::endl;
				}
				else
				{
					//std::cout << "PASS!" << std::endl;
				}
				turn = !turn;
			}
			else
			{
				//std::cout << '\n' << board << std::endl;
				//int x, y;
				//// std::cin >> "your move:please enter x-pos and then y pos like \' 4 5 \'" >> std::endl;
				//// std::cin >> "x-pos ranges from 0 to 7. So is y-pos" >> std::endl;
				//std::printf("your move. Please enter your decision\n");
				//std::cin >> x >> y;
				//reversiStepOnPos(board, Position(x, y), !first);
				//std::cout << '\n' << board << std::endl;
				//turn = !turn;

				Timer t(ai_seconds);
				//action act = run(p1_engine,first?WHITE:BLACK, b, t);
				action act = run(p2_engine,first?WHITE:BLACK, b, t);
				if (act) {
					b.takeAction(first ? WHITE : BLACK, act);
					//b.printBoard();
					//printf("%lf\n", t.getTimeLeft());
					reversiStepOnPos(board, Position(decode_action(act)), !first);
				}
				turn = !turn;
			}
		}
		int black, white;
		reversiColorCount(board, black, white);
		printf("black: %d, white: %d\n", black, white);
	}

}

void battle() {
	bool machine_first = false;
	std::printf("-1 -> machine\n1 -> you\n the chess board is a 8*8 matrix ranging from 0 to 7\n");
	std::printf("for example,\'2 3\' means the 3rd row and 4th column\n");
	mainServer(MACHINE_TIME);

}

