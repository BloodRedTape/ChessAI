#include <core/print.hpp>
#include <core/function.hpp>
#include <core/string_print.hpp>
#include <core/ranges/algorithm.hpp>
#include <core/ranges/filtered.hpp>
#include <core/ranges/transformed.hpp>
#include <core/os/directory.hpp>

#include "chess.hpp"
#include "char.hpp"
#include "board.hpp"
#include "pixel_engine.hpp"
#include <string>
#include <iostream>

class ChessGame : public PixelEngine {
private:
	Board m_Board;
	const Texture2D* m_FigureTextures[6][2] = {};

	Optional<Figure> m_DragingFigure;
	Position m_DragFigurePosition;
	Vector2f m_DragingPosition;
	Vector2f m_DragInFigureOffset;
public:
	ChessGame():
		PixelEngine("Chess", 8, 8, 100)
	{

		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 6; j++) {
				String path = StringPrint("resources/%/%.png", FigureSideToString((FigureSide)i), FigureTypeToString((FigureType)j));
				m_FigureTextures[j][i] = Texture2D::Create(path);
			}
		}
		
	}

	void OnUpdate(float dt)override {
		m_DragingPosition = MouseToPixel(Mouse::RelativePosition(Window()));

		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				int x = i;
				int y = j;
				DrawPixel(x, y, (x * 9 + y) % 2 == 0 ? Color((u8)222, 192, 158, 255) : Color((u8)100, 62, 30, 255));
			}
		}


		if (m_DragingFigure.HasValue()) {
			List<Position> moves = m_Board.DumpPossibleMoves(m_DragFigurePosition, m_DragingFigure.Value());

			for (const Position& pos : moves) {
				const bool is_attacking = m_Board[pos].HasValue();
				Color semi_transparent(1.f, 1.f, 1.f, 0.5f);
				DrawPixel(pos.X, 7 - pos.Y,  (is_attacking ? Color::Red : Color::Orange) * semi_transparent);
			}

			DrawPixel(m_DragingPosition.x - m_DragInFigureOffset.x, m_DragingPosition.y - m_DragInFigureOffset.y, FigureTexture(m_DragingFigure.Value()));
		}

		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				int x = i;
				int y = j;
				const Optional<Figure> &figure = m_Board.Figures[7 - y][x];
				if (figure.HasValue())
					DrawPixel(x, y, FigureTexture(figure.Value()));
			}
		}
	}

	void OnEvent(const Event& e)override {
		PixelEngine::OnEvent(e);
		
		if (e.Type == EventType::MouseButtonPress) {
			Vector2f pixel = MouseToPixel({e.MouseButtonPress.x, e.MouseButtonPress.y});
			Vector2s figure_coords = pixel;
			m_DragInFigureOffset = pixel - Vector2f(figure_coords);
			m_DragFigurePosition.Y = 7 - figure_coords.y;
			m_DragFigurePosition.X = figure_coords.x;
			m_DragingFigure = Move(m_Board[m_DragFigurePosition]);
		}

		if (e.Type == EventType::MouseButtonRelease) {
			Vector2f pixel = MouseToPixel({e.MouseButtonPress.x, e.MouseButtonPress.y});
			Vector2s figure_coords = pixel;


			List<Position> moves = m_Board.DumpPossibleMoves(m_DragFigurePosition, m_DragingFigure.Value());

			Position try_pos;
			try_pos.X = figure_coords.x;
			try_pos.Y = 7 - figure_coords.y;

			if (moves | Contains(try_pos))
				m_Board[try_pos] = Move(m_DragingFigure);
			else
				m_Board[m_DragFigurePosition] = Move(m_DragingFigure);
		}
	}

	const Texture2D* FigureTexture(const Figure& figure) {
		return m_FigureTextures[(int)figure.Type][(int)figure.Side];
	}
};


int main() {
	Directory::Change(SX_UTF8("../../../"));

	Position pos('a', 2);
	SIPosition si(pos, FigureSide::White);
	SX_ASSERT(pos.X == si.ToPosition().X);
	SX_ASSERT(pos.Y == si.ToPosition().Y);

	GPU::ForceInit();

	ChessGame game;
	game.Run();
}