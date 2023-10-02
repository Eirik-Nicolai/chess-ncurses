#ifndef UTILS_H_
#define UTILS_H_

#include <entt/entt.hpp>
#include <ncurses.h>
#include <iostream>

#include <string>

#include "components.hpp"

#define CLEAR_LINE "       "

#define CLR_HIGHLIGHT_SQR 1
#define CLR_WHITE_SQUARE  2
#define CLR_CRASH         3
#define CLR_ACT_MOVES     4
#define CLR_CAPTURE       5

#define WINDOW_OFFS_H 40
#define WINDOW_OFFS_V 10
const int8_t SQUARE_SIZE   = 5;
const int8_t SQUARE_SIZE_H = 9;


#define DEBUG_SHOW_MOVES 0

class DebugLogger
{
    private:
        std::vector<std::string> DebugLines;

    public:
        void DebugPrint();

        void Debug(std::string);

        void Debug(std::string s, pos p)
        {
            Debug(s + "  [" + std::to_string(p.y) + ", " + std::to_string(p.x) + "]");
        }

        template<typename T, typename TT>
        void Debug(std::string s, std::pair<T, TT> p)
        {
            Debug(s + ": [" + std::to_string(p.first) + ", "+ std::to_string(p.second) + "]");
        }

        template<typename T>
        void Debug(std::string s, T t)
        {
            Debug(s + " " + std::to_string(t));
        }

        void Debug(std::string s, std::vector<pos> v)
        {
            std::string out;
            for (auto s:v)
            {
                out.append("["+std::to_string(s.y)+","+std::to_string(s.x)+"]");
                out.append(",");
            }
            Debug(s + "{" + out + "}");
        }

};

std::tuple<int, int> get_rect_by_pos(int x, int y);


void rectangle(WINDOW* win, int y1, int x1, int y2, int x2);
void fill_rect(WINDOW* win, int x, int y, std::string ch);
void fill_rect(WINDOW* win, int x, int y, char* ch, int col);

void clear_highlighting(entt::registry&, DebugLogger*);


#endif // UTILS_H_
