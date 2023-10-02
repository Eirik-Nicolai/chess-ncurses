#include "movement.hpp"

void find_possible_moves(entt::registry& reg, DebugLogger* db)
{
    // get moves
    bool crash_check = true;
    int8_t is_pawn = 0;
    pos selected_pos;

    auto selected_view = reg.view<const piece, const pos, const _selected>();
    std::vector<std::vector<pos>> moveset;
    for (auto [ent, piece, p]: selected_view.each())
    {
        if(reg.any_of<_pawn>(ent))
        {
            is_pawn = piece.is_white ? 1 : -1;
            auto move = reg.get<_pawn>(ent).has_moved;
            moveset.emplace_back(
                pawn_movement(
                    is_pawn,
                    move,
                    p
                ));
            move = false;
        }
        else if(reg.any_of<_knight>(ent))
        {
            crash_check = false;
            moveset.emplace_back(knight_movement(p));
        }
        else if(reg.any_of<_bishop>(ent))
        {
            moveset = bishop_movement(p);
        }
        else if(reg.any_of<_rook>(ent))
        {
            moveset = rook_movement(p);
        }
        else if(reg.any_of<_king>(ent))
        {
            crash_check = false;
            moveset.emplace_back(king_movement(p));
        }
        else if(reg.any_of<_queen>(ent))
        {
            moveset = queen_movement(p);
        }
        else
        {
            std::cerr << "ERR";
        }

        selected_pos = p;
    }


    std::vector<pos> moves;
    std::vector<pos> crash;

    std::vector<pos> capture;

    auto piece_view = reg.view<const piece, const pos>();
    if (is_pawn != 0)
    {
        for (auto [ent, _, pos]: piece_view.each())
        {
            if(pos.x == selected_pos.x+is_pawn)
            {
                if(pos.y == selected_pos.y - 1 || pos.y == selected_pos.y + 1)
                {
                    capture.push_back(pos);
                }
            }
        }
    }
    for(auto dir: moveset)
    {
        for (auto move: dir)
        {
            bool crashed = false;
            for (auto [ent, _, pos]: piece_view.each())
            {
                if (pos.x == move.x && pos.y == move.y)
                {
                    crash.emplace_back(move);
                    crashed = true;
                    // can we capture this piece
                    if(!reg.all_of<_has_turn>(ent))
                    {
                        if(is_pawn == 0)
                            capture.push_back(pos);
                    }
                    break;
                }
            }
            if(crashed)
            {
                if(crash_check)break;
            }
            else
            {
                moves.emplace_back(move);
            }
        }
    }

    // set moves
    // TODO could maybe do view except ones with pieces on them
    auto squares_view = reg.view<const square, const pos>();
    reg.clear<_highlighted>();
    for (auto [ent, square, pos]: squares_view.each())
    {
        for(auto dir: moveset)
        {
            for (auto move: dir)
            {
                if (pos.x == move.x && pos.y == move.y)
                {
                    reg.emplace_or_replace<_allmoves>(ent);
                }
            }
        }
        for(auto movepos: crash)
        {
            if (pos.x == movepos.x && pos.y == movepos.y)
            {
                reg.emplace_or_replace<_crash>(ent);
            }
        }
        for(auto movepos: moves)
        {
            if (pos.x == movepos.x && pos.y == movepos.y)
            {
                reg.emplace_or_replace<_highlighted>(ent);
            }
        }
        for(auto movepos: capture)
        {
            if (pos.x == movepos.x && pos.y == movepos.y)
            {
                db->Debug("FOr capture: ", pos);
                reg.emplace_or_replace<_capturable>(ent);
            }
        }
    }
}



std::vector<pos> pawn_movement(int8_t delta, bool first_move, pos p)
{
    auto moves = std::vector<pos>();
    moves.emplace_back(pos{p.x+1 * delta, p.y});
    if(!first_move)moves.emplace_back(pos{p.x+2 * delta, p.y});
    return moves;
}

std::vector<pos> knight_movement(pos p)
{
    auto moves = std::vector<pos>();

    moves.emplace_back(pos{p.x+2, p.y+1});
    moves.emplace_back(pos{p.x+2, p.y-1});
    moves.emplace_back(pos{p.x+1, p.y+2});
    moves.emplace_back(pos{p.x+1, p.y-2});

    moves.emplace_back(pos{p.x-2, p.y+1});
    moves.emplace_back(pos{p.x-2, p.y-1});
    moves.emplace_back(pos{p.x-1, p.y+2});
    moves.emplace_back(pos{p.x-1, p.y-2});

    return moves;
}

std::vector<std::vector<pos>> rook_movement(pos p)
{
    auto moves_n = std::vector<pos>();
    auto moves_e = std::vector<pos>();
    auto moves_s = std::vector<pos>();
    auto moves_w = std::vector<pos>();
    for (int i = 1; i < 8; ++i)
        if(p.x+i < 8)  moves_n.emplace_back(pos{p.x+i, p.y});
    for (int i = 1; i < 8; ++i)
        if(p.y+i < 8)  moves_e.emplace_back(pos{p.x, p.y+i});
    for (int i = 1; i < 8; ++i)
        if(p.x-i >= 0) moves_s.emplace_back(pos{p.x-i, p.y});
    for (int i = 1; i < 8; ++i)
        if(p.y-i >= 0) moves_w.emplace_back(pos{p.x, p.y-i});

    return std::vector<std::vector<pos>>({
        moves_n,
        moves_e,
        moves_s,
        moves_w

    });
}

std::vector<std::vector<pos>> bishop_movement(pos p)
{
    auto moves_n = std::vector<pos>();
    auto moves_e = std::vector<pos>();
    auto moves_s = std::vector<pos>();
    auto moves_w = std::vector<pos>();
    for (int i = 1; i < 8; ++i)
        if(p.x+i < 8)  moves_n.emplace_back(pos{p.x+i, p.y+i});
    for (int i = 1; i < 8; ++i)
        if(p.y-i >= 0)  moves_e.emplace_back(pos{p.x-i,p.y+i});
    for (int i = 1; i < 8; ++i)
        if(p.x+i < 8) moves_s.emplace_back(pos{p.x+i, p.y-i});
    for (int i = 1; i < 8; ++i)
        if(p.y-i >= 0) moves_w.emplace_back(pos{p.x-i, p.y-i});

    return std::vector<std::vector<pos>>({
        moves_n,
        moves_e,
        moves_s,
        moves_w

    });
}


std::vector<std::vector<pos>> queen_movement(pos p)
{
    auto moves = std::vector<std::vector<pos>>();
    auto rook_moves = rook_movement(p);
    auto bish_moves = bishop_movement(p);
    moves.insert(moves.end(),rook_moves.begin(), rook_moves.end());
    moves.insert(moves.end(),bish_moves.begin(), bish_moves.end());
    return moves;
}

std::vector<pos> king_movement(pos p)
{
    auto moves = std::vector<pos>();
    moves.emplace_back(pos{p.x+1, p.y+1});
    moves.emplace_back(pos{p.x+1, p.y-1});
    moves.emplace_back(pos{p.x-1, p.y+1});
    moves.emplace_back(pos{p.x-1, p.y-1});

    moves.emplace_back(pos{p.x+1, p.y});
    moves.emplace_back(pos{p.x-1, p.y});
    moves.emplace_back(pos{p.x, p.y+1});
    moves.emplace_back(pos{p.x, p.y-1});
    return moves;
}
