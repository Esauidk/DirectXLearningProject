#include "App.h"
#include "Box.h"
#include "SkinnedBox.h"
#include <memory>
#include "Sheet.h"
#include "Surface.h"
#include "GDIPlusManager.h"
#include "ChiliMath.h"
#include "imgui-1.88/imgui.h"

GDIPlusManager gdipm;

App::App():wnd(800, 600, "Direct X Test"), timer(){
	class Factory
	{
	public:
		Factory(Graphics& gfx)
			:
			gfx(gfx)
		{}
		std::unique_ptr<Drawable> operator()()
		{
			switch (typedist(rng))
			{
			case 0:
				return std::make_unique<Box>(
					gfx, rng, adist, ddist,
					odist, rdist, bdist
					);
			case 1:
				return std::make_unique<Sheet>(
					gfx, rng, adist, ddist,
					odist, rdist
					);
			case 2:
				return std::make_unique<SkinnedBox>(
					gfx, rng, adist, ddist,
					odist, rdist
					);
			default:
				assert(false && "bad drawable type in factory");
				return {};
			}
		}
	private:
		Graphics& gfx;
		std::mt19937 rng{ std::random_device{}() };
		std::uniform_real_distribution<float> adist{ 0.0f,PI * 2.0f };
		std::uniform_real_distribution<float> ddist{ 0.0f,PI * 0.5f };
		std::uniform_real_distribution<float> odist{ 0.0f,PI * 0.08f };
		std::uniform_real_distribution<float> rdist{ 6.0f,20.0f };
		std::uniform_real_distribution<float> bdist{ 0.4f,5.0f };
		std::uniform_int_distribution<int> latdist{ 5,20 };
		std::uniform_int_distribution<int> longdist{ 10,40 };
		std::uniform_int_distribution<int> typedist{ 0,2 };
	};
	

	Factory f(wnd.Gfx());
	drawables.reserve(nDrawables);
	std::generate_n(std::back_inserter(drawables), nDrawables, f);

	wnd.Gfx().SetProjecton(DirectX::XMMatrixPerspectiveLH(1.0, 3.0f / 4.0f, 0.5f, 40.0f));
}
App::~App() {}

int App::Go() {
	while (true) {
		if (const auto ecode = Window::ProcessMessages()) {
			return *ecode;
		}
		DoFrame();
	}

}

void App::DoFrame() {

	auto dt = timer.Mark() * speed_factor;

	
	wnd.Gfx().BeginFrame(0.07f, 0.0f, 0.12f);
	wnd.Gfx().SetCamera(cam.GetMatrix());
	for (auto& b : drawables) {
		b->Update(wnd.kbd.KeyIsPressed(VK_SPACE) ? 0.0f : dt);
		b->Draw(wnd.Gfx());
	}

	if (ImGui::Begin("Simulation Speed")) {
		ImGui::SliderFloat("Speed Factor", &speed_factor, 0.0f, 4.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::Text("Status %s", wnd.kbd.KeyIsPressed(VK_SPACE) ? "PAUSED" : "RUNNING (hold space to pause)");
	}
	ImGui::End();

	cam.SpawnControlWindow();

	wnd.Gfx().EndFrame();
}
