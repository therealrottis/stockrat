#pragma once

#include "move.h"
#include "board.h"
#include "game_tree.h"

#include "engine.h"

namespace Engine {
    // naive: find M1 or direct material advantage
    Move solve(Board &board, int max_depth) {
        GameTree tree = GameTree(board);

        tree.set_max_depth(max_depth);

        tree.solve();

        return tree.get_best_move();
    }
};
