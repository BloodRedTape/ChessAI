#pragma once

#include <core/print.hpp>
#include <string>
#include <iostream>
#include <core/optional.hpp>
#include "char.hpp"

enum class FigureType {
    Pawn,
    Bishop,
    Knight,
    Rook,
    Queen,
    King
};

enum class FigureSide {
	White,
	Black
};

const char* FigureTypeToString(FigureType type);
const char* FigureSideToString(FigureSide side);

struct Figure {
    FigureType Type;
    FigureSide Side;

    Figure(FigureType type = FigureType::Pawn, FigureSide side = FigureSide::White):
        Type(type),
        Side(side)
    {}
};

struct Position {
    int X = -1, Y = -1;

    Position(char y = 'a', int x = 1) {
        Y = y - 'a';
        X = x - 1;
    }

    bool IsValid()const {
        return X >= 0 && X <= 7 && Y >= 0 && Y <= 7;
    }

    int GetInitialX()const{
        return X + 1;
    }

    char GetInitialY()const{
        return Y + 'a';
    }
};


template<>
struct Printer<Position>{
	static void Print(const Position &value, StringWriter &writer){
		Printer<char>::Print('(', writer);
		Printer<char>::Print(value.GetInitialY(), writer);
		Printer<char>::Print(',', writer);
		Printer<int>::Print(value.GetInitialX(), writer);
		Printer<char>::Print(')', writer);
	}
};

struct SIPosition {
	int X;
	int Y;
	FigureSide Side;

	SIPosition(Position pos, FigureSide side) :
		Side(side)
	{
		SX_ASSERT(pos.IsValid());

		X = (side == FigureSide::White ? pos.X : 7 - pos.X);
		Y = pos.Y;
	}

	Position ToPosition()const {
		Position pos;
		pos.X = (Side == FigureSide::White ? X : 7 - X);
		pos.Y = Y;
		return pos;
	}

	bool IsValid()const {
		return ToPosition().IsValid();
	}

    operator Position()const {
        return ToPosition();
    }

    SIPosition Adv(int offset_x, int offset_y)const {
        Position pos;
        pos.X = X + offset_x;
        pos.Y = Y + offset_y;
        return { pos, Side };
    }
};

inline bool IsValidMoveString(const std::string& move) {
	return move.size() == 4
		&& IsLowerCaseLetter(move[0])
		&& IsDigit(move[1])
		&& IsLowerCaseLetter(move[2])
		&& IsDigit(move[3]);
}

