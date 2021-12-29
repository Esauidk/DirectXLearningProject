#pragma once

#include "Drawable.h"
class Box : public Drawable
{
public:
	Box(Graphics& gfx,
		std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist);
	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	float r;
	// z-axis
	float roll = 0.0f;
	// x-axis
	float pitch = 0.0f;
	// y-axis
	float yaw = 0.0f;
	float theta;
	float phi;
	float chi;

	// z-axis
	float droll;
	// x-axis
	float dpitch;
	// y-axis
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;
};
