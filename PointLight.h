#pragma once
#include "Graphics.h"
#include "ConstantBuffer.h"
#include "SolidSphere.h"

class PointLight
{
public: 
	PointLight(Graphics& gfx, float radius = 0.5f);
	void SpawnControlWindow() noexcept;
	void Reset() noexcept;
	void Draw(Graphics& gfx) const noexcept(!IS_DEBUG);
	void Bind(Graphics& gfx, DirectX::FXMMATRIX view) const noexcept;
private:
	struct PointLightConstantBuf {
		alignas(16)DirectX::XMFLOAT3 pos;
		alignas(16)DirectX::XMFLOAT3 diffuseColor;
		alignas(16)DirectX::XMFLOAT3 ambientColor;
		float diffuseIntensity;
		float constAttenuation;
		float linearAuttenuation;
		float quadraticAuttenuation;
	};
private:
	PointLightConstantBuf cbData;
	mutable SolidSphere modelMesh;
	mutable PixelConstantBuffer<PointLightConstantBuf> cbuf;
};

