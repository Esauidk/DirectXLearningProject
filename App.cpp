#include "App.h"

App::App():wnd(800, 600, "Direct X Test"), timer(){}

int App::Go() {
	while (true) {
		if (const auto ecode = Window::ProcessMessages()) {
			return *ecode;
		}
		DoFrame();
	}

}

void App::DoFrame() {
	const float c = sin(timer.Peek()) / 2.0f + 0.5f;
	wnd.Gfx().ClearBuffer(c, c, 1.0f);
	wnd.Gfx().DrawTestTriangle(
		-timer.Peek(),
		0.0f,
		0.0f
	);
	wnd.Gfx().DrawTestTriangle(
		timer.Peek(), 
		wnd.mouse.GetPosX() / 400.0f - 1.0f, 
		-wnd.mouse.GetPosY() / 300.0f + 1.0f
	);
	wnd.Gfx().EndFrame();
}
