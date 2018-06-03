#include<vector>
#include <ctime>
#include<cmath>
#include<algorithm>
#include<cstdio>
#include <cstring>
#include "common.h"
#include "bitboard.hpp"
#include "minmax.h"
unsigned char Bitboard::cntOfByte[256] = {0};
unsigned char Bitboard::roxanneWeights[64] = { 0 };
unsigned char Bitboard::indices[64] = { 0 };
using namespace std;

action p1_engine(Role player, Bitboard b) {
	return minmax(player, b, 5);
}

action p2_engine(Role player, Bitboard b) {
	return minmax(player, b, 8);
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
	Bitboard::init();
	Bitboard b(0x810000000, 0x1008000000);
	//b.printBoard();
	Role tplayer = BLACK;
	Role p1= WHITE;
	char* p1_name = "human";
	Role p2 = BLACK;
	char* p2_name = "machine";
	int idx;
	uint64_t act;
	while (!b.hasEnded()) {
		b.printBoard();
		printf("%s's turn\n",tplayer==p1?p1_name:p2_name);
		if(tplayer==p1){
			act = run(p1_engine,tplayer,b);
		}
		else{
			act = run(p2_engine,tplayer,b);
		}
		if(act){
			b.takeAction(tplayer, act);
			std::pair<int, int> ta = decode_action(act);
			printf("%s do : %d %d\n", tplayer == p1 ? p1_name : p2_name, ta.first+1, ta.second+1);
		}
		tplayer = change_player(tplayer);
	}
	b.printBoard();
	int p1c = b.getScore(p1);
	int p2c = b.getScore(p2);
	if(p1c>p2c) printf("%s wins\n",p1_name);
	else if(p1c<p2c) printf("%s wins\n",p2_name);
	else printf("tie\n");
	system("pause");
	return 0;
}