#include <cstdint>
#include <cassert>
#include <cstdio>

enum Role
{
	BLACK,
	WHITE
};

class Bitboard
{
public:
	// board:
	// 0, 1, 2, 3, 4, 5, 6, 7
	// 8, 9,11,12,13,14,15,16
	// ....
	uint64_t black, white;
	Bitboard(uint64_t black, uint64_t white) : black(black), white(white) {}

private:
	static int cntOfByte[256];
	static void initCntOfByte() {
		if (cntOfByte[255])
			return;
		cntOfByte[0] = 0;
		for (int i = 1; i < 256; i++) {
			cntOfByte[i] = cntOfByte[i / 2] + (i & 1);
		}
	}

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

	uint64_t getEmpty()
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
	actions |= shift##DIR(subOpp) & empty;\

public:
	uint64_t getActions(Role player)
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

#define reverseBoardIn(DIR)          \
	subOpp = 0;                      \
	tmp = action;                    \
	while (tmp)                      \
	{                                \
		tmp = shift##DIR(tmp) & opp; \
		subOpp |= tmp;               \
	}                                \
	if (shift##DIR(subOpp) & cur)                   \
	{                                \
		cur ^= subOpp;               \
		opp ^= subOpp;               \
	}

private:
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
	void takeAction(Role player, uint64_t action)
	{
		reverseBoard(player, action);
	}

	bool isValidAction(Role player, uint64_t action)
	{
		return action & getActions(player);
	}

	bool hasEnded()
	{
		return !(getActions(BLACK) | getActions(WHITE));
	}

	int getBlackNum() {
		initCntOfByte();
		return cntOfByte[black & 0xff] + cntOfByte[(black >> 8) & 0xff] + cntOfByte[(black >> 16) & 0xff] + cntOfByte[(black >> 24) & 0xff] + cntOfByte[(black >> 32) & 0xff]
			+ cntOfByte[(black >> 40) & 0xff] + cntOfByte[(black >> 48) & 0xff] + cntOfByte[(black >> 56) & 0xff];
	}

	int getWhiteNum() {
		initCntOfByte();
		return cntOfByte[white & 0xff] + cntOfByte[(white >> 8) & 0xff] + cntOfByte[(white >> 16) & 0xff] + cntOfByte[(white >> 24) & 0xff] + cntOfByte[(white >> 32) & 0xff]
			+ cntOfByte[(white >> 40) & 0xff] + cntOfByte[(white >> 48) & 0xff] + cntOfByte[(white >> 56) & 0xff];

	}

	void printBoard()
	{
		uint64_t b = black, w = white;
		printf("   ");
		for (int i = 0; i < 8; i++)
		{
			printf(" %c", 'a' + i);
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

	static	void printPawn(uint64_t p) {
		printf("   ");
		for (int i = 0; i < 8; i++)
		{
			printf(" %c", 'a' + i);
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

};

