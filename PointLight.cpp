#include "PointLight.h"
#include "imgui-1.88/imgui.h"

PointLight::PointLight(Graphics& gfx, float radius) : modelMesh(gfx, radius), cbuf(gfx) {
	Reset();
}

void PointLight::SpawnControlWindow() noexcept {
	if (ImGui::Begin("Light")) {
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &cbData.pos.x, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Y", &cbData.pos.y, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Z", &cbData.pos.z, -60.0f, 60.0f, "%.1f");

		ImGui::Text("Intensity/Color");
		ImGui::SliderFloat("Intesity", &cbData.diffuseIntensity, 0.01f, 2.0f, "%.2f");
		ImGui::ColorEdit3("Diffuse Color", &cbData.diffuseColor.x);
		ImGui::ColorEdit3("Ambient Color", &cbData.ambientColor.x);

		ImGui::Text("Light Falloff/Attenuation");
		ImGui::SliderFloat("Constant Falloff", &cbData.constAttenuation, 0.05f, 10.0f, "%.2f");
		ImGui::SliderFloat("Linear Falloff", &cbData.linearAuttenuation, 0.001f, 4.0f, "%.4f");
		ImGui::SliderFloat("Quadratic Falloff", &cbData.quadraticAuttenuation, 0.0000001f, 10.0f, "%.7f");

		if (ImGui::Button("Reset")) {
			Reset();
		}
	}
	ImGui::End();
}

void PointLight::Reset() noexcept {
	cbData = {
		{0.0f, 0.0f, 0.0f},
		{1.0f, 1.0f, 1.0f},
		{0.05f, 0.05f, 0.05f},
		1.0f,
		1.0f,
		0.045f,
		0.0075f
	};
}

void PointLight::Draw(Graphics& gfx) const noexcept(!IS_DEBUG) {
	modelMesh.SetPos(cbData.pos);
	modelMesh.Draw(gfx);
}

void PointLight::Bind(Graphics& gfx, DirectX::FXMMATRIX view) const noexcept {
	auto dataCopy = cbData;
	const auto pos = DirectX::XMLoadFloat3(&cbData.pos);
	DirectX::XMStoreFloat3(&dataCopy.pos, DirectX::XMVector3Transform(pos, view));
	cbuf.Update(gfx, dataCopy);
	cbuf.Bind(gfx);
}
