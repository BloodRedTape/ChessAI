#pragma once

#include <2d/rect_renderer.hpp>
#include <graphics/render_window.hpp>
#include <graphics/api/fence.hpp>
#include <graphics/api/gpu.hpp>
#include <graphics/api/command_buffer.hpp>

class PixelEngine {
private:
	RenderWindow m_Window;
	RectRenderer m_Renderer{ m_Window.FramebufferPass() };
	const size_t m_Width;
	const size_t m_Height;
	const size_t m_PixelSize;
public:
	PixelEngine(const char *title, size_t width, size_t height, size_t pixel_size);

	void Run();

	virtual void OnUpdate(float dt) = 0;

	void DrawRect(int x, int y, Color color, Texture2D* texture);

	void DrawRect(int x, int y, Color color);

	void DrawRect(int x, int y, Texture2D* texture);

	virtual void OnEvent(const Event& e);
};
