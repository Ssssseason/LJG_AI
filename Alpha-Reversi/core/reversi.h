#ifndef ALPHA_REVERSI_REVERSI_H
#define ALPHA_REVERSI_REVERSI_H

#include <vector>
#include <algorithm>
#include <iterator>

#include <eigen3\Eigen\Eigen>
//#include <eigen3\Eigen/Eigen>

#include <string>

using std::vector;
using std::string;
using Eigen::MatrixXd;

class Position
{
public:
    Position(const std::pair<int, int> &pos): _value(pos) {}
    Position(int x = 0, int y = 0): _value(x, y) {}
    int x() const { return _value.first;  }
    int y() const { return _value.second; }
    std::pair<int, int> data() const { return _value; }
private:
    std::pair<int, int> _value;
};

constexpr int REVERSI_WIDTH     = 8;
constexpr int REVERSI_HEIGHT    = 8;
constexpr int POS_BLACK         = 1;
constexpr int POS_WHITE         = -1;
constexpr int POS_EMPTY         = 0;

void reversiInit(MatrixXd &board);
bool reversiNextValidPos(const MatrixXd &board, bool black_turn, vector<int> &validPosSet);
void reversiColorCount(const MatrixXd &board, int &nblack, int &nwhite);
bool reversiIsTerminal(const MatrixXd &board);
bool reversiStepOnPos(MatrixXd &board, const Position &pos, bool black_turn);

bool utilOutOfBound(const Position &pos);
int  utilCheckValidDir(const MatrixXd &board, const Position &cur_pos,
                       const Position &tar_pos, bool black_turn);
#endif //ALPHA_REVERSI_REVERSI_H
