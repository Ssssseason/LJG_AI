#include "uct.h"
#include "timer.h"

#include <iostream>

UCTNode::UCTNode(const Position &pos, shared_ptr<UCTNode> father, int color):
_father(father), _access_time(0), _reward(0), _value(pos), _color(color)
{
    if (!_father)
    {
        _access_time = 1; // root init
    }
}

UCTTree::UCTTree(const MatrixXd &board, bool black_turn, const Position &last_pos):
black_turn(black_turn), _board(board), _last_position(last_pos)
{
    int color = black_turn ? POS_WHITE: POS_BLACK;
    _root = std::make_shared<UCTNode>(last_pos, nullptr, color);
}

bool UCTTree::UCTSearch(double t, Position &best_pos)
{
    Timer T;
    int count = 0;
    while (T.elapsed_seconds() < t)
    {
        MatrixXd board_t(_board);
        bool turn(black_turn);
        shared_ptr<UCTNode> vl = UCTTreePolicy(_root, board_t, turn);
        if (_root == vl)
        {
            return false;
        }
        count++;
        int delta = UCTDefaultPolicy(vl, board_t, turn, black_turn);
        UCTBackup(vl, delta, black_turn);
    }
    // std::cout << count << std::endl;
    vector<shared_ptr<UCTNode>> children(_root->getChildren());
    double best_child_score = std::numeric_limits<double>::lowest();
    for (auto &child: children)
    {
        double score = double(child->getReward()) / child->getAccessTime();
        if (score > best_child_score)
        {
            best_child_score = score;
            best_pos = child->getPosition();
        }
    }
    // std::cout << '\n' << _board << std::endl;
    return true;
}

shared_ptr<UCTNode> UCTTreePolicy(shared_ptr<UCTNode> v, MatrixXd &board, bool &black_turn)
{
    shared_ptr<UCTNode> v_tmp = v;
    while (!reversiIsTerminal(board))
    {
        vector<int> valid_next_pos;
        if (!reversiNextValidPos(board, black_turn, valid_next_pos))
        {
            black_turn = !black_turn;
            return v_tmp;
        }
        vector<shared_ptr<UCTNode>> children_tmp(v_tmp->getChildren());
        double best_child_score = std::numeric_limits<double>::lowest();
        Position best_child_pos;
        shared_ptr<UCTNode> best_child;
        for (int i = 0; i != valid_next_pos.size() / 3; i++)
        {
            bool not_exist(true);
            for (auto &node: children_tmp)
            {
                if (node->getPosition().x() == valid_next_pos[3 * i] &&
                    node->getPosition().y() == valid_next_pos[3 * i + 1])
                {
                    not_exist = false;
                    double score = double(node->getReward()) / node->getAccessTime() +
                    c * std::sqrt(2 * std::log(v_tmp->getAccessTime()) / double(node->getAccessTime()));
                    if (score > best_child_score)
                    {
                        best_child_score = score;
                        best_child_pos = Position(node->getPosition());
                        best_child = node;
                    }
                    break;
                }
            }
            if (not_exist)
            {
                Position next_pos(valid_next_pos[3 * i], valid_next_pos[3 * i + 1]);
                shared_ptr<UCTNode> expand_child =
                std::make_shared<UCTNode>(next_pos, v_tmp, black_turn ? POS_BLACK: POS_WHITE);
                v_tmp->addChild(expand_child);
                reversiStepOnPos(board, next_pos, black_turn);
                black_turn = !black_turn;

                return expand_child;
            }
        }
        reversiStepOnPos(board, best_child_pos, black_turn);
        black_turn = !black_turn;
        v_tmp = best_child;
    }
    return v_tmp;
}

int UCTDefaultPolicy(shared_ptr<UCTNode> v, const MatrixXd &board, bool cur_turn, bool black_win)
{
    bool turn(cur_turn);
    MatrixXd board_t(board);

    std::random_device rd;
    std::mt19937 gen(rd());

    vector<int> nextPosSet;
    while (!reversiIsTerminal(board_t))
    {
        if (!reversiNextValidPos(board_t, turn, nextPosSet))
        {
            turn = !turn;
            if (!reversiNextValidPos(board_t, turn, nextPosSet))
                std::cout << "fatal error UCTDefaultPolicy" << std::endl;
        }
        int nvalid_pos = nextPosSet.size() / 3;
        std::uniform_int_distribution<> dis(0, nvalid_pos - 1);

        int idx = dis(gen);
        reversiStepOnPos(board_t, Position(nextPosSet[3 * idx], nextPosSet[3 * idx + 1]), turn);
        turn = !turn;
    }

    int nblack, nwhite;
    reversiColorCount(board_t, nblack, nwhite);
    auto sign = [](int x)
    {
        if (x > 0) return 1;
        else if (x == 0) return 0;
        else return -1;
    };
    return sign(black_win ? (nblack - nwhite) : (nwhite - nblack));
}

void UCTBackup(shared_ptr<UCTNode> v, int reward, bool black_win)
{
    shared_ptr<UCTNode> tmp = v;
    while (tmp)
    {
        tmp->addAccessTime();
        int factor = -1;
        if ((tmp->getColor() == POS_BLACK && black_win) || (tmp->getColor() == POS_WHITE && !black_win))
            factor = 1;
        tmp->addReward(factor * reward);
        //reward = -reward;
        tmp = tmp->getFather();
    }
}
