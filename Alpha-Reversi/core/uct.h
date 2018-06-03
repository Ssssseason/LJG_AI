#include "reversi.h"

#include <random>
#include <cmath>
#include <memory>

#include <limits>

using std::shared_ptr;

constexpr double c = 0.5;

class UCTNode
{
public:
    UCTNode(const Position &pos, shared_ptr<UCTNode> father, int color);
    vector<shared_ptr<UCTNode>> getChildren() { return _children; }
    Position getPosition() const { return _value; }

    unsigned int getAccessTime() const { return _access_time; }
    int getReward() const { return _reward; }
    shared_ptr<UCTNode> getFather() { return _father; }
    int getColor() const { return _color; }

    void addAccessTime() { _access_time += 1; }
    void addReward(int delta) { _reward += delta; }
    void addChild(shared_ptr<UCTNode> child) { _children.push_back(child); }

private:
    Position _value;
    unsigned int _access_time;
    int _reward;

    int _color;

    vector<shared_ptr<UCTNode>> _children;
    shared_ptr<UCTNode> _father;
};

class UCTTree
{
public:
    UCTTree(const MatrixXd &board, bool black_turn, const Position &last_pos);
    bool UCTSearch(double t, Position &best_pos);
    MatrixXd _board;

private:
    shared_ptr<UCTNode> _root;


    Position _last_position;
    bool black_turn;
};


shared_ptr<UCTNode> UCTTreePolicy(shared_ptr<UCTNode> v, MatrixXd &board, bool &black_turn);
int UCTDefaultPolicy(shared_ptr<UCTNode> v, const MatrixXd &board, bool cur_turn, bool black_win);
void UCTBackup(shared_ptr<UCTNode> v, int reward, bool black_win);
