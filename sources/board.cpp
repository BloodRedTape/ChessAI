#include "board.hpp"
#include "moves_builder.hpp"

Board::Board()
{
	static FigureType types[8] = { FigureType::Rook, FigureType::Knight, FigureType::Bishop, FigureType::Queen, FigureType::King, FigureType::Bishop, FigureType::Knight, FigureType::Rook };

	for (int i = 0; i < 8; i++) {
		Figures[1][i] = { {FigureType::Pawn, FigureSide::White} };
		Figures[6][i] = { { FigureType::Pawn, FigureSide::Black} };
	}
	for (int i = 0; i < 8; i++) {
		Figures[0][i] = { { types[i], FigureSide::White } };
		Figures[7][i] = { { types[i], FigureSide::Black } };
	}
}

void Board::Dump() {
	for (int i = 0; i < 8; i++) {
		Print("%", 8 - i);
		for (int j = 0; j < 8; j++) {
			const auto& figure = Figures[8 - i - 1][j];
			Print("|");
			Print(ToColor(figure));
			Print("%", ToChar(figure));
			Print(NC);
		}
		Print("|\n");
	}
	Println("  a b c d e f g h");
}

void Board::DoMove(Position src, Position dst) {
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

List<Position> Board::DumpPossibleMoves(Position source) {
	Optional<Figure>& src_figure = (*this)[source];

	if (!src_figure.HasValue())
		return {};

	FigureType type = src_figure.Value().Type;
	FigureSide side = src_figure.Value().Side;

	SIPosition src(source, side);

	MovesBuilder builder(*this, src);

	switch (type) {
	case FigureType::Pawn: {
		builder.Translate(1, 0);
		if (src.X == 1)
			builder.Translate(2, 0);
		builder.Capture(1, 1);
		builder.Capture(1, -1);
	}break;
	case FigureType::Bishop: {
		builder.Axis(1, 1);
		builder.Axis(-1, 1);
		builder.Axis(1, -1);
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
		builder.Axis(1, 0);
		builder.Axis(-1, 0);
		builder.Axis(0, 1);
		builder.Axis(0, -1);
	}break;
	case FigureType::Queen: {
		builder.Axis(1, 1);
		builder.Axis(-1, 1);
		builder.Axis(1, -1);
		builder.Axis(-1, -1);

		builder.Axis(1, 0);
		builder.Axis(-1, 0);
		builder.Axis(0, 1);
		builder.Axis(0, -1);
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