#include "TransformConstantBuffer.h"
#include "GraphicsMacros.h"

TransformConstantBuffer::TransformConstantBuffer(Graphics& gfx, const Drawable& parent, UINT slot) : parent(parent) {
	if (!transMatBuf) {
		transMatBuf = std::make_unique<VertexConstantBuffer<Transforms>>(gfx, slot);
	}
};

void TransformConstantBuffer::Bind(Graphics& gfx) noexcept {
	const auto modelView = parent.GetTransformXM() * gfx.GetCamera();

	const Transforms tf = {
		DirectX::XMMatrixTranspose(modelView),
		DirectX::XMMatrixTranspose(
			modelView * gfx.GetProjection()
			)
	};

	transMatBuf->Update(gfx, tf);
	transMatBuf->Bind(gfx);
}

std::unique_ptr<VertexConstantBuffer<TransformConstantBuffer::Transforms>> TransformConstantBuffer::transMatBuf;
