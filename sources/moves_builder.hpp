#pragma once

#include <core/list.hpp>
#include "chess.hpp"

class Board;

class MovesBuilder {
	List<Position> m_Positions;
	Board& m_Board;
	SIPosition m_Source;
public:
	MovesBuilder(Board& board, SIPosition source);
	
	void Axis(int dx, int dy);

	void Capture(int x, int y);

	void Translate(int x, int y);

	void CaptureOrTranslate(int x, int y);

	List<Position>&& Dump();

	bool IsValidEmpty(SIPosition position);

	bool IsValidEnemy(SIPosition position);
};