#include "systems.hpp"

void init(entt::registry& reg)
{


    // init board
    for (auto i = 0; i < 8; ++i)
    {
        for (auto j = 0; j < 8; ++j)
        {
            const auto board_square = reg.create();
            reg.emplace<pos>(board_square, i, j);
            reg.emplace<square>(board_square, (i+j%2)%2==0);
            reg.emplace<renderable>(board_square);
        }
    }

    // init pieces
    // pawns

    for (auto i = 0; i < 8; ++i)
    {
        const auto pawn_w = reg.create();
        reg.emplace<_pawn>(pawn_w, false);
        reg.emplace<pos>(pawn_w, 1, i);
        reg.emplace<piece>(pawn_w, true);
        reg.emplace<render>(pawn_w,
                            " o ",
                            "|#|",
                            ""
        );
        reg.emplace<_movement>(pawn_w);
        reg.emplace<_has_turn>(pawn_w);
        const auto pawn_b = reg.create();
        reg.emplace<_pawn>(pawn_b, false);
        reg.emplace<pos>(pawn_b, 6, i);
        reg.emplace<piece>(pawn_b, false);
        reg.emplace<render>(pawn_b,
                            " o ",
                            "|_|",
                            ""
        );
        reg.emplace<_movement>(pawn_b);
    }

    for(int i = 0; i < 2; ++i)
    {
        const auto rook_w = reg.create();
        reg.emplace<pos>(rook_w, 0, 7*i);
        reg.emplace<piece>(rook_w, true);
        reg.emplace<render>(rook_w,
                            " + ",
                            "|#|",
                            "|#|"
        );
        reg.emplace<_movement>(rook_w);
        reg.emplace<_rook>(rook_w);
        reg.emplace<_has_turn>(rook_w);
        const auto rook_b = reg.create();
        reg.emplace<pos>(rook_b, 7, 7*i);
        reg.emplace<piece>(rook_b, false);
        reg.emplace<render>(rook_b,
                            " + ",
                            "| |",
                            "|_|"
        );
        reg.emplace<_movement>(rook_b);
        reg.emplace<_rook>(rook_b);
    }

    for(int i = 0; i < 2; ++i)
    {
        const auto knight_w = reg.create();
        reg.emplace<pos>(knight_w, 0, 1+(5*i));
        reg.emplace<piece>(knight_w, true);
        reg.emplace<render>(knight_w,
                            " _\\",
                            "\\#\\",
                            "/#/"
        );
        reg.emplace<_movement>(knight_w);
        reg.emplace<_knight>(knight_w);
        reg.emplace<_has_turn>(knight_w);
        const auto knight_b = reg.create();
        reg.emplace<pos>(knight_b, 7,  1+(5*i));
        reg.emplace<piece>(knight_b, false);
        reg.emplace<render>(knight_b,
                            " _\\",
                            "\\ \\",
                            "/_/"
        );
        reg.emplace<_movement>(knight_b);
        reg.emplace<_knight>(knight_b);
    }

    for(int i = 0; i < 2; ++i)
    {
        const auto bishop_w = reg.create();
        reg.emplace<pos>(bishop_w, 0,  2+(3*i));
        reg.emplace<piece>(bishop_w, true);
        reg.emplace<render>(bishop_w,
                            " o ",
                            "/#\\",
                            "|#|"
        );
        reg.emplace<_movement>(bishop_w);
        reg.emplace<_bishop>(bishop_w);
        reg.emplace<_has_turn>(bishop_w);
        const auto bishop_b = reg.create();
        reg.emplace<pos>(bishop_b, 7, 2+(3*i));
        reg.emplace<piece>(bishop_b, false);
        reg.emplace<render>(bishop_b,
                            " o ",
                            "/ \\",
                            "|_|"
        );
        reg.emplace<_movement>(bishop_b);
        reg.emplace<_bishop>(bishop_b);
    }

    const auto queen_w = reg.create();
    reg.emplace<pos>(queen_w, 4, 4);
    reg.emplace<piece>(queen_w, true);
    reg.emplace<render>(queen_w,
                        "/O\\",
                        "L#J",
                        "\\#/"
    );
    reg.emplace<_movement>(queen_w);
    reg.emplace<_queen>(queen_w);
    reg.emplace<_has_turn>(queen_w);

    const auto queen_b = reg.create();
    reg.emplace<pos>(queen_b, 7, 4);
    reg.emplace<piece>(queen_b, false);
    reg.emplace<render>(queen_b,
                        "/O\\",
                        "L J",
                        "\\_/"
    );
    reg.emplace<_movement>(queen_b);
    reg.emplace<_queen>(queen_b);

    const auto king_w = reg.create();
    reg.emplace<pos>(king_w, 0, 4);
    reg.emplace<piece>(king_w, true);
    reg.emplace<render>(king_w,
                        "/M\\",
                        "L#J",
                        "\\#/"
    );
    reg.emplace<_movement>(king_w);
    reg.emplace<_king>(king_w);
    reg.emplace<_has_turn>(king_w);

    const auto king_b = reg.create();
    reg.emplace<pos>(king_b, 7, 3);
    reg.emplace<piece>(king_b, false);
    reg.emplace<render>(king_b,
                        "/M\\",
                        "L J",
                        "\\_/"
    );
    reg.emplace<_movement>(king_b);
    reg.emplace<_king>(king_b);
}

void draw_piece(WINDOW *win, const render &r, const pos &p)
{
    mvwprintw(win, p.x+2, p.y+3, "%s",r.ch[0].c_str());
    mvwprintw(win, p.x+3, p.y+3, "%s",r.ch[1].c_str());
    mvwprintw(win, p.x+4, p.y+3, "%s",r.ch[2].c_str());
}


void draw_piece_nooffs(WINDOW *win, const render &r, const pos &p)
{
    mvwprintw(win, p.x, p.y, "%s",r.ch[0].c_str());
    mvwprintw(win, p.x, p.y, "%s",r.ch[1].c_str());
    mvwprintw(win, p.x, p.y, "%s",r.ch[2].c_str());
}

void draw_board(entt::registry& reg, WINDOW* win)
{
    // print board
    auto view = reg.view<const pos, const renderable, const square>();

    for (auto [ent, pos, square]: view.each())
    {
        auto posx = pos.x * SQUARE_SIZE;
        auto posy = pos.y * SQUARE_SIZE_H;
        rectangle(win, posx, posy, posx+SQUARE_SIZE, posy+SQUARE_SIZE_H);
        if(square.is_white)
        {
            wattron(win, COLOR_PAIR(2));
            fill_rect(win, posx, posy, "#");
            wattroff(win, COLOR_PAIR(2));
        }
        else
        {
            fill_rect(win, posx, posy, " ");

        }

        mvwprintw(win, posx+1, posy+1, "%d,%d",pos.y,pos.x);
    }
}

void draw_pieces(entt::registry& reg, WINDOW* win)
{

    // print pieces
    auto piece_view = reg.view<const pos, const piece, const render>();

    for (auto [ent, pos, piece, rend]: piece_view.each())
    {
        auto posx = pos.x * SQUARE_SIZE;
        auto posy = pos.y * SQUARE_SIZE_H;

        mvwprintw(win, posx+2, posy+3, "%s",rend.ch[0].c_str());
        mvwprintw(win, posx+3, posy+3, "%s",rend.ch[1].c_str());
        mvwprintw(win, posx+4, posy+3, "%s",rend.ch[2].c_str());

    }

    // print pieces
    auto selected_piece_view = reg.view<const pos, const piece, const render, const _selected>();

    for (auto [ent, pos, piece, rend]: selected_piece_view.each())
    {
        mvprintw(0, 0, "                               ");
        mvprintw(0, 0, "Drawing");
        auto posx = pos.x * SQUARE_SIZE;
        auto posy = pos.y * SQUARE_SIZE_H;
        wattron(win, COLOR_PAIR(1));
        mvwprintw(win, posx+2, posy+3, "%s",rend.ch[0].c_str());
        mvwprintw(win, posx+3, posy+3, "%s",rend.ch[1].c_str());
        mvwprintw(win, posx+4, posy+3, "%s",rend.ch[2].c_str());
        wattroff(win, COLOR_PAIR(1));
    }
}

void draw_possible_moves(entt::registry& reg, WINDOW* win)
{
    // print board
    auto view = reg.view<const pos, const renderable, const square, const _highlighted>();

    //db->Debug("drawing");
    for (auto [ent, pos, square]: view.each())
    {
        auto posx = pos.x * SQUARE_SIZE;
        auto posy = pos.y * SQUARE_SIZE_H;
        rectangle(win, posx, posy, posx+SQUARE_SIZE, posy+SQUARE_SIZE_H);
        wattron(win, COLOR_PAIR(CLR_HIGHLIGHT_SQR));
        if(square.is_white)
        {
            fill_rect(win, posx, posy, "#");
        }
        else
        {
            fill_rect(win, posx, posy, " ");

        }
        wattroff(win, COLOR_PAIR(CLR_HIGHLIGHT_SQR));

        mvwprintw(win, posx+1, posy+1, "%d,%d",pos.y,pos.x);
    }
    auto view_capture = reg.view<const pos, const renderable, const square, const _capturable>();

    for (auto [ent, pos, square]: view_capture.each())
    {
        auto posx = pos.x * SQUARE_SIZE;
        auto posy = pos.y * SQUARE_SIZE_H;
        rectangle(win, posx, posy, posx+SQUARE_SIZE, posy+SQUARE_SIZE_H);
        wattron(win, COLOR_PAIR(CLR_CAPTURE));
        if(square.is_white)
        {
            fill_rect(win, posx, posy, "#");
        }
        else
        {
            fill_rect(win, posx, posy, " ");

        }
        wattroff(win, COLOR_PAIR(CLR_CAPTURE));

        mvwprintw(win, posx+1, posy+1, "%d,%d",pos.y,pos.x);
    }

    if(DEBUG_SHOW_MOVES)
    {
        auto view_moves = reg.view<const pos, const renderable, const square, const _allmoves>();

        //db->Debug("drawing");
        for (auto [ent, pos, square]: view_moves.each())
        {
            auto posx = pos.x * SQUARE_SIZE;
            auto posy = pos.y * SQUARE_SIZE_H;
            rectangle(win, posx, posy, posx+SQUARE_SIZE, posy+SQUARE_SIZE_H);
            wattron(win, COLOR_PAIR(CLR_ACT_MOVES));
            if(square.is_white)
            {
                fill_rect(win, posx, posy, "#");
            }
            else
            {
                fill_rect(win, posx, posy, " ");

            }
            wattroff(win, COLOR_PAIR(CLR_ACT_MOVES));
        }

        auto view_crash = reg.view<const pos, const renderable, const square, const _crash>();

        //db->Debug("drawing");
        for (auto [ent, pos, square]: view_crash.each())
        {
            auto posx = pos.x * SQUARE_SIZE;
            auto posy = pos.y * SQUARE_SIZE_H;
            rectangle(win, posx, posy, posx+SQUARE_SIZE, posy+SQUARE_SIZE_H);
            wattron(win, COLOR_PAIR(CLR_CRASH));
            if(square.is_white)
            {
                fill_rect(win, posx, posy, "#");
            }
            else
            {
                fill_rect(win, posx, posy, " ");

            }
            wattroff(win, COLOR_PAIR(CLR_CRASH));
        }
    }
}



void draw_sideview(entt::registry& reg, WINDOW* win, DebugLogger *db)
{
    // print board
    auto view = reg.view<const render, const piece>(entt::exclude_t<pos>());

    int posy = 1;
    for (auto [ent, r, p]: view.each())
    {
        int posx = p.is_white ? 1 : 9;

        if(reg.any_of<_pawn>(ent))
        {
            posx += 0;
            pos p = pos{posx,posy};
            draw_piece_nooffs(win, r, p);
        }
        else if(reg.any_of<_rook>(ent))
        {
            posx += 3;
            pos p = pos{posx,posy};
            draw_piece_nooffs(win, r, p);

        }
        else if(reg.any_of<_bishop>(ent))
        {
            posx += 6;
            pos p = pos{posx,posy};
            draw_piece_nooffs(win, r, p);

        }
        else if(reg.any_of<_knight>(ent))
        {
            posx += 9;
            pos p = pos{posx,posy};
            draw_piece_nooffs(win, r, p);

        }
        else if(reg.any_of<_queen>(ent))
        {
            posx += 12;
            pos p = pos{posx,posy};
            draw_piece_nooffs(win, r, p);
        }

        posy += 2;
    }


    wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
}

void draw(entt::registry& reg, WINDOW* win, WINDOW* side_win, DebugLogger *db)
{
    draw_board(reg, win);
    draw_pieces(reg, win);
    draw_possible_moves(reg, win);

    draw_sideview(reg, side_win, db);

    refresh();
    wrefresh(win);
    wrefresh(side_win);
}

void change_turn(entt::registry &reg, bool &is_white)
{
    is_white = !is_white;

    auto view = reg.view<piece>();
    for(auto [ent, p]: view.each())
    {
        if(p.is_white == is_white)
            reg.emplace_or_replace<_has_turn>(ent);
        else
            reg.remove<_has_turn>(ent);
    }
}
