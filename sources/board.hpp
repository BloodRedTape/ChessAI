#pragma once

#include <core/list.hpp>
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
        return Figures[position.X][position.Y];
    }

    Optional<Figure>& operator[](SIPosition position) {
        return Get(position.ToPosition());
    }

    void DoMove(Position src, Position dst);

    List<Position> DumpPossibleMoves(Position source);
};