#pragma once
#include "DrawableBase.h"

template<class T>
class TestObject : public DrawableBase<T> {
public:
	TestObject(Graphics& gfx,
		std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist) :
		r(rdist(rng)),
		droll(ddist(rng)),
		dpitch(ddist(rng)),
		dyaw(ddist(rng)),
		dphi(odist(rng)),
		dtheta(odist(rng)),
		dchi(odist(rng)),
		chi(adist(rng)),
		theta(adist(rng)),
		phi(adist(rng))
	{}

	void Update(float dt) noexcept {
		roll += droll * dt;
		pitch += dpitch * dt;
		yaw += dyaw * dt;
		theta += dtheta * dt;
		phi += dphi * dt;
		chi += dchi * dt;
	}

	DirectX::XMMATRIX GetTransformXM() const noexcept {
		namespace dx = DirectX;
		return dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
			dx::XMMatrixTranslation(r, 0.0f, 0.0f) *
			dx::XMMatrixRotationRollPitchYaw(theta, phi, chi);
	}

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
