#include <iostream>
#include <entt/entt.hpp>
#include <ncurses.h>

#include <string>

#include "utils.hpp"
#include "systems.hpp"
#include "movement.hpp"

auto main(int argc, char *argv[]) -> int
{
    entt::registry reg;

    init(reg);

    bool player_turn = true; //true = white, false = black

    // init output
    initscr();

    noecho();
    cbreak();

    start_color();

    init_color(23, 305,305,305);

    init_pair(CLR_HIGHLIGHT_SQR, COLOR_WHITE, COLOR_CYAN);
    init_pair(CLR_WHITE_SQUARE, 248, COLOR_BLACK);
    init_pair(CLR_CRASH, COLOR_WHITE, COLOR_RED);
    init_pair(CLR_ACT_MOVES, COLOR_WHITE, COLOR_GREEN);
    init_pair(CLR_CAPTURE, COLOR_WHITE, COLOR_MAGENTA);

    curs_set(0);

    auto win = newwin(9*SQUARE_SIZE, 9*SQUARE_SIZE_H, WINDOW_OFFS_V, WINDOW_OFFS_H);
    auto side_win = newwin(7*SQUARE_SIZE, 2.5*SQUARE_SIZE_H, WINDOW_OFFS_V, WINDOW_OFFS_H-(3*SQUARE_SIZE_H));
    nodelay(win, true);

    // listen to left and right click
    mousemask(BUTTON1_PRESSED | BUTTON3_PRESSED, NULL);
    keypad(win, TRUE);
    mouseinterval(0);

    DebugLogger debugLogger;
    auto *db = &debugLogger;

    // DEBUGGING
    bool show_all_moves = false;

    bool run = true;
    while(run)
    {
        auto ch = wgetch(win);
        if (ch == ERR)
        {
            // rien
        }
        else
        {
            switch (ch) {
                case KEY_BACKSPACE:
                    run = false;
                break;
                case KEY_MOUSE:
                    MEVENT event;
                    if(getmouse(&event) == OK)
                    {
                        clear();
                        if(event.bstate == BUTTON3_PRESSED) // debugging
                        {
                            //change_turn(reg, player_turn);
                        }
                        else
                        {
                            auto [posx, posy] = get_rect_by_pos(event.x, event.y);

                            // player clicked on our board
                            db->Debug("Mouse: ", std::make_pair(posx, posy));
                            if (posx!=-1 && posy!=-1)
                            {
                                auto view = reg.view<const pos>();
                                entt::entity ent;
                                for(auto [select_ent, pos_selecting]: view.each())
                                {
                                    if(posy == pos_selecting.x && posx == pos_selecting.y)
                                    {
                                        db->Debug("pos", pos_selecting);
                                        db->Debug("     ", std::make_pair(posx, posy));

                                        // is the player selecting somewhere to move
                                        if (reg.all_of<_highlighted>(select_ent))
                                        {  //yes
                                            db->Debug(",oving ", pos_selecting);
                                            auto move_view = reg.view<const piece, pos, const _selected>();
                                            for (auto [ent, piece, p]: move_view.each())
                                            {
                                                p.x = pos_selecting.x;
                                                p.y = pos_selecting.y;

                                                if(reg.all_of<_pawn>(ent))
                                                {
                                                    reg.replace<_pawn>(ent, true);
                                                }
                                            }

                                            change_turn(reg, player_turn);
                                            clear_highlighting(reg, db);

                                            break;

                                        }
                                        // is the player selecting a piece for capture
                                        else if (reg.all_of<_capturable>(select_ent))
                                        { //yes
                                            db->Debug("Capturing ", pos_selecting);
                                            auto capture_view = reg.view<const piece, pos>();
                                            for (auto [ent, entpiece, p]: capture_view.each())
                                            {
                                                if(p.x == pos_selecting.x && p.y == pos_selecting.y)
                                                {
                                                    reg.erase<pos>(ent);
                                                    break;
                                                }
                                            }

                                            auto move_view = reg.view<const _selected, pos>();
                                            for (auto [ent, p]: move_view.each())
                                            {
                                                p.x = pos_selecting.x;
                                                p.y = pos_selecting.y;
                                            }

                                            change_turn(reg, player_turn);
                                            clear_highlighting(reg, db);

                                            break;
                                        }
                                        else if (reg.all_of<piece, _has_turn>(select_ent))
                                        {
                                            db->Debug("selecting ", pos_selecting);
                                            clear_highlighting(reg, db);

                                            reg.emplace_or_replace<_selected>(select_ent);
                                            find_possible_moves(reg, db);
                                            break;
                                        }
                                    }
                                } // view for
                            }
                            if (posx <= -1 || posx > 7 || posy <= -1 || posy > 7)
                            {
                                clear_highlighting(reg, db);
                            }
                        }
                    }
                break;
            }
        }

        int ppos = 0;

        auto view = reg.view<const pos, const _pawn>(entt::exclude_t<_has_turn>());
        for (auto [ent, piece, p]: view.each()) ppos++;

        db->Debug("Pawns ", ppos);


        db->DebugPrint();

        draw(reg, win, side_win, db);
    }


    endwin();
}
