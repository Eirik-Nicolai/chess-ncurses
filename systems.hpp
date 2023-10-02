#ifndef SYSTEMS_H_
#define SYSTEMS_H_

#include <string>
#include <iostream>
#include <ncurses.h>
#include <entt/entt.hpp>

#include "utils.hpp"
#include "components.hpp"

void init(entt::registry&);

void draw_piece(WINDOW*, const render&, const pos&);

void draw_board(entt::registry&, WINDOW*);
void draw_pieces(entt::registry&, WINDOW*);
void draw_possible_moves(entt::registry&, WINDOW*);
void draw_sideview(entt::registry&, WINDOW*, DebugLogger*);
void draw(entt::registry&, WINDOW*,WINDOW*, DebugLogger*);

void change_turn(entt::registry&, bool&);

void on_click(entt::registry&);



#endif // SYSTEMS_H_
