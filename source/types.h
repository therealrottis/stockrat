#pragma once

#include "includes.h"
#include "position.h"

inline std::string humanize_bool(bool b) {
    return b ? "true" : "false";
}

enum class Piece : uint8_t {
    EMPTY,
    WHITE_PAWN,
    BLACK_PAWN,
    WHITE_KNIGHT,
    BLACK_KNIGHT,
    WHITE_BISHOP,
    BLACK_BISHOP,
    WHITE_ROOK,
    BLACK_ROOK,
    WHITE_QUEEN,
    BLACK_QUEEN,
    WHITE_KING,
    BLACK_KING
};

enum class Player : uint8_t {
    UNDEFINED,
    WHITE,
    BLACK
};

enum class Castles : int8_t {
    UNDEFINED = -1,
    WHITE_KING = 0,
    WHITE_QUEEN = 1,
    BLACK_KING = 2,
    BLACK_QUEEN = 3
};

const std::pair<Castles, std::string> all_castles[4] = {
    {Castles::WHITE_KING, "white kingside"},
    {Castles::WHITE_QUEEN, "white queenside"},
    {Castles::BLACK_KING, "black kingside"},
    {Castles::BLACK_QUEEN, "black queenside"},
};

enum class GameState : uint8_t {
    IN_PROGRESS,
    DRAW,
    WHITE_WIN,
    BLACK_WIN,
};

const std::map<GameState, std::string> state_to_string = {
    {GameState::IN_PROGRESS, "in progress"},
    {GameState::DRAW, "draw"},
    {GameState::WHITE_WIN, "white win"},
    {GameState::BLACK_WIN, "black win"},
};

// rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR
const std::map<char, Piece> fen_to_piece = {
    {'P', Piece::WHITE_PAWN},
    {'N', Piece::WHITE_KNIGHT},
    {'B', Piece::WHITE_BISHOP},
    {'R', Piece::WHITE_ROOK},
    {'Q', Piece::WHITE_QUEEN},
    {'K', Piece::WHITE_KING},
    
    {'p', Piece::BLACK_PAWN},
    {'n', Piece::BLACK_KNIGHT},
    {'b', Piece::BLACK_BISHOP},
    {'r', Piece::BLACK_ROOK},
    {'q', Piece::BLACK_QUEEN},
    {'k', Piece::BLACK_KING},
};

const std::map<Piece, char> piece_to_fen = {
    {Piece::EMPTY, ' '},
    
    {Piece::WHITE_PAWN, 'P'},
    {Piece::WHITE_KNIGHT, 'N'},
    {Piece::WHITE_BISHOP, 'B'},
    {Piece::WHITE_ROOK, 'R'},
    {Piece::WHITE_QUEEN, 'Q'},
    {Piece::WHITE_KING, 'K'},
    
    {Piece::BLACK_PAWN, 'p'},
    {Piece::BLACK_KNIGHT, 'n'},
    {Piece::BLACK_BISHOP, 'b'},
    {Piece::BLACK_ROOK, 'r'},
    {Piece::BLACK_QUEEN, 'q'},
    {Piece::BLACK_KING, 'k'},
};

const std::map<Piece, Piece> piece_to_opposite_color = {
    {Piece::WHITE_PAWN, Piece::BLACK_PAWN},
    {Piece::WHITE_KNIGHT, Piece::BLACK_KNIGHT},
    {Piece::WHITE_BISHOP, Piece::BLACK_BISHOP},
    {Piece::WHITE_ROOK, Piece::BLACK_ROOK},
    {Piece::WHITE_QUEEN, Piece::BLACK_QUEEN},
    {Piece::WHITE_KING, Piece::BLACK_KING},
    
    {Piece::BLACK_PAWN, Piece::WHITE_PAWN},
    {Piece::BLACK_KNIGHT, Piece::WHITE_KNIGHT},
    {Piece::BLACK_BISHOP, Piece::WHITE_BISHOP},
    {Piece::BLACK_ROOK, Piece::WHITE_ROOK},
    {Piece::BLACK_QUEEN, Piece::WHITE_QUEEN},
    {Piece::BLACK_KING, Piece::WHITE_KING},
};

const std::map<char, Castles> fen_to_castles = {
    {'K', Castles::WHITE_KING},
    {'Q', Castles::WHITE_QUEEN},

    {'k', Castles::BLACK_KING},
    {'q', Castles::BLACK_QUEEN},
};

const std::map<Castles, char> castles_to_fen = {
    {Castles::WHITE_KING, 'K'},
    {Castles::WHITE_QUEEN, 'Q'},

    {Castles::BLACK_KING, 'k'},
    {Castles::BLACK_QUEEN, 'q'},    
};

Player get_piece_color(Piece piece) {
    switch (piece) {
    case (Piece::WHITE_PAWN):
    case (Piece::WHITE_KNIGHT):
    case (Piece::WHITE_BISHOP):
    case (Piece::WHITE_ROOK):
    case (Piece::WHITE_QUEEN):
    case (Piece::WHITE_KING):
        return Player::WHITE;
    
    case (Piece::BLACK_PAWN):
    case (Piece::BLACK_KNIGHT):
    case (Piece::BLACK_BISHOP):
    case (Piece::BLACK_ROOK):
    case (Piece::BLACK_QUEEN):
    case (Piece::BLACK_KING):
        return Player::BLACK;
    
    }
    return Player::UNDEFINED;
}

Piece convert_piece_to_color(Piece piece, Player color) {
    if (get_piece_color(piece) == color) return piece;

    if (color == Player::UNDEFINED) {
        std::cerr << "Convert piece to color called with undefined player\n";
        return piece;
    }

    return piece_to_opposite_color.at(piece);
}

Castles move_to_castles(std::string move, Player side) {
    if (move == "O-O") {
        if (side == Player::WHITE) {
            return Castles::WHITE_KING;
        } else {
            return Castles::BLACK_KING;
        }
    } else if (move == "O-O-O") {
        if (side == Player::WHITE) {
            return Castles::WHITE_QUEEN;
        } else {
            return Castles::BLACK_QUEEN;
        }
    } else {
        std::cerr << "Invalid castling move " << move << '\n';
        return Castles::UNDEFINED;
    }
}

// does not take into account board state
bool piece_can_move(Piece piece, Position from, Position to) {
    int rank_delta = to.m_rank - from.m_rank;
    int file_delta = to.m_file - from.m_file;

    if (rank_delta == 0 && file_delta == 0) return false;

    int deltas[2];

    switch (piece) {
        case Piece::WHITE_PAWN:
            // direction: negative y
            if (file_delta != 0) return false;

            if (from.m_rank == Position::WHITE_HOME_RANK && rank_delta == -2)
                return true;
        
            return (rank_delta == -1);

        case Piece::BLACK_PAWN:
            // direction: positive y
            if (file_delta != 0) return false;
            
            if (from.m_rank == Position::BLACK_HOME_RANK && rank_delta == 2)
                return true;
            
            return (rank_delta == 1);

        case Piece::WHITE_KING:
        case Piece::BLACK_KING:
            return (rank_delta >= -1 && rank_delta <= 1 && file_delta >= -1 && file_delta <= 1);
        
        case Piece::WHITE_KNIGHT:
        case Piece::BLACK_KNIGHT:
            deltas[0] = abs(rank_delta);
            deltas[1] = abs(file_delta);
            
            if (deltas[0] < deltas[1])
                std::swap(deltas[0], deltas[1]);

            return (deltas[0] == 2 && deltas[1] == 1);
        
        case Piece::WHITE_BISHOP:
        case Piece::BLACK_BISHOP:
            return abs(file_delta) == abs(rank_delta);
        
        case Piece::WHITE_ROOK:
        case Piece::BLACK_ROOK:
            return file_delta == 0 || rank_delta == 0;
        
        case Piece::WHITE_QUEEN:
        case Piece::BLACK_QUEEN:
            return file_delta == 0 || rank_delta == 0 || abs(file_delta) == abs(rank_delta);
        
        default:
            std::cerr << "Invalid piece: \"" << piece_to_fen.at(piece) << "\"\n";
            return false;
    }
}

bool piece_can_capture(Piece piece, Position from, Position to) {
    int rank_delta = to.m_rank - from.m_rank;
    int file_delta = to.m_file - from.m_file;
    
    if (rank_delta == 0 && file_delta == 0) return false;

    switch (piece) {
        case Piece::WHITE_PAWN:
            // direction: negative y
            return rank_delta == -1 && abs(file_delta) == 1;

        case Piece::BLACK_PAWN:
            // direction: positive y
            return rank_delta == 1 && abs(file_delta) == 1;

        default:
            return piece_can_move(piece, from, to);
    }
    return true;
}

const std::pair<int, int> horse_deltas[] = {
    { 1,  2},
    { 1, -2},
    {-1,  2},
    {-1, -2},
    { 2,  1},
    { 2, -1},
    {-2,  1},
    {-2, -1},
};

const std::pair<int, int> all_directions[] = {
    { 1,  0},
    { 1,  1},
    { 0,  1},
    {-1,  1},
    {-1,  0},
    {-1, -1},
    { 0, -1},
    { 1, -1},
};

const std::pair<int, int> rook_directions[] = {
    { 1,  0},
    { 0,  1},
    {-1,  0},
    { 0, -1},
};

const std::pair<int, int> bishop_directions[] = {
    { 1,  1},
    {-1,  1},
    {-1, -1},
    { 1, -1},
};
