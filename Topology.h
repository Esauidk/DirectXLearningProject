#pragma once
#include "Bindable.h"
class Topology : public Bindable
{
public:
	// Stores the given desired topology in the structure before binding
	//
	// top: The desired topology for drawing a model
	Topology(D3D11_PRIMITIVE_TOPOLOGY top);
	void Bind(Graphics& gfx) noexcept override;
private:
	D3D11_PRIMITIVE_TOPOLOGY curTop;
};

