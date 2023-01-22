#include <core/print.hpp>
#include <core/function.hpp>
#include <core/ranges/algorithm.hpp>
#include <core/ranges/filtered.hpp>
#include <core/ranges/transformed.hpp>
#include "chess.hpp"
#include "char.hpp"
#include <string>
#include <iostream>

bool IsValidMoveString(const std::string& move) {
	return move.size() == 4
		&& IsLowerCaseLetter(move[0])
		&& IsDigit(move[1])
		&& IsLowerCaseLetter(move[2])
		&& IsDigit(move[3]);
}

class MovesBuilder {
	List<Position> m_Positions;
	Board& m_Board;
	SIPosition m_Source;
public:
	MovesBuilder(Board &board, SIPosition source):
		m_Board(board),
		m_Source(source)
	{}
	
	void Axis(int dx, int dy) {
		for (int i = 1; i < 8; i++) {
			auto pos = m_Source.Adv(i * dx, i * dy);
			if (IsValidEmpty(pos)) {
				m_Positions.Add(pos);
			}
			if (IsValidEnemy(pos)) {
				m_Positions.Add(pos);
				break;
			}
		}
	}

	void Capture(int x, int y) {
		auto pos = m_Source.Adv(x, y);
		if (IsValidEnemy(pos))
			m_Positions.Add(pos);
	}

	void Translate(int x, int y) {
		auto pos = m_Source.Adv(x, y);
		if (IsValidEmpty(pos))
			m_Positions.Add(pos);
	}

	void CaptureOrTranslate(int x, int y) {
		auto pos = m_Source.Adv(x, y);
		if (IsValidEnemy(pos) || IsValidEmpty(pos))
			m_Positions.Add(pos);
	}

	List<Position>&& Dump() {
		return Move(m_Positions);
	}
private:
	bool IsValidEmpty(SIPosition position){
		if(position.IsValid())
			return !m_Board[position].HasValue();
		return false;
	}

	bool IsValidEnemy(SIPosition position) {
		if (position.IsValid() && m_Board[position].HasValue())
			return m_Board[position].Value().Side != position.Side;
		return false;
	}
};

struct Game {
private:
	Board m_Board;
public:

	void Run() {

		for (;;) {
			m_Board.Dump();

			Position src, dst;
			for (;;) {
				Print("Enter Move:");
				std::string move;
				std::getline(std::cin, move);

				if (!IsValidMoveString(move)) {
					Println("Invalid Move Encoding");
				}
				else {
					src = Position(move[0], move[1] - '0');
					dst = Position(move[2], move[3] - '0');
					break;
				}
			}

			m_Board.DoMove(src, dst);
			Print("\n");
		}
	}

	List<Position> DumpPossibleMoves(Position source) {
		Optional<Figure>& src_figure = m_Board[source];

		if (!src_figure.HasValue())
			return {};

		FigureType type = src_figure.Value().Type;
		FigureSide side = src_figure.Value().Side;
		
		SIPosition src(source, side);

		MovesBuilder builder(m_Board, src);
		
		switch (type) {
		case FigureType::Pawn: {
			builder.Translate(1, 0);
			if(src.X == 1)
				builder.Translate(2, 0);
			builder.Capture(1, 1);
			builder.Capture(1,-1);
		}break;
		case FigureType::Bishop: {
			builder.Axis( 1,  1);
			builder.Axis(-1,  1);
			builder.Axis( 1, -1);
			builder.Axis(-1, -1);
		}break;
		case FigureType::Knight: {
			builder.CaptureOrTranslate(1, 2);
			builder.CaptureOrTranslate(2, 1);
			builder.CaptureOrTranslate(-1, 2);
			builder.CaptureOrTranslate(-2, 1);
			builder.CaptureOrTranslate(1, -2);
			builder.CaptureOrTranslate(2, -1);
			builder.CaptureOrTranslate(-1, -2);
			builder.CaptureOrTranslate(-2, -1);
		}break;
		case FigureType::Rook: {
			builder.Axis( 1,  0);
			builder.Axis(-1,  0);
			builder.Axis( 0,  1);
			builder.Axis( 0, -1);
		}break;
		case FigureType::Queen: {
			builder.Axis( 1,  1);
			builder.Axis(-1,  1);
			builder.Axis( 1, -1);
			builder.Axis(-1, -1);

			builder.Axis( 1,  0);
			builder.Axis(-1,  0);
			builder.Axis( 0,  1);
			builder.Axis( 0, -1);
		}break;
		case FigureType::King: {
			builder.CaptureOrTranslate(1, 1);
			builder.CaptureOrTranslate(-1, 1);
			builder.CaptureOrTranslate(1, -1);
			builder.CaptureOrTranslate(-1, -1);

			builder.CaptureOrTranslate(1, 0);
			builder.CaptureOrTranslate(-1, 0);
			builder.CaptureOrTranslate(0, 1);
			builder.CaptureOrTranslate(0, -1);
		}break;
		default:
			SX_ASSERT(false);
		}

		return builder.Dump();
	}

	bool IsValidEmpty(SIPosition position){
		if(position.IsValid())
			return !m_Board[position].HasValue();
		return false;
	}

	bool IsValidEnemy(SIPosition position) {
		if (position.IsValid() && m_Board[position].HasValue())
			return m_Board[position].Value().Side != position.Side;
		return false;
	}
};

template<typename ValueType>
auto HigherThan(ValueType value) {
	return Filtered([=](auto current) {
		return value < current;
	});
}

inline auto Increment() {
	return Transformed([](auto value) {
		return value++;
	});
}

int main() {
	//int *res = list | Find(3);
	//bool has = list | Increment() | HigherThan(3) | Contains(3);
	//Println("%", *res);
	//Println("%", has);

	List<int> list = { 1, 2, 3, 4 };

	for (int& e : list | HigherThan(2)) 
		e = 0;

	Println("%", list);


	Position pos('a', 5);
	SIPosition ai_pos(pos, FigureSide::White);
	SX_ASSERT(pos.X == ai_pos.ToPosition().X);
	SX_ASSERT(pos.Y == ai_pos.ToPosition().Y);
	Println("%", ai_pos.ToPosition());
	Game game;
	game.Run();
}