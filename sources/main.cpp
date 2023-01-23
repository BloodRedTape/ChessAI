#include <core/print.hpp>
#include <core/function.hpp>
#include <core/ranges/algorithm.hpp>
#include <core/ranges/filtered.hpp>
#include <core/ranges/transformed.hpp>

#include "chess.hpp"
#include "char.hpp"
#include "pixel_engine.hpp"
#include <string>
#include <iostream>

class ChessGame : public PixelEngine {
public:
	ChessGame():
		PixelEngine("Chess", 8, 8, 100)
	{}

	void OnUpdate(float dt)override {
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				DrawRect(i, j, (i * 9 + j) % 2 == 0 ? Color::White : Color::Black);
	}
};


int main() {

	GPU::ForceInit();

	ChessGame game;
	game.Run();
}