#ifndef BITBOARD_HPP
#define BITBOARD_HPP
#include <cstdint>
#include <cassert>
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;

enum Role
{
	BLACK,
	WHITE
};

class Bitboard
{
private:
	static unsigned char cntOfByte[256];
	static unsigned char roxanneWeights[64];
	static unsigned char indices[64];

public:
	// board:
	// 0, 1, 2, 3, 4, 5, 6, 7
	// 8, 9,11,12,13,14,15,16
	// ....
	uint64_t black, white;
	Bitboard(uint64_t black, uint64_t white) : black(black), white(white) {}

private:


	static uint64_t shiftLeft(uint64_t pawn)
	{
		return (pawn >> 1) & 0x7f7f7f7f7f7f7f7f;
	}

	static uint64_t shiftRight(uint64_t pawn)
	{
		return (pawn << 1) & 0xfefefefefefefefe;
	}

	static uint64_t shiftUp(uint64_t pawn)
	{
		return pawn >> 8;
	}

	static uint64_t shiftDown(uint64_t pawn)
	{
		return pawn << 8;
	}

	static uint64_t shiftUpLeft(uint64_t pawn)
	{
		return shiftUp(shiftLeft(pawn));
	}

	static uint64_t shiftUpRight(uint64_t pawn)
	{
		return shiftUp(shiftRight(pawn));
	}

	static uint64_t shiftDownLeft(uint64_t pawn)
	{
		return shiftDown(shiftLeft(pawn));
	}

	static uint64_t shiftDownRight(uint64_t pawn)
	{
		return shiftDown(shiftRight(pawn));
	}

	static int getPopCnt(uint64_t pawn)
	{
		return cntOfByte[pawn & 0xff] + cntOfByte[(pawn >> 8) & 0xff] + cntOfByte[(pawn >> 16) & 0xff] + cntOfByte[(pawn >> 24) & 0xff] + cntOfByte[(pawn >> 32) & 0xff] + cntOfByte[(pawn >> 40) & 0xff] + cntOfByte[(pawn >> 48) & 0xff] + cntOfByte[(pawn >> 56) & 0xff];
	}

	uint64_t getEmpty() const
	{
		return ~(black | white);
	}
#define getActionsIn(DIR)            \
	subOpp = 0;                      \
	tmp = cur;                       \
	while (tmp)                      \
	{                                \
		tmp = shift##DIR(tmp) & opp; \
		subOpp |= tmp;               \
	}                                \
	actions |= shift##DIR(subOpp) & empty;


#define reverseBoardIn(DIR)          \
	subOpp = 0;                      \
	tmp = action;                    \
	while (tmp)                      \
	{                                \
		tmp = shift##DIR(tmp) & opp; \
		subOpp |= tmp;               \
	}                                \
	if (shift##DIR(subOpp) & cur)    \
	{                                \
		cur ^= subOpp;               \
		opp ^= subOpp;               \
	}

	void reverseBoard(Role player, uint64_t action)
	{
		uint64_t cur, opp, subOpp, tmp;
		if (player == BLACK)
		{
			cur = black;
			opp = white;
		}
		else
		{
			cur = white;
			opp = black;
		}
		reverseBoardIn(Left);
		reverseBoardIn(Right);
		reverseBoardIn(Up);
		reverseBoardIn(Down);
		reverseBoardIn(UpLeft);
		reverseBoardIn(UpRight);
		reverseBoardIn(DownLeft);
		reverseBoardIn(DownRight);
		cur |= action;
		if (player == BLACK)
		{
			black = cur;
			white = opp;
		}
		else
		{
			black = opp;
			white = cur;
		}
	}

public:
	uint64_t getActions(Role player) const
	{
		uint64_t actions = 0;
		uint64_t cur, opp, subOpp, tmp;
		if (player == BLACK)
		{
			cur = black;
			opp = white;
		}
		else
		{
			cur = white;
			opp = black;
		}
		uint64_t empty = getEmpty();

		getActionsIn(Left);
		getActionsIn(Right);
		getActionsIn(Up);
		getActionsIn(Down);
		getActionsIn(UpLeft);
		getActionsIn(UpRight);
		getActionsIn(DownLeft);
		getActionsIn(DownRight);

		return actions;
	}

	void takeAction(Role player, uint64_t action)
	{
		reverseBoard(player, action);
	}

	bool isValidAction(Role player, uint64_t action)const
	{
		return action & getActions(player);
	}

	bool hasEnded()const
	{
		return !(getActions(BLACK) | getActions(WHITE));
	}

	pair<int, int> getPieces()const
	{
		return make_pair(getPopCnt(black), getPopCnt(white));
	}

	pair<int, int> getMobility()const
	{
		return make_pair(getPopCnt(getActions(BLACK)), getPopCnt(getActions(WHITE)));
		//return getPopCnt(getActions(BLACK)) - getPopCnt(getActions(WHITE));
	}

	pair<int, int> getProtMobility() const
	{
		uint64_t empty = getEmpty();
		uint64_t exEmpty = shiftLeft(empty) | shiftRight(empty) | shiftUp(empty) | shiftDown(empty) | shiftUpLeft(empty) | shiftUpRight(empty) |
			shiftDownLeft(empty) | shiftDownRight(empty);
		return make_pair(getPopCnt(exEmpty & white), getPopCnt(exEmpty & black));
		//return getPopCnt(exEmpty & white) - getPopCnt(exEmpty & black);
	}

	pair<int, int> getCorner() const
	{
		return make_pair(getPopCnt(0x8100000000000081 & black), getPopCnt(0x8100000000000081 & white));
		//return getPopCnt(0x8100000000000081 & black) - getPopCnt(0x8100000000000081 & white);
	}

	pair<int, int> getSubcorner() const {
		uint64_t emptyCorner = 0x8100000000000081 & getEmpty();
		int b = 0, w = 0;
		b = getPopCnt((shiftLeft(black) & emptyCorner)) 
		+ getPopCnt((shiftRight(black) & emptyCorner))
		+ getPopCnt((shiftUp(black) & emptyCorner))
		+ getPopCnt((shiftDown(black) & emptyCorner))
		+ getPopCnt((shiftUpLeft(black) & emptyCorner))
		+ getPopCnt((shiftUpRight(black) & emptyCorner))
		+ getPopCnt((shiftDownLeft(black) & emptyCorner))
		+ getPopCnt((shiftDownRight(black) & emptyCorner));

		w = getPopCnt((shiftLeft(white) & emptyCorner)) 
		+ getPopCnt((shiftRight(white) & emptyCorner))
		+ getPopCnt((shiftUp(white) & emptyCorner))
		+ getPopCnt((shiftDown(white) & emptyCorner))
		+ getPopCnt((shiftUpLeft(white) & emptyCorner))
		+ getPopCnt((shiftUpRight(white) & emptyCorner))
		+ getPopCnt((shiftDownLeft(white) & emptyCorner))
		+ getPopCnt((shiftDownRight(white) & emptyCorner));
		// printPawn(black);
		// printPawn(white);
		// printPawn(emptyCorner);
		// printf("%d %d\n",b,w );
		return make_pair(b, w);
	}

	pair<int, int>getRoxaneDotProduct() const {
		int b = 0, w = 0;
		for (int i = 0, tmp = 1; i < 64; i++, tmp << 1) {
			if (black & tmp) {
				b += roxanneWeights[i];
			}
			else if (white & tmp) {
				w += roxanneWeights[i];
			}
		}
		return make_pair(b, w);
	}

	static int scanForward(uint64_t bits) {
		assert(bits);
		return indices[((bits & -bits) * 0x03f79d71b4cb0a89) >> 58];
	}

	/**
	 * @brief Get the Roxanne Priority: when two actions have athe same estimated value, choose ont with the higher RoxannePriority.
	 *
	 * @param action
	 * @return int
	 */
	static int getRoxannePriority(uint64_t action) {
		return roxanneWeights[scanForward(action)];
	}


	double evaluate()const {
		// will be removed
		pair<int, int> pMob = getMobility();
		pair<int, int> pPMob = getProtMobility();
		pair<int, int> pCor = getCorner();
		pair<int, int> pRox = getRoxaneDotProduct();
		pair<int, int> pPie = getPieces();
		int mob = pMob.first - pMob.second;
		int pmob = pPMob.first - pPMob.second;
		int cor = pCor.first - pCor.second;
		// int rox = pRox.first - pRox.second;
		int pie = pPie.first - pPie.second;
		return mob + pmob + cor * 80 + pie * 80;
		// return m + p + c * 80 + r * 80;
	}

	static double evaluateCombine(const Bitboard &b) {

		pair<int, int> pm = b.getMobility();
		pair<int, int> pp = b.getProtMobility();
		pair<int, int> pc = b.getCorner();
		pair<int, int> pr = b.getRoxaneDotProduct();
		pair<int, int> ps = b.getSubcorner();
		int m = pm.first - pm.second;
		int p = pp.first - pp.second;
		int c = pc.first - pc.second;
		int r = pr.first - pr.second;
		int s = ps.first - ps.second;
		/*printf("Mobility: %d\n", m);
		printf("ProbMobility: %d\n", p);
		printf("Corner: %d\n", c);*/
		//return m + p + c * 80 + r * 10 - 40 * s;
		return m /64.0+ p/32.0 + c /4.0 + r  - s/8.0;
		//int m = b.getMobility();
		//int p = b.getProtMobility();
		//int c = b.getCorner();
		//return m + p + c * 80;
	}

	void printBoard() const
	{
		uint64_t b = black, w = white;
		printf("   ");
		for (int i = 0; i < 8; i++)
		{
			printf(" %d", i + 1);
		}
		printf("\n");

		for (int i = 0; i < 64; i++)
		{
			if (i % 8 == 0)
			{
				printf(" %d ", i / 8 + 1);
			}
			assert(!(b & 1 && w & 1));
			printf(b & 1 ? "|X" : (w & 1 ? "|O" : "|_"));
			b >>= 1;
			w >>= 1;
			printf(i % 8 == 7 ? "|\n" : "");
		}
	}

	static void printPawn(uint64_t p)
	{
		printf("   ");
		for (int i = 0; i < 8; i++)
		{
			printf(" %d", i + 1);
		}
		printf("\n");

		for (int i = 0; i < 64; i++)
		{
			if (i % 8 == 0)
			{
				printf(" %d ", i / 8 + 1);
			}
			printf(p & 1 ? "|@" : "|_");
			p >>= 1;
			printf(i % 8 == 7 ? "|\n" : "");
		}
	}

	static void init()
	{
		if (cntOfByte[255])
			return;
		cntOfByte[0] = 0;
		for (int i = 1; i < 256; i++)
		{
			cntOfByte[i] = cntOfByte[i / 2] + (i & 1);
		}
		// unsigned char weights[64] = {
		// 	1,5,3,3,3,3,5,1,
		// 	5,5,4,4,4,4,5,5,
		// 	3,4,2,2,2,2,4,3,
		// 	3,4,2,0,0,2,4,3,
		// 	3,4,2,0,0,2,4,3,
		// 	3,4,2,2,2,2,4,3,
		// 	5,5,4,4,4,4,5,5,
		// 	1,5,3,3,3,3,5,1
		// };
		// unsigned char weights[64] = {
		// 	5,1,3,3,3,3,1,5,
		// 	1,1,2,2,2,2,1,1,
		// 	3,2,4,4,4,4,2,3,
		// 	3,2,4,0,0,4,2,3,
		// 	3,2,4,0,0,4,2,3,
		// 	3,2,4,4,4,4,2,3,
		// 	1,1,2,2,2,2,1,1,
		// 	5,1,3,3,3,3,1,5
		// };
		char weights[64] = {
			20, -3, 11,  8,  8, 11, -3, 20,
		    -3, -7, -4,  1,  1, -4, -7, -3,
		    11, -4,  2,  2,  2,  2, -4, 11,
		     8,  1,  2, -3, -3,  2,  1,  8,
		     8,  1,  2, -3, -3,  2,  1,  8,
		    11, -4,  2,  2,  2,  2, -4, 11,
		    -3, -7, -4,  1,  1, -4, -7, -3,
		    20, -3, 11,  8,  8, 11, -3, 20,
			//  4,-4, 3, 1, 1, 3,-4, 4,
			// -4,-5,-3,-2,-2,-3,-5,-4,
			//  3,-3, 2, 0, 0, 2,-3, 3,
			//  1,-2, 0,-1,-1, 0,-2,-1,
			//  1,-2, 0,-1,-1, 0,-2,-1,
			//  3,-3, 2, 0, 0, 2,-3, 3,
			// -4,-5,-3,-2,-2,-3,-5,-4,
			//  4,-4, 3, 1, 1, 3,-4, 4
		};
		unsigned char in[64] = {
			0,  1, 48,  2, 57, 49, 28,  3,
			61, 58, 50, 42, 38, 29, 17,  4,
			62, 55, 59, 36, 53, 51, 43, 22,
			45, 39, 33, 30, 24, 18, 12,  5,
			63, 47, 56, 27, 60, 41, 37, 16,
			54, 35, 52, 21, 44, 32, 23, 11,
			46, 26, 40, 15, 34, 20, 31, 10,
			25, 14, 19,  9, 13,  8,  7,  6
		};
		memcpy(roxanneWeights, weights, 64);
		memcpy(indices, in, 64);
	}
};
#endif
