#pragma once

#include "includes.h"
#include "types.h"
#include "move.h"

Move::Move() = default;

Move::Move(Piece p_piece, Position p_from_square, Position p_to_square, bool p_is_capture, Piece p_promoted_to) {
    piece = p_piece;
    from_square = p_from_square;
    to_square = p_to_square;
    is_capture = p_is_capture;
    promoted_to = p_promoted_to;
}

Move::Move(Castles p_castles) {
    castles = p_castles;
}

bool Move::operator==(Move other) {
    return (
        this->from_square == other.from_square &&
        this->to_square == other.to_square &&
        this->piece == other.piece &&
        this->promoted_to == other.promoted_to &&
        this->castles == other.castles &&
        this->is_capture == other.is_capture &&
        this->is_check == other.is_check &&
        this->is_checkmate == other.is_checkmate
    );
}

void Move::print_state() {
    if (castles == Castles::UNDEFINED) {
        std::cout << "Piece: " << piece_to_fen.at(piece) << '\n';
        if (promoted_to != Piece::EMPTY)
            std::cout << "Promoted to: " << piece_to_fen.at(promoted_to) << '\n';
        std::cout << "Source square: " << from_square.to_notation() << '\n';
        std::cout << "Target square: " << to_square.to_notation() << '\n';
        std::cout << "Is capture: " << humanize_bool(is_capture) << '\n';
    } else {
        std::cout << "Castles " << all_castles[(int) castles].second << '\n';
    }
    std::cout << "Is check: " << humanize_bool(is_check) << '\n';
    std::cout << "Is checkmate: " << humanize_bool(is_checkmate) << '\n';
}

std::string Move::to_string() {
    std::string res;
    if (castles != Castles::UNDEFINED) {
        if (castles == Castles::WHITE_KING || castles == Castles::BLACK_KING)
            res += "O-O";
        else
            res += "O-O-O";            
    } else {
        if (piece == Piece::EMPTY) return "<UNDEFINED PIECE>";

        if (piece != Piece::WHITE_PAWN && piece != Piece::BLACK_PAWN) {
            res += piece_to_fen.at(convert_piece_to_color(piece, Player::WHITE));
        }

        res += from_square.to_notation();
        if (is_capture) res += 'x';
        res += to_square.to_notation();

        if (promoted_to != Piece::EMPTY) {
            res += '=';
            res += piece_to_fen.at(convert_piece_to_color(promoted_to, Player::WHITE));
        }
    }

    if (is_checkmate) res += '#';
    else if (is_check) res += '+';

    return res;
}
