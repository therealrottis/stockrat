#pragma once

#include "move.h"
#include "board.h"

namespace Engine {
    // naive: find M1 or direct material advantage
    Move solve(Board &board, int max_depth);
};
