#include "TransformConstantBuffer.h"
#include "GraphicsMacros.h"

TransformConstantBuffer::TransformConstantBuffer(Graphics& gfx, const Drawable& parent) : parent(parent) {
	if (!transMatBuf) {
		transMatBuf = std::make_unique<VertexConstantBuffer<Transforms>>(gfx);
	}
};

void TransformConstantBuffer::Bind(Graphics& gfx) noexcept {
	const auto model = parent.GetTransformXM();

	const Transforms tf = {
		DirectX::XMMatrixTranspose(model),
		DirectX::XMMatrixTranspose(
			model * gfx.GetCamera() * gfx.GetProjection()
			)
	};

	transMatBuf->Update(gfx, tf);
	transMatBuf->Bind(gfx);
}

std::unique_ptr<VertexConstantBuffer<TransformConstantBuffer::Transforms>> TransformConstantBuffer::transMatBuf;
