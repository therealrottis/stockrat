#pragma once

#include "includes.h"

class Position {
public:
    const static int8_t WHITE_HOME_RANK = 6;
    const static int8_t BLACK_HOME_RANK = 1; 
    
    int8_t m_rank = -1;
    int8_t m_file = -1;

    static Position average(Position first, Position second);

    Position();

    Position(std::string str);

    Position(int8_t rank, int8_t file);

    bool operator==(Position other);

    bool operator!=(Position other);

    Position operator+(std::pair<int, int> p);

    Position operator+=(std::pair<int, int> p);

    std::string to_notation();

    bool is_in_bounds();

    std::vector<Position> interpolate(Position target_pos);
};

const Position INVALID_POS;
