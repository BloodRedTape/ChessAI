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
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				int x = i;
				int y = j;
				DrawRect(x, y, (x * 9 + y) % 2 == 0 ? Color((u8)222, 192, 158, 255) : Color((u8)89, 62, 30, 255));
				const Optional<Figure> &figure = m_Board.Figures[7 - y][x];

				if (figure.HasValue())
					DrawRect(x, y, m_FigureTextures[(int)figure.Value().Type][(int)figure.Value().Side]);
			}
		}
	}
};


int main() {
	Directory::Change(SX_UTF8("../../../"));

	GPU::ForceInit();

	ChessGame game;
	game.Run();
}