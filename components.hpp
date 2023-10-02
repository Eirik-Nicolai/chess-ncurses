#ifndef COMPONENTS_H_
#define COMPONENTS_H_

#include <entt/entt.hpp>
#include <ncurses.h>

#include <string>

enum class TYPE{Pawn, Rook, Knight, Bishop, King, Queen};

struct piece
{
    bool is_white;
    //TYPE type;

};
struct _pawn
{
    bool has_moved;
};
struct _rook{};
struct _knight{};
struct _bishop{};
struct _king{};
struct _queen{};



struct render
{
    std::string ch[3];
};

struct _selected{};

struct _castle{};

struct _movement{};

struct square
{
    bool is_white;
};

struct _has_turn{};

struct _capturable{};

struct _highlighted{};

struct _crash{};

struct _allmoves{};

struct _occupied{};

struct pos
{
    int x;
    int y;
};

struct renderable{};




#endif // COMPONENTS_H_
