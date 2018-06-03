#include "reversi.h"
#include <iostream>

bool utilOutOfBound(const Position &pos)
{
    return pos.x() < 0 || pos.y() < 0 || pos.x() >= REVERSI_HEIGHT || pos.y() >= REVERSI_WIDTH;
}

int utilCheckValidDir(const MatrixXd &board, const Position &cur_pos,
                      const Position &tar_pos, bool black_turn)
{
    int k = 1;
    int i = cur_pos.x(), j = cur_pos.y();
    int tari = tar_pos.x(), tarj = tar_pos.y();
    int vnp_color = black_turn ? POS_WHITE: POS_BLACK;
    int vp_color = black_turn ? POS_BLACK: POS_WHITE;
    int di = tari - i, dj = tarj - j;
    if (!utilOutOfBound(Position(tari, tarj)) && board(tari, tarj) == vnp_color)
    {
        while (!utilOutOfBound(Position(i + k * di, j + k * dj)))
        {
            int tmp_color = board(i + k * di, j + k * dj);
            if (tmp_color == vnp_color)
                k++;
            else if (tmp_color == vp_color)
                break;
            else if (tmp_color == POS_EMPTY)
                return 0;
        }

        if (utilOutOfBound(Position(i + k * di, j + k * dj)))
        {
            return 0;
        }
    }
    return k - 1;
}


void reversiInit(MatrixXd &board)
{
    board = MatrixXd::Zero(REVERSI_WIDTH, REVERSI_HEIGHT);
    board(REVERSI_WIDTH/2, REVERSI_HEIGHT/2)       = POS_WHITE;
    board(REVERSI_WIDTH/2-1, REVERSI_HEIGHT/2-1)   = POS_WHITE;
    board(REVERSI_WIDTH/2, REVERSI_HEIGHT/2-1)     = POS_BLACK;
    board(REVERSI_WIDTH/2-1, REVERSI_HEIGHT/2)     = POS_BLACK;
}

bool reversiNextValidPos(const MatrixXd &board, bool black_turn, vector<int> &validPosSet)
{
    validPosSet.clear();
    for (int i = 0; i < REVERSI_HEIGHT; i++)
    {
        for (int j = 0; j < REVERSI_WIDTH; j++)
        {
            if (board(i, j) != POS_EMPTY)
                continue;
            int reversal_count =utilCheckValidDir(board, Position(i, j), Position(i-1, j-1), black_turn) +
                                utilCheckValidDir(board, Position(i, j), Position(i-1, j)  , black_turn) +
                                utilCheckValidDir(board, Position(i, j), Position(i-1, j+1), black_turn) +
                                utilCheckValidDir(board, Position(i, j), Position(i, j-1)  , black_turn) +
                                utilCheckValidDir(board, Position(i, j), Position(i, j+1)  , black_turn) +
                                utilCheckValidDir(board, Position(i, j), Position(i+1, j-1), black_turn) +
                                utilCheckValidDir(board, Position(i, j), Position(i+1, j)  , black_turn) +
                                utilCheckValidDir(board, Position(i, j), Position(i+1, j+1), black_turn) ;
            if (reversal_count > 0)
            {
                validPosSet.push_back(i);
                validPosSet.push_back(j);
                validPosSet.push_back(reversal_count);
            }
        }
    }
    if (validPosSet.empty())
        return false;
    return true;
}

void reversiColorCount(const MatrixXd &board, int &nblack, int &nwhite)
{
    nblack = 0;
    nwhite = 0;
    for (int i = 0; i != REVERSI_HEIGHT; i++)
    {
        for (int j = 0; j != REVERSI_WIDTH; j++)
        {
            if (board(i, j) == POS_BLACK)
                nblack++;
            else if (board(i, j) == POS_WHITE)
                nwhite++;
        }
    }
}

bool reversiIsTerminal(const MatrixXd &board)
{
    for (int i = 0; i != REVERSI_HEIGHT; ++i)
    {
        for (int j = 0; j != REVERSI_WIDTH; ++j)
        {
            if (board(i, j) != POS_EMPTY)
                continue;
            if (utilCheckValidDir(board, Position(i, j), Position(i, j-1), true) > 0 ||
                utilCheckValidDir(board, Position(i, j), Position(i, j+1), true) > 0 ||
                utilCheckValidDir(board, Position(i, j), Position(i-1, j-1), true) > 0 ||
                utilCheckValidDir(board, Position(i, j), Position(i-1, j), true) > 0 ||
                utilCheckValidDir(board, Position(i, j), Position(i-1, j+1), true) > 0 ||
                utilCheckValidDir(board, Position(i, j), Position(i+1, j-1), true) > 0 ||
                utilCheckValidDir(board, Position(i, j), Position(i+1, j), true) > 0 ||
                utilCheckValidDir(board, Position(i, j), Position(i+1, j+1), true) > 0 ||
                utilCheckValidDir(board, Position(i, j), Position(i, j-1), false) > 0 ||
                utilCheckValidDir(board, Position(i, j), Position(i, j+1), false) > 0 ||
                utilCheckValidDir(board, Position(i, j), Position(i-1, j-1), false) > 0 ||
                utilCheckValidDir(board, Position(i, j), Position(i-1, j), false) > 0 ||
                utilCheckValidDir(board, Position(i, j), Position(i-1, j+1), false) > 0 ||
                utilCheckValidDir(board, Position(i, j), Position(i+1, j-1), false) > 0 ||
                utilCheckValidDir(board, Position(i, j), Position(i+1, j), false) > 0 ||
                utilCheckValidDir(board, Position(i, j), Position(i+1, j+1), false) > 0)
                {
                    return false;
                }
        }
    }
    return true;
}

bool reversiStepOnPos(MatrixXd &board, const Position &pos, bool black_turn)
{
    if (pos.x() < 0 || pos.x() >= REVERSI_HEIGHT ||
        pos.y() < 0 || pos.y() >= REVERSI_WIDTH)
    {
        return false;
    }

    if (board(pos.x(), pos.y()) != POS_EMPTY)
    {
        return false;
    }
    int vp_color = black_turn ? POS_BLACK: POS_WHITE;

    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
                continue;
            int count = utilCheckValidDir(board, pos, Position(pos.x() + i, pos.y() + j), black_turn);
            if (count > 0)
            {
                for (int k = 1; k <= count; k++)
                {
                    board(pos.x() + k * i, pos.y() + k * j) = vp_color;
                }
            }
        }
    }
    board(pos.x(), pos.y()) = vp_color;
    return true;
}
