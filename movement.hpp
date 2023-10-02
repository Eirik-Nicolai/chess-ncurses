#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include <iostream>
#include <entt/entt.hpp>
#include <ncurses.h>

#include "utils.hpp"
#include "components.hpp"

std::vector<pos> pawn_movement(int8_t delta, bool first_move, pos p);
std::vector<pos> knight_movement(pos p);
std::vector<pos> king_movement(pos p);
std::vector<std::vector<pos>> rook_movement(pos p);
std::vector<std::vector<pos>> bishop_movement(pos p);
std::vector<std::vector<pos>> queen_movement(pos p);

void find_possible_moves(entt::registry&, DebugLogger*);


#endif // MOVEMENT_H_
