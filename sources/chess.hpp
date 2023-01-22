#pragma once

#include <core/optional.hpp>
#include <core/print.hpp>
#include <core/list.hpp>

enum class FigureType {
    Pawn = 'P',
    Bishop = 'B',
    Knight = 'k',
    Rook = 'R',
    Queen = 'Q',
    King = 'K'
};

enum class FigureSide {
	White,
	Black
};

struct Figure {
    FigureType Type;
    FigureSide Side;

    Figure(FigureType type = FigureType::Pawn, FigureSide side = FigureSide::White):
        Type(type),
        Side(side)
    {}
};

inline char ToChar(const Optional<Figure> &figure) {
    if (figure.HasValue())
        return (char)figure.Value().Type;
    else
        return ' ';
}

constexpr char* NC  = "\033[0m";
constexpr char* RED = "\033[38;2;255;0;0m";
constexpr char* GRN = "\033[38;2;0;255;0m";

inline const char* ToColor(const Optional<Figure>& figure) {
    if (figure.HasValue()) {
        if (figure.Value().Side == FigureSide::White)
            return GRN;
        else
            return RED;
    }
    return NC;
}


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

struct Board {
    Optional<Figure> Figures[8][8];

    Board()
    {
        static FigureType types[8] = { FigureType::Rook, FigureType::Knight, FigureType::Bishop, FigureType::Queen, FigureType::King, FigureType::Bishop, FigureType::Knight, FigureType::Rook };

        for (int i = 0; i < 8; i++) {
            Figures[1][i] = {{FigureType::Pawn, FigureSide::White}};
            Figures[6][i] = {{ FigureType::Pawn, FigureSide::Black}};
        }
        for (int i = 0; i < 8; i++) {
            Figures[0][i] = {{ types[i], FigureSide::White }};
            Figures[7][i] = {{ types[i], FigureSide::Black }};
        }
    }

    void Dump() {
        for (int i = 0; i < 8; i++) {
            Print("%", 8 - i);
            for (int j = 0; j < 8; j++) {
                const auto& figure = Figures[ 8 - i - 1][j];
                Print("|");
                Print(ToColor(figure));
                Print("%", ToChar(figure));
                Print(NC);
            }
            Print("|\n");
        }
        Println("  a b c d e f g h");
    }

    Optional<Figure>& operator[](Position position) {
        return Get(position);
    }

    Optional<Figure>& Get(Position position) {
        return Figures[position.X][position.Y];
    }

    Optional<Figure>& operator[](SIPosition position) {
        return Get(position.ToPosition());
    }

    void DoMove(Position src, Position dst) {
        if (!src.IsValid())
            return Println("Move is not possible, source position is invalid: %", src);
        if (!dst.IsValid())
            return Println("Move is not possible, destination position is invalid: %", dst);
        
        
        auto& src_figure = Figures[src.X][src.Y];

        if (!src_figure.HasValue())
            return Println("Position % has no figure", src);
        //check if move is possible;
        Figures[dst.X][dst.Y] = Move(src_figure);
    }
};
