#pragma once

#include <core/list.hpp>
#include <core/math/vector2.hpp>
#include <core/optional.hpp>
#include "chess.hpp"

struct Board {
    Optional<Figure> Figures[8][8];

    Board();

    void Dump();

    Optional<Figure>& operator[](Position position) {
        return Get(position);
    }

    Optional<Figure>& Get(Position position) {
        return Figures[position.Y][position.X];
    }

    Optional<Figure>& operator[](SIPosition position) {
        return Get(position.ToPosition());
    }
    Optional<Figure>& operator[](Vector2s position) {
        Position pos;
        pos.X = position.y;
        pos.Y = position.x;
        return Get(pos);
    }

    void DoMove(Position src, Position dst);

    List<Position> DumpPossibleMoves(Position source, Figure src_figure);
};