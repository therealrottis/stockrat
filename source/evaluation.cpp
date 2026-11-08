#pragma once

#include "includes.h"
#include "types.h"
#include "evaluation.h"

Evaluation::Evaluation() = default;
    
Evaluation::Evaluation(Player p_player, GameState p_state, double p_material, bool p_is_check) {
    player = p_player;
    state = p_state;
    material = p_material;
    is_check = p_is_check;
}

void Evaluation::inherit_from(Evaluation &other) {
    state = other.state;
    material = other.material;
}

bool Evaluation::is_valid() {
    return player != Player::UNDEFINED;
}

void Evaluation::print_eval() {
    std::cout << "Player: " << (player == Player::WHITE ? "white" : "black") << ", material: " << material;
    std::cout << ", state: " << state_to_string.at(state) << '\n';
}

// smaller in order means better position for current player
bool Evaluation::operator<(const Evaluation &other) const {
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
    
bool Evaluation::operator==(const Evaluation &other) const {
    return (
        player == other.player &&
        state == other.state &&
        material == other.material
    );
}

inline bool Evaluation::is_win() const {
    return (
        (player == Player::WHITE && state == GameState::WHITE_WIN) ||
        (player == Player::BLACK && state == GameState::BLACK_WIN)
    );
}

inline bool Evaluation::is_draw() const {
    return state == GameState::DRAW;
}

inline bool Evaluation::is_loss() const {
    return (
        (player == Player::BLACK && state == GameState::WHITE_WIN) ||
        (player == Player::WHITE && state == GameState::BLACK_WIN)
    );
}

inline double Evaluation::is_up_material() const {
    return (
        (player == Player::WHITE && material > 0.0) ||
        (player == Player::BLACK && material < 0.0)   
    );
}
