#include<cstdio>
#include <ctime>

#include "common.h"
#include "bitboard.hpp"
#include "minmax.h"
#include "mcts.h"
#include <string>
#include "timer.hpp"
#include "mc_minmax.h"
#include <climits>

#include "net.hpp"
#include "iteration_minmax.h"
#include <iostream>
#include <unistd.h>


unsigned char Bitboard::cntOfByte[256] = {0};
char Bitboard::roxanneWeights[64] = { 0 };
unsigned char Bitboard::indices[64] = { 0 };
using namespace std;

string id="";
string player="";
string qipan="";
string turn="";
void battle();
void pkmain();

double evaluate_combine(Bitboard b) {
	return Bitboard::evaluateCombine(b);
}

action human(Role player, Bitboard board) {
	int x, y;
	scanf("%d%d", &x, &y);
	return encode_action(x - 1, y - 1);
}


action p1_engine(Role player, Bitboard b, Timer t) {
	//return MC_mct(b, player, 3, 4, t);

	return IMM_imm(b, player, 3, 0, t);

	//return IMM_imm(b, player, 3, false, t);
	//return minmax(player, b, 6, evaluate_combine);
}

action p2_engine(Role player, Bitboard b, Timer t) {
	// 0: randowm_search; -1: heuristic evaluation;
	// i (i > 0): the depth of minmax
	//return MC_mct(b, player, -1, t);
	//return IMM_imm(b, player, 3, 0, t);
	//return mc_minmax(player, b, 6, evaluate_combine, t);
	return minmax(player, b, 6, evaluate_combine);
}

action run(machine f,Role player, Bitboard board, Timer t){
	return f(player, board, t);
}

int main() {
    cout << "start" <<endl;
    
    srand(time(NULL));//for mcts
    
    Bitboard::init();
    Bitboard b(0x810000000, 0x1008000000);
    //Timer t=3;
    
    //b.printBoard();
    string SERVER_IPSERVER_IP="http://47.89.179.202:5000/";
    id="11";
    string res="";
    double timeSum=0;
    double time = 0;
    
    Role ply;
    while(player==""){
        player=tcurl(SERVER_IPSERVER_IP+"create_session/"+id);
        cout << player << endl;
    }
    if (player=="B") ply=BLACK;
    else ply=WHITE;
    while(1){
        int errorCount=0;
        qipan="";
        turn="";
        res="";
        while(turn=="" ||turn!=player){
            sleep(2);
            turn=tcurl(SERVER_IPSERVER_IP+"turn/"+id);
            //cout <<SERVER_IPSERVER_IP+"turn/"+id<<endl;
            cout << turn <<endl;
            
        }
        while(qipan==""){
            qipan=tcurl(SERVER_IPSERVER_IP+"board_string/"+id);
            //cout <<SERVER_IPSERVER_IP+"board_string/"+id<<endl;
            cout << qipan<<endl;
        }
        changeData(qipan,&b);
        b.printBoard();
        //if(b.hasEnded()) break;
        if(turn==player){
            
            Timer t=3;
            action temp=run(p1_engine, ply, b, t);
            time = t.getTimePassed();
            timeSum += time;
            printf("%lf\n", time);
            //cout << time << endl;
            //cout <<temp<<endl;
            assert(temp);
            pair<int,int> xy = decode_action(temp);
            printf("%d %d\n", xy.first, xy.second);
            //int i=0;
            //while((temp>>i)!=1){
            //    i++;
            //}
            //cout <<i/8<<' '<<i%8<<endl;
            while(res!="SUCCESS" && res!="ERROR") {
                //res=tcurl('0'+i/8,'0'+i%8) ;
                res = tcurl(xy.first+'0', xy.second+'0');
                cout <<res<<endl;
                // if(res=="ERROR") errorCount++;
                // if(errorCount>1) break;
            }
            b.takeAction(ply, temp);
            b.printBoard();
            if(b.hasEnded()){
                printf("Use of Time: %lf\n", timeSum);
                //cout << "Use of Time: " << timeSum << endl;
                pair<int, int> nums = b.getPieces();
                printf("Black: %d\tWhite: %d\n", nums.first, nums.second);
                break;
            }
        }
    }
    //cout <<"Use of Time: "<<timeSum<<endl;

   ////网络初始化
   ////请求房间号session_id 这个老师还没给 假装是2
   //
   //id="2";
   //string url="";
   //string turn="";
   ////1 返回整个棋盘
   //url="http://47.89.179.202:5000/board_string/"+id;
   //qipan=tcurl(url);
   //cout << "棋盘"<< endl;
   //cout << qipan << endl;
   ////2 返回W or B
   //url="http://47.89.179.202:5000/turn/"+id;
   //turn=tcurl(url);
   //cout << "turn"<< endl;
   //cout << turn << endl;
   ////3 返回自己白棋还是黑棋
   //url="http://47.89.179.202:5000/create_session/"+id;
   //player=tcurl(url);
   //cout << "己方"<< endl;
   //cout << player << endl;
   ////4 下棋
   //char x='3';
   //char y='5';
   //tcurl(x,y);//xy均为char
    //Bitboard b(0x810000000, 0x1008000000);
    //changeData("0.0.0.0.0.0.0.0;0.0.0.0.0.0.0.0;0.0.0.0.0.0.0.0;0.0.0.W.B.0.0.0;0.0.0.B.W.0. 0.0;0.0.0.0.0.0.0.0;0.0.0.0.0.0.0.0;0.0.0.0.0.0.0.0",b);

   
	 //clock_t bstart, bend, wstart, wend;
	 //clock_t p1time, p2time;
	 //int seconds = 10;
	 //int num = 3;
	 //while (num--) {
	 //	p1time = 0;
	 //	p2time = 0;

	 //	Bitboard b(0x810000000, 0x1008000000);
	 //	//b.printBoard();
	 //	Role tplayer = BLACK;
	 //	Role p1 = BLACK;
	 //	//char* p1_name = "mcts_3";
	 //	//char* p1_name = "minmax_6_combine";
	 //	char* p1_name = "imm_3";
	 //	Role p2 = WHITE;
	 //	char* p2_name = "minmax_6_combine";
	 //	//char* p2_name = "mcts_-1";
	 //	//char* p2_name = "mc_imm_3";
	 //	//char* p2_name = "mc_minmax";
	 //	//char* p2_name = "human";
	 //	printf("BLACK: %s\t WHITE: %s\t\n", p1 == BLACK ? p1_name : p2_name, p1 == WHITE ? p1_name : p2_name);
	 //	int idx;
	 //	uint64_t act;
	 //	while (!b.hasEnded()) {
	 //		b.printBoard();
	 //		// printf("%s's turn\n", tplayer == p1 ? p1_name : p2_name);
	 //		if (tplayer == p1) {
	 //			bstart = clock();
	 //			Timer t(seconds);
	 //			act = run(p1_engine, tplayer, b, t);
	 //			bend = clock();
	 //			p1time += bend - bstart;
	 //		}
	 //		else {
	 //			wstart = clock();
	 //			Timer t(seconds);
	 //			act = run(p2_engine, tplayer, b, t);
	 //			//act = human(tplayer, b);
	 //			wend = clock();
	 //			cout << t.getTimeLeft() << endl;
	 //			p2time += wend - wstart;
	 //		}
	 //		if (act) {
	 //			b.takeAction(tplayer, act);
	 //			std::pair<int, int> ta = decode_action(act);
	 //			// printf("%s do : %d %d\n", tplayer == p1 ? p1_name : p2_name, ta.first + 1, ta.second + 1);
	 //		}
	 //		tplayer = change_player(tplayer);

	 //	}
	 //	b.printBoard();
	 //	std::pair<int, int> sc = b.getPieces();
	 //	int bc = sc.first;
	 //	int wc = sc.second;
	 //	if (bc > wc) printf("BLACK WINS\n");
	 //	else if (bc < wc) printf("WHITE WINS\n");
	 //	else printf("TIE\n");
	 //	printf("BLACK: %s\t WHITE: %s\t\n", p1 == BLACK ? p1_name : p2_name, p1 == WHITE ? p1_name : p2_name);
	 //	printf("BLACK: %d\t WHITE: %d\t\n", bc, wc);
	 //	printf("BLACK: %ld\t WHITE: %ld\t\n", p1 == BLACK? p1time: p2time, p1 == WHITE? p1time: p2time);
	 //}

	 ///*getchar();
	 //getchar();
	 //*/

	 //num = 3;
	 //while (num--) {
	 //	p1time = 0;
	 //	p2time = 0;

	 //	Bitboard b(0x810000000, 0x1008000000);
	 //	b.printBoard();
	 //	Role tplayer = BLACK;
	 //	Role p2 = BLACK;
	 //	//char* p2_name = "human";
	 //	//char* p2_name = "mc_minmax";
	 //	//char* p2_name = "minmax_6_combine";
	 //	//char* p2_name = "imm_3";
	 //	char* p2_name = "mc_imm_3";
	 //	//char* p2_name = "mcts_-1";
	 //	Role p1 = WHITE;
	 //	char* p1_name = "mcts_3";
	 //	//char* p1_name = "imm_3";
	 //	//char* p1_name = "minmax_6_combine";
	 //	printf("BLACK: %s\t WHITE: %s\t\n", p1 == BLACK ? p1_name : p2_name, p1 == WHITE ? p1_name : p2_name);
	 //	int idx;
	 //	uint64_t act;
	 //	while (!b.hasEnded()) {
	 //		b.printBoard();
	 //		//printf("%s's turn\n", tplayer == p1 ? p1_name : p2_name);
	 //		if (tplayer == p1) {
	 //			bstart = clock();
	 //			Timer t(seconds);
	 //			act = run(p1_engine, tplayer, b, t);
	 //			bend = clock();
	 //			p1time += bend - bstart;
	 //		}
	 //		else {
	 //			wstart = clock();
	 //			Timer t(seconds);
	 //			//act = human(tplayer, b);
	 //			act = run(p2_engine, tplayer, b, t);
	 //			wend = clock();
	 //			p2time += wend - wstart;
	 //		}
	 //		if (act) {
	 //			b.takeAction(tplayer, act);
	 //			std::pair<int, int> ta = decode_action(act);
	 //			//printf("%s do : %d %d\n", tplayer == p1 ? p1_name : p2_name, ta.first + 1, ta.second + 1);
	 //		}
	 //		tplayer = change_player(tplayer);
	 //	}
	 //	b.printBoard();
	 //	std::pair<int, int> sc = b.getPieces();
	 //	int bc = sc.first;
	 //	int wc = sc.second;
	 //	if (bc > wc) printf("BLACK WINS\n");
	 //	else if (bc < wc) printf("WHITE WINS\n");
	 //	else printf("TIE\n");
	 //	printf("BLACK: %s\t WHITE: %s\t\n", p1 == BLACK ? p1_name : p2_name, p1 == WHITE ? p1_name : p2_name);
	 //	printf("BLACK: %d\t WHITE: %d\t\n", bc, wc);
	 //	printf("BLACK: %ld\t WHITE: %ld\t\n", p1 == BLACK? p1time: p2time, p1 == WHITE? p1time: p2time);
	 //}

	// battle();
	//pkmain();
	system("pause");
	return 0;
}
