#pragma once

#include "board.h"
#include "evaluation.h"
#include "move.h"

class GameTree {
private:
    Board board;
    Evaluation eval;
    std::vector<GameTree> moves;
    Move previous_move = INVALID_MOVE;
    int depth = 1;
    bool solved = false;

public:
    GameTree(Board &p_board, Move p_previous_move = INVALID_MOVE, int p_depth = 1);

    void set_max_depth(int p_max_depth);

    bool is_solved();

    bool operator<(GameTree &other);

    void solve();

    inline void sort_moves();

    Move get_best_move();
};
