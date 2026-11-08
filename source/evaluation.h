#pragma once

#include "types.h"

class Evaluation {
private:
    double material = 0.0;
    bool is_check = false;
    Player player = Player::UNDEFINED;
    GameState state = GameState::IN_PROGRESS;

public:
    Evaluation();
    
    Evaluation(Player p_player, GameState p_state, double p_material, bool p_is_check);

    void inherit_from(Evaluation &other);

    bool is_valid();

    void print_eval();

    // smaller in order means better position for current player
    bool operator<(const Evaluation &other) const;
    
    bool operator==(const Evaluation &other) const;

    inline bool is_win() const;

    inline bool is_draw() const;

    inline bool is_loss() const;

    inline double is_up_material() const;
};