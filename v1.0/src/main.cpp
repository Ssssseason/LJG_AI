#include <cstdio>
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
string changecolor="";

double evaluate_combine(Bitboard b) {
	return Bitboard::evaluateCombine(b);
}

action human(Role player, Bitboard board) {
	int x, y;
	scanf("%d%d", &x, &y);
	return encode_action(x - 1, y - 1);
}


action p1_engine(Role player, Bitboard b, Timer t) {
	return IMM_imm(b, player, 3, 0, t);
}

action p2_engine(Role player, Bitboard b, Timer t) {
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
    
    //b.printBoard();
    string SERVER_IPSERVER_IP="http://10.180.58.5:5000/";
    id="2";
    string res="";
    double timeSum=0;
    double time = 0;
    
    Role ply;
    while(player==""){
        player=tcurl(SERVER_IPSERVER_IP+"create_session/"+id);
        sleep(2);
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
            cout << turn <<endl;
            
        }
        while(qipan==""){
            qipan=tcurl(SERVER_IPSERVER_IP+"board_string/"+id);
            cout << qipan<<endl;
        }
        changeData(qipan,&b);
        b.printBoard();
        //if(b.hasEnded()) break;
        if(turn==player){
            
            Timer t=10;
            action temp=run(p1_engine, ply, b, t);
            time = t.getTimePassed();
            timeSum += time;
            printf("%lf\n", time);
            if(temp == 0){
            	changecolor=tcurl(SERVER_IPSERVER_IP+"change_color/"+id);
            }
            else {
                pair<int,int> xy = decode_action(temp);
	            printf("%d %d\n", xy.first, xy.second);
	            while(res!="SUCCESS" && res!="ERROR") {
	                res = tcurl(xy.first+'0', xy.second+'0');
	                cout <<res<<endl;
	                // if(res=="ERROR") errorCount++;
	                // if(errorCount>1) break;
	            }
	            b.takeAction(ply, temp);
	        }
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

	return 0;
}
