#pragma once

#include "includes.h"

#include "board.cpp"
#include "game_tree.cpp"

namespace Engine {

// naive: find M1 or direct material advantage
Move solve(Board &board, int max_depth) {
    GameTree tree = GameTree(board);

    tree.set_max_depth(max_depth);

    tree.solve();

    return tree.get_best_move();
}

};
