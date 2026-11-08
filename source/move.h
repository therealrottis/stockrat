#pragma once

#include "includes.h"

class Move {
public:
    Position from_square = INVALID_POS;
    Position to_square = INVALID_POS;
    Piece piece = Piece::EMPTY;
    Piece promoted_to = Piece::EMPTY;

    Castles castles = Castles::UNDEFINED;

    bool is_capture = false;
    bool is_check = false;
    bool is_checkmate = false;

    Move();

    Move(Piece p_piece, Position p_from_square, Position p_to_square, bool p_is_capture, Piece p_promoted_to = Piece::EMPTY);

    Move(Castles p_castles);

    bool operator==(Move other);

    void print_state();

    std::string to_string();
};

const Move INVALID_MOVE;
