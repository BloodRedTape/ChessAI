#include <core/print.hpp>
#include <core/function.hpp>
#include <core/ranges/algorithm.hpp>
#include <core/ranges/filtered.hpp>
#include <core/ranges/transformed.hpp>
#include <core/os/clock.hpp>
#include <core/os/sleep.hpp>
#include "chess.hpp"
#include "char.hpp"
#include <string>
#include <iostream>
#include <2d/rect_renderer.hpp>
#include <graphics/render_window.hpp>
#include <graphics/api/fence.hpp>
#include <graphics/api/gpu.hpp>
#include <graphics/api/command_buffer.hpp>

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

class Application {
private:
	RenderWindow m_Window{ 1080, 1080, "window", TextureFormat::Depth32};
	RectRenderer m_Renderer{ m_Window.FramebufferPass() };
public:
	Application() {
		m_Window.SetEventsHandler({ this, &Application::OnEvent });
	}

	void Run() {
		Semaphore acq, pst;
		Fence render;

		UniquePtr<CommandPool> cmd_pool(CommandPool::Create());
		UniquePtr<CommandBuffer, CommandBufferDeleter> cmd_buffer(cmd_pool->Alloc(), { cmd_pool.Get() });

		Clock cl;
		while (m_Window.IsOpen()) {
			Time dt = cl.Restart();

			Sleep(Max<Time>(Milliseconds(16) - dt , Milliseconds(0)));
			
			if (m_Window.IsFocused()) {

				const float cube_size = 1.f / 8.f;
				for (int i = 0; i < 8; i++)
					for (int j = 0; j < 8; j++)
						m_Renderer.DrawRect(Vector2f{ i * cube_size, j * cube_size } - Vector2f(0.5, 0.5), { cube_size, cube_size }, (i * 9 + j) % 2 == 0 ? Color::Black : Color::White);

				m_Window.AcquireNextFramebuffer(&acq);
				cmd_buffer->Begin();
				{
					cmd_buffer->ClearColor(m_Window.CurrentFramebuffer()->Attachments()[0], Color::LightBlue);
					ViewportParameters params;
					params.ViewportSize = { 1, 1 };
					m_Renderer.CmdRender(cmd_buffer.Get(), m_Window.CurrentFramebuffer(), params);
				}
				cmd_buffer->End();
				GPU::Execute(cmd_buffer.Get(), acq, pst, render);
				render.WaitAndReset();
				m_Window.PresentCurrentFramebuffer(&pst);
			}

			m_Window.DispatchEvents();
		}

		GPU::WaitIdle();
	}

	void OnEvent(const Event& e) {
		if (e.Type == EventType::WindowClose)
			m_Window.Close();
	}

};

int main() {

	GPU::ForceInit();
	Position pos('a', 5);
	SIPosition ai_pos(pos, FigureSide::White);
	SX_ASSERT(pos.X == ai_pos.ToPosition().X);
	SX_ASSERT(pos.Y == ai_pos.ToPosition().Y);
	Println("%", ai_pos.ToPosition());

	Application app;
	app.Run();
	Game game;
	game.Run();
}