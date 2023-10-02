#include "utils.hpp"

std::tuple<int, int> get_rect_by_pos(int x, int y)
{
    return std::make_tuple((x-WINDOW_OFFS_H)/SQUARE_SIZE_H,(y-WINDOW_OFFS_V)/SQUARE_SIZE);
}

void rectangle(WINDOW* win, int y1, int x1, int y2, int x2)
{
    mvwhline(win, y1, x1, 0, x2-x1);
    mvwhline(win, y2, x1, 0, x2-x1);
    mvwvline(win, y1, x1, 0, y2-y1);
    mvwvline(win, y1, x2, 0, y2-y1);
    mvwaddch(win, y1, x1, ACS_ULCORNER);
    mvwaddch(win, y2, x1, ACS_LLCORNER);
    mvwaddch(win, y1, x2, ACS_URCORNER);
    mvwaddch(win, y2, x2, ACS_LRCORNER);
}

void fill_rect(WINDOW* win, int x, int y, std::string ch)
{
    for (auto i = 1; i < SQUARE_SIZE; ++i)
    {
        for (auto j = 1; j < SQUARE_SIZE_H; ++j)
        {
            mvwprintw(win, x+i, y+j, "%s",ch.c_str());
        }
    }
}

void fill_rect(WINDOW* win, int x, int y, char* ch, int col)
{
    wattron(win, COLOR_PAIR(col));
    fill_rect(win, x, y, ch);
    wattroff(win, COLOR_PAIR(col));
}

void DebugLogger::Debug(std::string s)
{
    DebugLines.push_back("[DEBUG]  " + s);
}


void DebugLogger::DebugPrint()
{
    int offs = 0;
    mvprintw(offs, 0, "[DEBUG PRINT]");
    for(auto s: DebugLines)
    {
        ++offs;
        mvprintw(offs, 0,"%s", s.c_str());
    }

    DebugLines.clear();
}

void clear_highlighting(entt::registry &reg, DebugLogger *db)
{
    db->Debug("Clearing" );
    reg.clear<_highlighted>();
    reg.clear<_selected>();
    reg.clear<_allmoves>();
    reg.clear<_crash>();
    reg.clear<_capturable>();
}
