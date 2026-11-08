#pragma once

#include "position.h"
#include "types.h"
#include "move.h"
#include "evaluation.h"

class Board {
protected:
    // [0][0] is A8
    // [7][7] is H1
    Piece m_pieces[8][8];
    bool m_allowed_castles[4];
    int m_halfmove_clock = -1;
    int m_fullmove_clock = -1;
    GameState m_state = GameState::IN_PROGRESS;
    Position m_en_passant_target_square = INVALID_POS;
    Player m_to_move = Player::UNDEFINED;

public:
    Board();

    bool is_game_in_progress();

    static Board create_from_fen(std::string fen);

    std::string export_fen();

    Piece& piece_at(Position position);

    void print_state();

    void print_legal_moves();

    bool is_route_blocked(Piece piece, Position from, Position to);

    void execute_castles(Castles castles);

    Move parse_move(std::string move_string);

    bool is_castles_legal(Move move);

    bool is_move_legal(Move move);

    void execute_move_string(std::string move_string);

    void execute_move(Move move);

    std::vector<Move> get_legal_moves(Player side = Player::UNDEFINED, bool find_check_checkmate = true, int limit = -1);

    bool is_stalemate(Player to_move = Player::UNDEFINED);

    void after_move_checks();

    Position find_piece(Piece piece);

    // side is the side that would be in check
    bool is_in_check(Position pos, Player side, Position xray_pos = INVALID_POS);

    bool is_in_check_after(Move move, Position pos, Player side);

    Evaluation get_evaluation();

    double evaluate_material();

    bool is_checkmate(Player side);

    bool is_checkmate_after(Move move, Player side);

    Board new_board_from_move(Move move);
};