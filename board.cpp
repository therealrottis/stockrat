#pragma once

#include "includes.h"

class Position {
public:
    const static int8_t WHITE_HOME_RANK = 6;
    const static int8_t BLACK_HOME_RANK = 1; 
    
    int8_t m_rank = -1;
    int8_t m_file = -1;

    static Position average(Position first, Position second) {
        Position retval;
        
        retval.m_file = (first.m_file + second.m_file) / 2;
        retval.m_rank = (first.m_rank + second.m_rank) / 2;
        
        return retval;
    }

    Position() = default;

    Position(std::string str) {
        if (str.size() != 2) {
            std::cerr << "Coordinates notation has invalid length, notation: " << str << '\n';
            return;
        }

        char file = str[0];
        char rank = str[1];
        if (file >= 'a' && file <= 'h') {
            file -= ('a' - 'A');
        }

        if ((file < 'A' || file > 'H') && file != '?') {
            std::cerr << "Notation has invalid file \"" << file << "\".\n";
            return;
        }

        if ((rank < '1' || rank > '8') && rank != '?') {
            std::cerr << "Notation has invalid rank \"" << rank << "\".\n";
            return;
        }

        if (rank != '?') {
            m_rank = '8' - rank;
        }

        if (file != '?') {
            m_file = file - 'A';
        }
    }

    Position(int8_t rank, int8_t file) {
        m_rank = rank;
        m_file = file;
    }

    bool operator==(Position other) {
        return m_file == other.m_file && m_rank == other.m_rank;
    }

    bool operator!=(Position other) {
        return !(*this == other);
    }

    Position operator+(std::pair<int, int> p) {
        Position new_pos = *this;
        new_pos.m_rank += p.first;
        new_pos.m_file += p.second;
        return new_pos;
    }

    Position operator+=(std::pair<int, int> p) {
        *this = *this + p;
        return *this;
    }

    std::string to_notation() {
        std::string str;

        if (m_file < 0 || m_file >= 8)
            str.push_back('?');
        else
            str.push_back('a' + m_file);
        
        if (m_rank < 0 || m_rank >= 8)
            str.push_back('?');
        else
            str.push_back('8' - m_rank);

        return str;
    }

    bool is_in_bounds() {
        return m_file >= 0 && m_file < 8 && m_rank >= 0 && m_rank < 8;
    }

    std::vector<Position> interpolate(Position target_pos) {
        std::vector<Position> res;
        
        Position cur = *this;

        while (cur != target_pos) {
            if (cur.m_file < target_pos.m_file) cur.m_file++;
            else if (cur.m_file > target_pos.m_file) cur.m_file--;

            if (cur.m_rank < target_pos.m_rank) cur.m_rank++;
            else if (cur.m_rank > target_pos.m_rank) cur.m_rank--;
            res.push_back(cur);
        }
        // skip target square
        if (!res.empty())
            res.pop_back();
        return res;
    }
};

const Position INVALID_POS = Position(-1, -1);

std::string humanize_bool(bool b) {
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

enum class GameState {
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

    Move() = default;

    Move(Piece p_piece, Position p_from_square, Position p_to_square, bool p_is_capture, Piece p_promoted_to = Piece::EMPTY) {
        piece = p_piece;
        from_square = p_from_square;
        to_square = p_to_square;
        is_capture = p_is_capture;
        promoted_to = p_promoted_to;
    }

    Move(Castles p_castles) {
        castles = p_castles;
    }

    // strict weak ordering!!!
    constexpr bool operator<(const Move &other) const {
        return this < &other;
    }

    bool operator==(Move other) {
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

    void print_state() {
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

    std::string to_string() {
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
};

const Move INVALID_MOVE;


class Evaluation {
private:
    Player player = Player::UNDEFINED;
    GameState state = GameState::IN_PROGRESS;
    double material = 0.0;
    bool is_check = false;

public:
    Evaluation() = default;
    
    Evaluation(Player p_player, GameState p_state, double p_material, bool p_is_check) {
        player = p_player;
        state = p_state;
        material = p_material;
        is_check = p_is_check;
    }

    void inherit_from(Evaluation &other) {
        state = other.state;
        material = other.material;
    }

    bool is_valid() {
        return player != Player::UNDEFINED;
    }

    void print_eval() {
        std::cout << "Player: " << (player == Player::WHITE ? "white" : "black") << ", material: " << material;
        std::cout << ", state: " << state_to_string.at(state) << '\n';
    }

    // smaller in order means better position for current player
    bool operator<(const Evaluation &other) const {
        if (state == GameState::IN_PROGRESS && other.state == GameState::IN_PROGRESS) {
            if (player != other.player) {
                std::cerr << "Comparing two Evaluations with non-equal player.\n";
                std::cerr << this << ' ' << (int) player << ' ' << (int) state << ' ' << material << '\n';
                std::cerr << &other << ' ' << (int) other.player << ' ' << (int) other.state << ' ' << other.material << std::endl;
                return false;
            }
            
            // TODO fix? ei toimi?
            bool equal_material = abs(material - other.material) < 1.0;
            if (equal_material && is_check && !other.is_check) return false;
            if (equal_material && !is_check && other.is_check) return true;

            if (player == Player::BLACK) return material < other.material;
            else return other.material < material;

        } else if (state == other.state) return false; // doesn't really matter, strict weak ordering
        
        if (is_win() || other.is_loss()) return true; // self
        if (is_loss() || other.is_win()) return false; // other

        // no win/loss found, one game is a draw and the other is in progress
        // if down material, draw is better
        // if up material, in progress is better

        if (state == GameState::DRAW) {
            return !other.is_up_material(); // down material: draw (self), up material: continue (other)
        } else if (other.state == GameState::DRAW) {
            return is_up_material(); // up material: continue (self), down material: draw (other)
        } else {
            std::cerr << "Evaluation comparator found invalid game states.\n";
            std::cerr << this << ' ' << (int) player << ' ' << (int) state << ' ' << material << '\n';
            std::cerr << &other << ' ' << (int) other.player << ' ' << (int) other.state << ' ' << other.material << std::endl;
            return false;
        }
    }
    
    template <typename T>
    static bool PairComparator(const std::pair<Evaluation, T> &first, const std::pair<Evaluation, T> &second) {
        return first.first < second.first;
    }

    bool operator==(const Evaluation &other) const {
        return (
            player == other.player &&
            state == other.state &&
            material == other.material
        );
    }

    inline bool is_win() const {
        return (
            (player == Player::WHITE && state == GameState::WHITE_WIN) ||
            (player == Player::BLACK && state == GameState::BLACK_WIN)
        );
    }

    inline bool is_draw() const {
        return state == GameState::DRAW;
    }

    inline bool is_loss() const {
        return (
            (player == Player::BLACK && state == GameState::WHITE_WIN) ||
            (player == Player::WHITE && state == GameState::BLACK_WIN)
        );
    }

    inline double is_up_material() const {
        return (
            (player == Player::WHITE && material > 0.0) ||
            (player == Player::BLACK && material < 0.0)   
        );
    }
};

class Board {
protected:
    // [0][0] is A8
    // [7][7] is H1
    Piece m_pieces[8][8];
    Player m_to_move = Player::UNDEFINED;
    bool m_allowed_castles[4];
    // y, x
    Position m_en_passant_target_square = INVALID_POS;
    int m_halfmove_clock = -1;
    int m_fullmove_clock = -1;
    GameState m_state = GameState::IN_PROGRESS;

public:
    Board () {
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                m_pieces[y][x] = Piece::EMPTY;
            }
        }

        m_to_move = Player::WHITE;   
    };

    bool is_game_in_progress() {
        return m_state == GameState::IN_PROGRESS;
    }

    static Board create_from_fen(std::string fen) {
        // rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
        Board board;

        int step = 0;
        int rank = 0;
        int file = 0;
        std::string en_passant_buffer, halfmove_buffer, fullmove_buffer;

        int white_king_count = 0;
        int black_king_count = 0;

        for (char c : fen) {
            if (c == ' ') {
                step++;
                continue;
            }

            switch (step) {
            case 0: // rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR
                if (c == '/') {
                    rank++;
                    file = 0;
                } else {
                    if (c >= '1' && c <= '8') {
                        file += (c - '0');
                    } else {
                        if (file >= 8 || rank >= 8) {
                            std::cerr << "Out of bounds coordinates while parsing FEN: (" << file << ", " << rank << ")\n";
                            break;
                        }

                        if (fen_to_piece.count(c) == 0) {
                            std::cerr << "Invalid piece \"" << c << "\" while parsing FEN\n";
                            break;
                        }

                        Piece piece = fen_to_piece.at(c);
                        board.m_pieces[rank][file] = piece;
                        file++;
                        if (piece == Piece::WHITE_KING) white_king_count++;
                        else if (piece == Piece::BLACK_KING) black_king_count++;
                        
                        if ((rank == 0 || rank == 7) && (piece == Piece::WHITE_PAWN || piece == Piece::BLACK_PAWN)) {
                            std::cerr << "Pawn found in invalid rank\n";
                        }
                    }
                }
                break;
            case 1: // w or b
                if (c == 'w') {
                    board.m_to_move = Player::WHITE;
                } else if (c == 'b') {
                    board.m_to_move = Player::BLACK;
                } else {
                    std::cerr << "Invalid player to move \"" << c << "\" while parsing FEN.\n";
                }
                break;
            case 2: // KQkq
                if (c == '-') break;

                if (fen_to_castles.count(c) == 0) {
                    std::cerr << "Invalid castles notation \"" << c << "\" while parsing FEN.\n";
                    break;
                }

                board.m_allowed_castles[(int) (fen_to_castles.at(c))] = true;

                break;
            case 3: // e6 or - (en passant target square)
                if (c == '-') break;
                en_passant_buffer.push_back(c);
                break;
            case 4: // halfmove clock since capture or pawn move
                halfmove_buffer.push_back(c);
                break;
            case 5: // fullmove clock
                fullmove_buffer.push_back(c);
                break;
            default:
                std::cerr << "FEN is too long, current step: " << step << '\n';
            }
        }

        if (white_king_count != 1) {
            std::cerr << "Invalid FEN: 1 white king expected, " << white_king_count << " found\n";
        }

        if (black_king_count != 1) {
            std::cerr << "Invalid FEN: 1 black king expected, " << black_king_count << " found\n";
        }

        if (!en_passant_buffer.empty())
            board.m_en_passant_target_square = Position(en_passant_buffer);

        board.m_halfmove_clock = std::stoi(halfmove_buffer);
        board.m_fullmove_clock = std::stoi(fullmove_buffer);

        board.after_move_checks();

        return board;
    }

    std::string export_fen() {
        std::string res;

        // rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR
        for (int rank = 0; rank < 8; rank++) {
            int cur = 0;
            for (int file = 0; file < 8; file++) {
                if (m_pieces[rank][file] == Piece::EMPTY) cur++;
                else {
                    if (cur > 0) res.push_back('0' + cur);
                    cur = 0;
                    res.push_back(piece_to_fen.at(m_pieces[rank][file]));
                }
            }
            if (cur > 0) res.push_back('0' + cur);

            if (rank < 7) res.push_back('/');
            else res.push_back(' ');
        }

        if (m_to_move == Player::WHITE) {
            res.append("w ");
        } else {
            res.append("b ");
        }

        bool has_castles = true;
        for (int i = 0; i < 4; i++) {
            if (m_allowed_castles[i]) has_castles = true;
        }

        if (has_castles) {
            for (std::pair<Castles, std::string> castle : all_castles) {
                if (m_allowed_castles[(int) castle.first]) res.push_back(castles_to_fen.at(castle.first));
            }
        } else {
            res.push_back('-');
        }
        res.push_back(' ');

        if (m_en_passant_target_square.is_in_bounds()) {
            res.append(m_en_passant_target_square.to_notation());
        } else {
            res.push_back('-');
        }
        res.push_back(' ');

        res.append(std::to_string(m_halfmove_clock));
        res.push_back(' ');

        res.append(std::to_string(m_fullmove_clock));

        return res;
    }

    Piece& piece_at(Position position) {
        return m_pieces[position.m_rank][position.m_file];
    }

    void print_state() {
        /*
            Piece m_pieces[8][8];
            Player m_to_move = Player::WHITE;
            bool m_allowed_castles[4];
            // y, x
            std::pair<int, int> m_en_passant_target_square{};
            int m_halfmove_clock = 0;
            int m_fullmove_clock = 0;
        */
        if (m_to_move == Player::WHITE) {
            std::cout << "White to move\n\n";
        } else if (m_to_move == Player::BLACK) {
            std::cout << "Black to move\n\n";
        } else {
            std::cout << "Undefined to move\n\n";
        }

        std::cout << "  A B C D E F G H  \n";
        for (int rank = 0; rank < 8; rank++) {
            std::cout << 8-rank << ' ';
            for (int file = 0; file < 8; file++) {
                std::cout << piece_to_fen.at(m_pieces[rank][file]) << ' ';
            }
            std::cout << 8-rank << '\n';
        }
        std::cout << "  A B C D E F G H  \n";

        std::vector<std::string> castles_strings;
        for (std::pair<Castles, std::string> p : all_castles) {
            if (m_allowed_castles[(int) p.first])
                castles_strings.push_back(p.second);
        }

        if (castles_strings.empty()) {
            std::cout << "\nNo castles available\n";
        } else {
            std::cout << "\nAvailable castles: ";
            bool first = true;
            for (std::string s : castles_strings) {
                if (!first) {
                    std::cout << ", ";
                }
                first = false;
                std::cout << s;
            }
            std::cout << '\n';
        }

        /*
            std::pair<int, int> m_en_passant_target_square{};
            int m_halfmove_clock = 0;
            int m_fullmove_clock = 0;
        */

        std::cout << "En passant target square: " << m_en_passant_target_square.to_notation() << '\n';
        std::cout << "Halfmove clock since capture or pawn move: " << m_halfmove_clock << '\n';
        std::cout << "Fullmove clock: " << m_fullmove_clock << '\n';
    }

    void print_legal_moves() {
        std::cout << "Legal moves: ";
        bool first = true;
        for (Move move : get_legal_moves()) {
            if (!first) std::cout << ", ";
            first = false;

            std::cout << move.to_string();
        }
        std::cout << '\n';
    }

    bool is_route_blocked(Piece piece, Position from, Position to) {
        switch (piece) {
            case Piece::WHITE_PAWN:
            case Piece::BLACK_PAWN:
            case Piece::WHITE_BISHOP:
            case Piece::BLACK_BISHOP:
            case Piece::WHITE_ROOK:
            case Piece::BLACK_ROOK:
            case Piece::WHITE_QUEEN:
            case Piece::BLACK_QUEEN:
                // all squares in the path must be empty
                for (Position pos : from.interpolate(to)) {
                    if (m_pieces[pos.m_rank][pos.m_file] != Piece::EMPTY) {
                        return true;
                    }
                }
        }
        return false;
    }

    void execute_castles(Castles castles) {
        switch (castles) {
            case Castles::WHITE_QUEEN:
                m_pieces[7][0] = Piece::EMPTY; // rook
                m_pieces[7][4] = Piece::EMPTY; // king
                m_pieces[7][3] = Piece::WHITE_ROOK; // rook target
                m_pieces[7][2] = Piece::WHITE_KING; // king target
                break;

            case Castles::BLACK_QUEEN:
                m_pieces[0][0] = Piece::EMPTY; // rook
                m_pieces[0][4] = Piece::EMPTY; // king
                m_pieces[0][3] = Piece::BLACK_ROOK; // rook target
                m_pieces[0][2] = Piece::BLACK_KING; // king target
                break;
            
            case Castles::WHITE_KING:
                m_pieces[7][7] = Piece::EMPTY; // rook
                m_pieces[7][4] = Piece::EMPTY; // king
                m_pieces[7][5] = Piece::WHITE_ROOK; // rook target
                m_pieces[7][6] = Piece::WHITE_KING; // king target
                break;

            case Castles::BLACK_KING:
                m_pieces[0][7] = Piece::EMPTY; // rook
                m_pieces[0][4] = Piece::EMPTY; // king
                m_pieces[0][5] = Piece::BLACK_ROOK; // rook target
                m_pieces[0][6] = Piece::BLACK_KING; // king target
                break;
        }
        
        if (m_to_move == Player::WHITE) {
            m_allowed_castles[(int) Castles::WHITE_KING] = false;
            m_allowed_castles[(int) Castles::WHITE_QUEEN] = false;
            m_to_move = Player::BLACK;
        } else if (m_to_move == Player::BLACK) {
            m_allowed_castles[(int) Castles::BLACK_KING] = false;
            m_allowed_castles[(int) Castles::BLACK_QUEEN] = false;
            m_to_move = Player::WHITE;
            m_fullmove_clock++;
        }
        m_halfmove_clock++;

        after_move_checks();
    }

    Move parse_move(std::string move_string) {
        Move move;
        
        if (move_string[0] == 'O') {
            if (move_string.size() == 4 || move_string.size() == 6) {
                if (move_string.back() == '+') {
                    move.is_check = true;
                } else if (move_string.back() == '#') {
                    move.is_checkmate = true;
                } else {
                    std::cerr << "Invalid castling move " << move_string << "\n";
                    return INVALID_MOVE;
                }
                move_string.pop_back();
            }

            if (m_to_move == Player::UNDEFINED) {
                std::cerr << "Undefined player while executing castles move\n";
                return INVALID_MOVE;
            }

            Castles castles = move_to_castles(move_string, m_to_move);

            move.castles = castles;
        }

        std::string squares_buffer;
        // may be partially defined, only rank or file
        
        int step = 0;
        for (char c : move_string) {
            switch (step) {
            case 0:
                step++;
                // (possibly) piece
                if (c >= 'A' && c <= 'Z') {
                    if (fen_to_piece.count(c) == 0) {
                        std::cerr << "Invalid piece " << c << " found\n";
                        return INVALID_MOVE;
                    }
                    move.piece = fen_to_piece.at(c);
                    break;
                } else {
                    move.piece = Piece::WHITE_PAWN;
                }
                [[fallthrough]];
            case 1:
                // disambiguated source square
                // note: with moves such as Ne4 the target square is also parsed here
                // still works

                if (c == 'x' || c == '+' || c == '#' || c == '=') {
                    step++;
                } else {
                    squares_buffer.push_back(c);
                    break;
                }
                [[fallthrough]];
            case 2:
                step++;
                // capture/no capture
                if (c == 'x') {
                    move.is_capture = true;
                    break;
                }
                [[fallthrough]];
            case 3:
                // target square

                if (c == '#' || c == '+' || c == '=') {
                    step++;
                } else {
                    squares_buffer.push_back(c);
                    break;
                }
                [[fallthrough]];
            case 4:
                // equals sign in promotion
                if (c == '=') {
                    step++;
                    break;
                } else {
                    step++;
                }
                [[fallthrough]];
            case 5:
                if (c != '+' && c != '#') {
                    if (fen_to_piece.count(c) == 0) {
                        std::cerr << "Cannot promote to invalid piece \"" << c << "\"\n";
                        return INVALID_MOVE;
                    }
                    move.promoted_to = convert_piece_to_color(fen_to_piece.at(c), m_to_move);
                    
                    if (move.promoted_to == Piece::WHITE_PAWN || move.promoted_to == Piece::BLACK_PAWN) {
                        std::cerr << "Cannot promote to pawn\n";
                        return INVALID_MOVE;
                    }
                    if (move.promoted_to == Piece::WHITE_KING || move.promoted_to == Piece::BLACK_KING) {
                        std::cerr << "Cannot promote to king\n";
                        return INVALID_MOVE;
                    }

                    break;
                }
                [[fallthrough]];
            case 6:
                // check/checkmate
                if (c == '+') move.is_check = true;
                else if (c == '#') move.is_checkmate = true;
            }
        }

        move.piece = convert_piece_to_color(move.piece, m_to_move);

        if (squares_buffer.size() == 2) {
            move.to_square = Position(squares_buffer);

        } else if (squares_buffer.size() == 3) {
            // rank/file disambiguation
            std::string source_string = squares_buffer.substr(0, 1);
            
            if (source_string[0] >= '1' && source_string[0] <= '8') {
                source_string = '?' + source_string;
            } else {
                source_string += '?';
            }

            move.from_square = Position(source_string);

            std::string target_string = squares_buffer.substr(1, 2);

            move.to_square = Position(target_string);
        
        } else if (squares_buffer.size() == 4) {
            std::string source_string = squares_buffer.substr(0, 2);
            std::string target_string = squares_buffer.substr(2, 2);

            move.from_square = Position(source_string);
            if (!move.from_square.is_in_bounds()) {        
                std::cerr << "Invalid source/target square\n";
                return INVALID_MOVE;
            }
            move.to_square = Position(target_string);
        
        } else {
            std::cerr << "Invalid source/target square string \"" << squares_buffer << "\" while executing move\n";
            return INVALID_MOVE;
        }

        if (!move.to_square.is_in_bounds()) {
            std::cerr << "Invalid source/target square\n";
            return INVALID_MOVE;
        } 

        if (move.from_square.m_rank == -1 || move.from_square.m_file == -1) {
            // find square on board
            std::vector<Position> candidates;
            for (int rank = 0; rank < 8; rank++) {
                for (int file = 0; file < 8; file++) {
                    if (rank != move.from_square.m_rank && move.from_square.m_rank != -1) continue;
                    if (file != move.from_square.m_file && move.from_square.m_file != -1) continue;

                    if (m_pieces[rank][file] == move.piece) {
                        Position candidate = Position(rank, file);
                        if (move.is_capture &&
                            piece_can_capture(move.piece, candidate, move.to_square) &&
                            !is_route_blocked(move.piece, candidate, move.to_square))
                            candidates.push_back(Position(rank, file));
                        else if (!move.is_capture &&
                            piece_can_move(move.piece, candidate, move.to_square) &&
                            !is_route_blocked(move.piece, candidate, move.to_square))
                            candidates.push_back(Position(rank, file));
                    }
                }
            }

            if (candidates.size() == 0) {
                std::cerr << "Invalid move " << move_string << ", no candidate squares found\n";
                return INVALID_MOVE;
            } else if (candidates.size() >= 2) {
                std::cerr << "Ambiguous move " << move_string << ", too many candidate squares found: ";
                bool first = true;
                for (Position c : candidates) {
                    if (!first) 
                        std::cerr << ", ";
                    first = false;
                    std::cerr << c.to_notation();
                }
                std::cerr << '\n';
                return INVALID_MOVE;
            } else {
                move.from_square = candidates[0];
            }
        }

        return move;
    }

    bool is_castles_legal(Move move) {
        if (move.castles == Castles::UNDEFINED) {
            //std::cerr << "Undefined castles in execute_castles\n";
            return false;
        }
        
        if (!m_allowed_castles[(int) move.castles]) {
            //std::cerr << "Illegal castling move: " << all_castles[(int) move.castles].second << '\n';
            return false;
        }

        if (m_to_move == Player::WHITE) {
            if (is_in_check(Position("e1"), Player::WHITE)) {
                //std::cerr << "Cannot castle, king is in check\n";
                return false;
            }
        } else if (m_to_move == Player::BLACK) {
            if (is_in_check(Position("e8"), Player::BLACK)) {
                //std::cerr << "Cannot castle, king is in check\n";
                return false;
            }
        }

        switch (move.castles) {
            case Castles::WHITE_QUEEN:
                if (
                    m_pieces[7][1] != Piece::EMPTY || m_pieces[7][2] != Piece::EMPTY ||
                    m_pieces[7][3] != Piece::EMPTY
                ) {
                    //std::cerr << "Cannot castle: squares between are occupied\n";
                    return false;
                }
                if (is_in_check(Position("d1"), Player::WHITE) || is_in_check(Position("c1"), Player::WHITE)) {
                    //std::cerr << "Cannot castle: castle target square or square in between is in check\n";
                    return false;
                }
                if (piece_at(Position("a1")) != Piece::WHITE_ROOK || piece_at(Position("e1")) != Piece::WHITE_KING) {
                    // no rook or king in correct place
                    return false;
                }
                return true;

            case Castles::BLACK_QUEEN:
                if (
                    m_pieces[0][1] != Piece::EMPTY || m_pieces[0][2] != Piece::EMPTY ||
                    m_pieces[0][3] != Piece::EMPTY
                ) {
                    //std::cerr << "Cannot castle: squares between are occupied\n";
                    return false;
                }
                if (is_in_check(Position("d8"), Player::BLACK) || is_in_check(Position("c8"), Player::BLACK)) {
                    //std::cerr << "Cannot castle: castle target square or square in between is in check\n";
                    return false;
                }
                if (piece_at(Position("a8")) != Piece::BLACK_ROOK || piece_at(Position("e8")) != Piece::BLACK_KING) {
                    // no rook or king in correct place
                    return false;
                }
                return true;
            
            case Castles::WHITE_KING:
                if (
                    m_pieces[7][5] != Piece::EMPTY || m_pieces[7][6] != Piece::EMPTY
                ) {
                    //std::cerr << "Cannot castle: squares between are occupied\n";
                    return false;
                }
                if (is_in_check(Position("f1"), Player::WHITE) || is_in_check(Position("g1"), Player::WHITE)) {
                    //std::cerr << "Cannot castle: castle target square or square in between is in check\n";
                    return false;
                }
                if (piece_at(Position("h1")) != Piece::WHITE_ROOK || piece_at(Position("e1")) != Piece::WHITE_KING) {
                    // no rook or king in correct place
                    return false;
                }
                return true;

            case Castles::BLACK_KING:
                if (
                    m_pieces[0][5] != Piece::EMPTY || m_pieces[0][6] != Piece::EMPTY
                ) {
                    //std::cerr << "Cannot castle: squares between are occupied\n";
                    return false;
                }
                if (is_in_check(Position("f8"), Player::BLACK) || is_in_check(Position("g8"), Player::BLACK)) {
                    //std::cerr << "Cannot castle: castle target square or square in between is in check\n";
                    return false;
                }
                if (piece_at(Position("h8")) != Piece::BLACK_ROOK || piece_at(Position("e8")) != Piece::BLACK_KING) {
                    // no rook or king in correct place
                    return false;
                }
                return true;
        }
        return false;
    }

    bool is_move_legal(Move move) {
        if (move.castles != Castles::UNDEFINED)
            return is_castles_legal(move);

        if (get_piece_color(piece_at(move.to_square)) == m_to_move) {
            //std::cerr << "Cannot move to square with same color piece\n";
            return false;
        }

        if (piece_at(move.to_square) != Piece::EMPTY && !move.is_capture) {
            //std::cerr << "Cannot move to occupied square without capture\n";
            return false;
        }

        Position king_square;
        if (m_to_move == Player::WHITE) {
            king_square = find_piece(Piece::WHITE_KING);
        } else {
            king_square = find_piece(Piece::BLACK_KING);
        }
        Piece from_piece_before = piece_at(move.from_square);
        Piece to_piece_before = piece_at(move.to_square);
        
        if (move.promoted_to != Piece::EMPTY) {
            piece_at(move.to_square) = move.promoted_to;
        } else {
            piece_at(move.to_square) = move.piece;
        }
        piece_at(move.from_square) = Piece::EMPTY;

        if (move.piece == Piece::WHITE_KING || move.piece == Piece::BLACK_KING) {
            if (is_in_check(move.to_square, m_to_move)) {
                //std::cerr << "Cannot move to square that is in check\n";
                piece_at(move.from_square) = from_piece_before;
                piece_at(move.to_square) = to_piece_before;
                return false;
            }
        } else if (is_in_check(king_square, m_to_move)) {
            //std::cerr << "Move leaves king in check\n";
            piece_at(move.from_square) = from_piece_before;
            piece_at(move.to_square) = to_piece_before;
            return false;
        }

        // en passant
        if (move.is_capture && m_en_passant_target_square == move.to_square && (move.piece == Piece::WHITE_PAWN || move.piece == Piece::BLACK_PAWN)) {
            piece_at(move.from_square) = from_piece_before;
            piece_at(move.to_square) = to_piece_before;
            const Piece enemy_pawn = (move.piece == Piece::WHITE_PAWN ? Piece::BLACK_PAWN : Piece::WHITE_PAWN);
            if (move.piece == Piece::WHITE_PAWN) {
                return m_pieces[move.to_square.m_rank + 1][move.to_square.m_file] == enemy_pawn;
            } else {
                return m_pieces[move.to_square.m_rank - 1][move.to_square.m_file] == enemy_pawn;
            }
        } else if (to_piece_before != Piece::EMPTY && !move.is_capture) {
            //std::cerr << "Cannot move to occupied square without capturing\n";
            piece_at(move.from_square) = from_piece_before;
            piece_at(move.to_square) = to_piece_before;
            return false;
        } else if (to_piece_before == Piece::EMPTY && move.is_capture) {
            //std::cerr << "Cannot capture empty square\n";
            piece_at(move.from_square) = from_piece_before;
            piece_at(move.to_square) = to_piece_before;
            return false;
        }
        
        piece_at(move.from_square) = from_piece_before;
        piece_at(move.to_square) = to_piece_before;
        return true;
    }

    void execute_move_string(std::string move_string) {
        Move move = parse_move(move_string);
        
        execute_move(move);
    }

    void execute_move(Move move) {
        if (move == INVALID_MOVE || !is_move_legal(move)) {
            std::cerr << "Invalid move\n";
            return;
        }

        if (move.castles != Castles::UNDEFINED) {
            execute_castles(move.castles);
            return;
        }

        if (move.promoted_to != Piece::EMPTY) {
            piece_at(move.to_square) = move.promoted_to;
        } else {
            piece_at(move.to_square) = move.piece;
        }
        piece_at(move.from_square) = Piece::EMPTY;

        m_halfmove_clock++;
        if (
            move.piece == Piece::WHITE_PAWN || move.piece == Piece::WHITE_PAWN || move.is_capture
        ) {
            // reset 50 move rule clock
            m_halfmove_clock = 0;
        }

        if (m_to_move == Player::BLACK) {
            m_fullmove_clock++;
            m_to_move = Player::WHITE;
        } else {
            m_to_move = Player::BLACK;
        }

        if ((move.piece == Piece::WHITE_PAWN || move.piece == Piece::BLACK_PAWN) && abs(move.from_square.m_rank - move.to_square.m_rank) == 2) {
            m_en_passant_target_square = Position::average(move.from_square, move.to_square);
        } else {
            m_en_passant_target_square = INVALID_POS;
        }

        after_move_checks();
    }

    std::vector<Move> get_legal_moves(Player side = Player::UNDEFINED, bool find_check_checkmate = true, int limit = -1) {
        if (side == Player::UNDEFINED) side = m_to_move;
        
        std::vector<Position> positions;
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                if (get_piece_color(m_pieces[y][x]) == side) {
                    positions.push_back(Position(y, x));
                }
            }
        }
        
        const Piece pawn = convert_piece_to_color(Piece::WHITE_PAWN, side);
        const Piece knight = convert_piece_to_color(Piece::WHITE_KNIGHT, side);
        const Piece bishop = convert_piece_to_color(Piece::WHITE_BISHOP, side);
        const Piece rook = convert_piece_to_color(Piece::WHITE_ROOK, side);
        const Piece queen = convert_piece_to_color(Piece::WHITE_QUEEN, side);
        const Piece king = convert_piece_to_color(Piece::WHITE_KING, side);

        const int home_rank = (side == Player::WHITE ? Position::WHITE_HOME_RANK : Position::BLACK_HOME_RANK);
        const int pawn_direction = (side == Player::WHITE ? -1 : +1);

        const Player opposite_color = (side == Player::WHITE ? Player::BLACK : Player::WHITE);

        Position enemy_king_pos = find_piece(side == Player::WHITE ? Piece::BLACK_KING : Piece::WHITE_KING);

        std::vector<Move> moves;

        auto add_move = [this, enemy_king_pos, opposite_color, find_check_checkmate, &moves, limit](Move rvalue_move) {
            Move &move(rvalue_move);

            if (is_move_legal(move)) {
                if (find_check_checkmate) {
                    if (is_in_check_after(move, enemy_king_pos, opposite_color)) move.is_check = true;
                    if (is_checkmate_after(move, opposite_color)) move.is_checkmate = true;
                }
                moves.push_back(move);
            }
        };

        for (Position position : positions) {
            Piece piece = piece_at(position);
            if (limit > 0 && static_cast<int> (moves.size()) >= limit) return moves;
            
            if (piece == pawn) {
                Position capture_left = position + std::pair<int, int> {pawn_direction, -1};
                Position capture_right = position + std::pair<int, int> {pawn_direction, 1};

                for (Position capture : {capture_left, capture_right}) {
                    if (capture.is_in_bounds() && (
                        get_piece_color(piece_at(capture)) == opposite_color ||
                        m_en_passant_target_square == capture
                    )) {

                        if (capture.m_rank == 0 || capture.m_rank == 7) {
                            // promotion
                            add_move(Move(pawn, position, capture, true, knight));
                            add_move(Move(pawn, position, capture, true, bishop));
                            add_move(Move(pawn, position, capture, true, rook));
                            add_move(Move(pawn, position, capture, true, queen));
                        } else {
                            add_move(Move(pawn, position, capture, true));
                        }
                    }
                }
                
                if (piece_at(position + std::pair<int, int> {pawn_direction, 0}) != Piece::EMPTY) continue;
                
                Position target_pos = position + std::pair<int, int> {pawn_direction, 0};
                if (target_pos.m_rank == 0 || target_pos.m_rank == 7) {
                    // promotion
                    add_move(Move(pawn, position, target_pos, false, knight));
                    add_move(Move(pawn, position, target_pos, false, bishop));
                    add_move(Move(pawn, position, target_pos, false, rook));
                    add_move(Move(pawn, position, target_pos, false, queen));
                } else {
                    add_move(Move(pawn, position, target_pos, false));
                }
                
                if (position.m_rank == home_rank) {
                    if (piece_at(position + std::pair<int, int> {2*pawn_direction, 0}) != Piece::EMPTY) continue;
                    add_move(Move(pawn, position, position + std::pair<int, int> {2*pawn_direction, 0}, false));
                }
                
            } else if (piece == knight) {
                for (std::pair<int, int> delta : horse_deltas) {
                    Position target_pos = position + delta;
                    if (target_pos.is_in_bounds() && get_piece_color(piece_at(target_pos)) != side) {
                        add_move(Move(knight, position, target_pos, get_piece_color(piece_at(target_pos)) == opposite_color));
                    }
                }
            } else if (piece == bishop) {
                for (std::pair<int, int> direction : bishop_directions) {
                    Position target_pos = position + direction;
                    while (target_pos.is_in_bounds()) {
                        Piece target_piece = piece_at(target_pos);

                        if (target_piece == Piece::EMPTY) {
                            add_move(Move(piece, position, target_pos, false));
                        } else if (get_piece_color(target_piece) == side) {
                            break; // blocks vision
                        } else {
                            add_move(Move(piece, position, target_pos, true));
                            break;
                        }

                        target_pos += direction;
                    }
                }
            } else if (piece == rook) {
                for (std::pair<int, int> direction : rook_directions) {
                    Position target_pos = position + direction;
                    while (target_pos.is_in_bounds()) {
                        Piece target_piece = piece_at(target_pos);

                        if (target_piece == Piece::EMPTY) {
                            add_move(Move(piece, position, target_pos, false));
                        } else if (get_piece_color(target_piece) == side) {
                            break; // blocks vision
                        } else {
                            add_move(Move(piece, position, target_pos, true));
                            break;
                        }

                        target_pos += direction;
                    }
                }
            } else if (piece == queen) {
                for (std::pair<int, int> direction : all_directions) {
                    Position target_pos = position + direction;
                    while (target_pos.is_in_bounds()) {
                        Piece target_piece = piece_at(target_pos);

                        if (target_piece == Piece::EMPTY) {
                            add_move(Move(piece, position, target_pos, false));
                        } else if (get_piece_color(target_piece) == side) {
                            break; // blocks vision
                        } else {
                            add_move(Move(piece, position, target_pos, true));
                            break;
                        }

                        target_pos += direction;
                    }
                }
            } else if (piece == king) {
                for (std::pair<int, int> direction : all_directions) {
                    Position target_pos = position + direction;
                    if (target_pos.is_in_bounds() && get_piece_color(piece_at(target_pos)) != side) {
                        add_move(Move(piece, position, target_pos, get_piece_color(piece_at(target_pos)) == opposite_color));
                    }
                }
            }
        }

        if (side == Player::WHITE) {
            if (m_allowed_castles[(int) Castles::WHITE_KING]) add_move(Move(Castles::WHITE_KING));
            if (m_allowed_castles[(int) Castles::WHITE_QUEEN]) add_move(Move(Castles::WHITE_QUEEN));
        } else {
            if (m_allowed_castles[(int) Castles::BLACK_KING]) add_move(Move(Castles::BLACK_KING));
            if (m_allowed_castles[(int) Castles::BLACK_QUEEN]) add_move(Move(Castles::BLACK_QUEEN));
        }
        
        return moves;
    }

    bool is_stalemate(Player to_move = Player::UNDEFINED) {
        if (to_move == Player::UNDEFINED) to_move = m_to_move;
        
        return get_legal_moves(to_move, false, 1).empty();
    }

    void after_move_checks() {
        if (piece_at(Position("e1")) != Piece::WHITE_KING) {
            m_allowed_castles[(int) Castles::WHITE_KING] = false;
            m_allowed_castles[(int) Castles::WHITE_QUEEN] = false;
        }
        if (piece_at(Position("e8")) != Piece::BLACK_KING) {
            m_allowed_castles[(int) Castles::BLACK_KING] = false;
            m_allowed_castles[(int) Castles::BLACK_QUEEN] = false;
        }

        if (piece_at(Position("a1")) != Piece::WHITE_ROOK) {
            m_allowed_castles[(int) Castles::WHITE_QUEEN] = false;
        }
        if (piece_at(Position("h1")) != Piece::WHITE_ROOK) {
            m_allowed_castles[(int) Castles::WHITE_KING] = false;
        }
        if (piece_at(Position("a8")) != Piece::BLACK_ROOK) {
            m_allowed_castles[(int) Castles::BLACK_QUEEN] = false;
        }
        if (piece_at(Position("h8")) != Piece::BLACK_ROOK) {
            m_allowed_castles[(int) Castles::BLACK_KING] = false;
        }
        
        bool white_insufficient_material = false;
        bool black_insufficient_material = false;
        for (Player side : {Player::WHITE, Player::BLACK}) {
            if (is_checkmate(side)) {
                //std::cout << "Checkmate for " << (side == Player::WHITE ? "black" : "white") << "\n";
                m_state = (side == Player::WHITE ? GameState::BLACK_WIN : GameState::WHITE_WIN);
                return;
            }

            int horse_count = 0;
            int light_bishop_count = 0;
            int dark_bishop_count = 0;
            bool has_other_pieces = false;
            for (int y = 0; y < 8; y++) {
                for (int x = 0; x < 8; x++) {
                    if (m_pieces[y][x] == convert_piece_to_color(Piece::WHITE_KNIGHT, side)) horse_count++;
                    else if (m_pieces[y][x] == convert_piece_to_color(Piece::WHITE_BISHOP, side)) {
                        if (y % 2 == x % 2) light_bishop_count++;
                        else dark_bishop_count++;
                    }
                    else if (get_piece_color(m_pieces[y][x]) == side && m_pieces[y][x] != convert_piece_to_color(Piece::WHITE_KING, side)) {
                        has_other_pieces = true;
                        break;
                    }
                }
                if (has_other_pieces) break;
            }
            if (!has_other_pieces) {
                bool has_horse_mate = horse_count >= 3;
                bool has_horse_and_bishop_mate = horse_count >= 1 && (dark_bishop_count + light_bishop_count) >= 1;
                bool has_bishop_mate = dark_bishop_count >= 1 && light_bishop_count >= 1;

                bool has_mate = has_horse_mate || has_horse_and_bishop_mate || has_bishop_mate;
                if (!has_mate) {
                    if (side == Player::WHITE) white_insufficient_material = true;
                    else black_insufficient_material = true;
                }
            }
        }

        if (m_halfmove_clock >= 100) {
            //std::cout << "50 move rule: game ended in a draw\n";
            m_state = GameState::DRAW;
            return;
        }

        if (white_insufficient_material && black_insufficient_material) {
            //std::cout << "Draw by insufficient material\n";
            m_state = GameState::DRAW;
            return;
        }

        if (is_stalemate()) {
            //std::cout << "Draw by stalemate\n";
            m_state = GameState::DRAW;
            return;
        }
    }

    Position find_piece(Piece piece) {
        for (int rank = 0; rank < 8; rank++) {
            for (int file = 0; file < 8; file++) {
                if (m_pieces[rank][file] == piece) return Position(rank, file);
            }
        }
        return INVALID_POS;
    }

    // side is the side that would be in check
    bool is_in_check(Position pos, Player side, Position xray_pos = INVALID_POS) {
        const Piece opposite_horse = (side == Player::WHITE ? Piece::BLACK_KNIGHT : Piece::WHITE_KNIGHT);
        for (std::pair<int, int> delta : horse_deltas) {
            Position new_pos = pos + delta;
            if (new_pos.is_in_bounds() && piece_at(new_pos) == opposite_horse) {
                return true;
            }
        }
        
        for (std::pair<int, int> direction : all_directions) {
            Position cur_pos = pos + direction;
            while (cur_pos.is_in_bounds()) {
                if (cur_pos == xray_pos) {
                    cur_pos += direction;
                    continue;
                }
                
                Piece piece = piece_at(cur_pos);
                if (piece == Piece::EMPTY) {
                    cur_pos += direction;
                    continue;
                }
                if (get_piece_color(piece) == side) break; // blocks vision
                if (piece_can_capture(piece, cur_pos, pos)) {
                    return true;
                } else break; // blocks vision for pieces behind this one
            }
        }
        
        return false;
    }

    bool is_in_check_after(Move move, Position pos, Player side) {
        Board new_board = new_board_from_move(move);
        
        return new_board.is_in_check(pos, side);
    }

    Evaluation get_evaluation() {
        Position king_pos = find_piece(m_to_move == Player::WHITE ? Piece::BLACK_KING : Piece::WHITE_KING);

        Evaluation eval = Evaluation(
            m_to_move == Player::WHITE ? Player::BLACK : Player::WHITE,
            m_state,
            evaluate_material(),
            is_in_check(king_pos, m_to_move)
        );
        if (!eval.is_valid()) {
            std::cerr << "Invalid evaluation created from board\n";
            print_state();
        }
        return eval;
    }

    double evaluate_material() {
        static const std::map<Piece, double> piece_values = {
            {Piece::WHITE_PAWN, 1.0},
            {Piece::BLACK_PAWN, 1.0},
            {Piece::WHITE_KNIGHT, 3.0},
            {Piece::BLACK_KNIGHT, 3.0},
            {Piece::WHITE_BISHOP, 3.25},
            {Piece::BLACK_BISHOP, 3.25},
            {Piece::WHITE_ROOK, 5.0},
            {Piece::BLACK_ROOK, 5.0},
            {Piece::WHITE_QUEEN, 9.0},
            {Piece::BLACK_QUEEN, 9.0},
        };
        
        int white_sum = 0;
        int black_sum = 0;
        
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                Piece piece = m_pieces[y][x];
                if (piece == Piece::EMPTY) continue;

                if (piece == Piece::WHITE_KING || piece == Piece::BLACK_KING) continue;

                if (get_piece_color(piece) == Player::WHITE) {
                    white_sum += piece_values.at(piece);
                } else {
                    black_sum += piece_values.at(piece);
                }
            }
        }

        return white_sum - black_sum;
    }

    bool is_checkmate(Player side) {
        Position king_pos = find_piece(side == Player::WHITE ? Piece::WHITE_KING : Piece::BLACK_KING);
        
        return is_stalemate(side) && is_in_check(king_pos, side);
    }

    bool is_checkmate_after(Move move, Player side) {
        Board new_board = new_board_from_move(move);
        return new_board.is_checkmate(side);
    }

    Board new_board_from_move(Move move) {
        Board new_board(*this);

        new_board.execute_move(move);

        return new_board;
    }
};
