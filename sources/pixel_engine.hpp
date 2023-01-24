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

	void DrawPixel(float x, float y, Color color, const Texture2D* texture);

	void DrawPixel(float x, float y, Color color);

	void DrawPixel(float x, float y, const Texture2D* texture);

	virtual void OnEvent(const Event& e);

	const RenderWindow& Window()const {
		return m_Window;
	}

	Vector2u Size()const {
		return { (u32)m_Width, (u32)m_Height };
	}

	size_t PixelSize()const {
		return m_PixelSize;
	}

	Vector2f MouseToPixel(Vector2s mouse)const{
		return (Vector2f(mouse) / Vector2f(Window().Size())) * Vector2f(Size());
	}
};
