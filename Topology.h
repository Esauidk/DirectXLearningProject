#pragma once
#include "Bindable.h"
class Topology : public Bindable
{
public:
	Topology(D3D11_PRIMITIVE_TOPOLOGY top);
	void Bind(Graphics& gfx) noexcept override;
private:
	D3D11_PRIMITIVE_TOPOLOGY curTop;
};

