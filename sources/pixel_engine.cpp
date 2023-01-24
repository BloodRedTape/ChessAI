#include "pixel_engine.hpp"
#include <core/os/clock.hpp>
#include <core/os/sleep.hpp>

PixelEngine::PixelEngine(const char *title, size_t width, size_t height, size_t pixel_size):
	m_Window(width * pixel_size, height * pixel_size, title, TextureFormat::Depth32),
	m_Width(width),
	m_Height(height),
	m_PixelSize(pixel_size)
{
	m_Window.SetEventsHandler({ this, &PixelEngine::OnEvent });
}

void PixelEngine::Run() {
	Semaphore acq, pst;
	Fence render;

	UniquePtr<CommandPool> cmd_pool(CommandPool::Create());
	UniquePtr<CommandBuffer, CommandBufferDeleter> cmd_buffer(cmd_pool->Alloc(), { cmd_pool.Get() });

	Clock cl;
	while (m_Window.IsOpen()) {
		Time dt = cl.Restart();

		Sleep(Max<Time>(Milliseconds(16) - dt, Milliseconds(0)));

		if (m_Window.IsFocused()) {
			OnUpdate(dt.AsSeconds());

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

void PixelEngine::DrawPixel(float x, float y, Color color, const Texture2D* texture) {
	const Vector2f pixel_size = Vector2f(1.f / float(m_Width), 1.f/ float(m_Height));
	m_Renderer.DrawRect(Vector2f(x, y) * pixel_size - Vector2f(0.5, 0.5), pixel_size, color, texture);
}

void PixelEngine::DrawPixel(float x, float y, Color color) {
	DrawPixel(x, y, color, Texture2D::White());
}

void PixelEngine::DrawPixel(float x, float y, const Texture2D* texture) {
	DrawPixel(x, y, Color::White, texture);
}

void PixelEngine::OnEvent(const Event& e) {
	if (e.Type == EventType::WindowClose)
		m_Window.Close();
}