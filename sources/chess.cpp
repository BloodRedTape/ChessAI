#include "chess.hpp"
#include <cassert>

const char* FigureTypeToString(FigureType type) {
	switch (type) {
	case FigureType::Pawn:
		return "Pawn";
	case FigureType::Bishop:
		return "Bishop";
	case FigureType::Knight:
		return "Knight";
	case FigureType::Rook:
		return "Rook";
	case FigureType::Queen:
		return "Queen";
	case FigureType::King:
		return "King";
	default:
		assert(false);
		return "";
	}
}
const char* FigureSideToString(FigureSide side) {
	switch (side) {
	case FigureSide::Black:
		return "Black";
	case FigureSide::White:
		return "White";
	default:
		assert(false);
		return "";
	}
}
