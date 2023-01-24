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

    Position(char x = 'a', int y = 1) {
        Y = y -  1;
        X = x - 'a';
    }

    bool IsValid()const {
        return X >= 0 && X <= 7 && Y >= 0 && Y <= 7;
    }

    char GetInitialX()const{
        return X + 'a';
    }

    int GetInitialY()const{
        return Y + 1;
    }

	bool operator==(const Position &other)const{
		return X == other.X && Y == other.Y;
	}
};


template<>
struct Printer<Position>{
	static void Print(const Position &value, StringWriter &writer){
		Printer<char>::Print('(', writer);
		Printer<char>::Print(value.GetInitialX(), writer);
		Printer<char>::Print(',', writer);
		Printer<int>::Print(value.GetInitialY(), writer);
		Printer<char>::Print(')', writer);
	}
};

struct SIPosition {
	int X;
	int Y;
	FigureSide Side;

	SIPosition(int x, int y, FigureSide side):
		X(x),
		Y(y),
		Side(side)
	{}

	SIPosition(Position pos, FigureSide side) :
		Side(side)
	{
		//SX_ASSERT(pos.IsValid());

		Y = (Side == FigureSide::White ? pos.Y : 7 - pos.Y);
		X = pos.X;
	}

	Position ToPosition()const {
		Position pos;
		pos.Y = (Side == FigureSide::White ? Y : 7 - Y);
		pos.X = X;
		return pos;
	}

	bool IsValid()const {
		return ToPosition().IsValid();
	}

    SIPosition Adv(int offset_x, int offset_y)const {
        return { X + offset_x, Y + offset_y, Side };
    }
};

inline bool IsValidMoveString(const std::string& move) {
	return move.size() == 4
		&& IsLowerCaseLetter(move[0])
		&& IsDigit(move[1])
		&& IsLowerCaseLetter(move[2])
		&& IsDigit(move[3]);
}

