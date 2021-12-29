#include "Topology.h"

Topology::Topology(D3D11_PRIMITIVE_TOPOLOGY top) : curTop(top) {}

void Topology::Bind(Graphics& gfx) noexcept {
	GetContext(gfx)->IASetPrimitiveTopology(curTop);
}
