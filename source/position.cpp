#pragma once

#include "includes.h"
#include "position.h"

Position Position::average(Position first, Position second) {
    Position retval;
    
    retval.m_file = (first.m_file + second.m_file) / 2;
    retval.m_rank = (first.m_rank + second.m_rank) / 2;
    
    return retval;
}

Position::Position() = default;

Position::Position(std::string str) {
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

Position::Position(int8_t rank, int8_t file) {
    m_rank = rank;
    m_file = file;
}

bool Position::operator==(Position other) {
    return m_file == other.m_file && m_rank == other.m_rank;
}

bool Position::operator!=(Position other) {
    return !(*this == other);
}

Position Position::operator+(std::pair<int, int> p) {
    Position new_pos = *this;
    new_pos.m_rank += p.first;
    new_pos.m_file += p.second;
    return new_pos;
}

Position Position::operator+=(std::pair<int, int> p) {
    *this = *this + p;
    return *this;
}

std::string Position::to_notation() {
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

bool Position::is_in_bounds() {
    return m_file >= 0 && m_file < 8 && m_rank >= 0 && m_rank < 8;
}

std::vector<Position> Position::interpolate(Position target_pos) {
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
