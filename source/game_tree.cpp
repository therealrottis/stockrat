#pragma once

#include "includes.h"
#include "board.h"
#include "types.h"
#include "move.h"
#include "game_tree.h"

GameTree::GameTree(Board &p_board, Move p_previous_move, int p_depth) {
    board = p_board;
    eval = board.get_evaluation();
    previous_move = p_previous_move;
    depth = p_depth;
}

void GameTree::set_max_depth(int p_max_depth) {
    depth = p_max_depth;
}

bool GameTree::is_solved() {
    return solved;
}

bool GameTree::operator<(GameTree &other) {
    // evaluation of this node depends on the evaluation of child nodes
    return eval < other.eval;
}

void GameTree::solve() {
    solved = true;
    //std::cout << "Solving... depth: " << depth << '\n';
    //board.print_state();
    if (depth < 0) return;

    for (Move move : board.get_legal_moves()) {
        Board new_board = board.new_board_from_move(move);
        
        moves.push_back(GameTree(new_board, move, depth - 1));
    }

    if (moves.empty()) return;

    sort_moves();

    int runs_left = 50;
    
    while (runs_left-- > 0 && !(moves.begin()->eval.is_win() || moves.begin()->eval.is_loss())) {
        bool no_progress = true;

        for (GameTree &tree : moves) {
            if (!tree.is_solved() && !tree.eval.is_draw()) {
                tree.solve();
                no_progress = false;
                break;
            }
        }
        if (no_progress) break;
        
        sort_moves();
        /*std::cout << std::endl;
        for (GameTree tree : moves) {
            std::cout << tree.previous_move.to_string() << ' ';
            tree.eval.print_eval();
        }
        std::cout << std::endl;*/
    }

    eval.inherit_from(moves.begin()->eval);
}

inline void GameTree::sort_moves() {
    std::sort(moves.begin(), moves.end());
}

Move GameTree::get_best_move() {
    if (moves.empty()) return INVALID_MOVE;
    
    return moves.begin()->previous_move;
}
