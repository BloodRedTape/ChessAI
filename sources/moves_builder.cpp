#include "moves_builder.hpp"
#include "board.hpp"

MovesBuilder::MovesBuilder(Board& board, SIPosition source) :
	m_Board(board),
	m_Source(source)
{}
	
void MovesBuilder::Axis(int dx, int dy) {
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

void MovesBuilder::Capture(int x, int y) {
	auto pos = m_Source.Adv(x, y);
	if (IsValidEnemy(pos))
		m_Positions.Add(pos);
}

void MovesBuilder::Translate(int x, int y) {
	auto pos = m_Source.Adv(x, y);
	if (IsValidEmpty(pos))
		m_Positions.Add(pos);
}

void MovesBuilder::CaptureOrTranslate(int x, int y) {
	auto pos = m_Source.Adv(x, y);
	if (IsValidEnemy(pos) || IsValidEmpty(pos))
		m_Positions.Add(pos);
}

List<Position>&& MovesBuilder::Dump() {
	return Move(m_Positions);
}

bool MovesBuilder::IsValidEmpty(SIPosition position) {
	if (position.IsValid())
		return !m_Board[position].HasValue();
	return false;
}

bool MovesBuilder::IsValidEnemy(SIPosition position) {
	if (position.IsValid() && m_Board[position].HasValue())
		return m_Board[position].Value().Side != position.Side;
	return false;
}