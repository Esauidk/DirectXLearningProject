#include "TransformConstantBuffer.h"
#include "GraphicsMacros.h"

TransformConstantBuffer::TransformConstantBuffer(Graphics& gfx, const Drawable& parent) : parent(parent) {
	if (!transMatBuf) {
		transMatBuf = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(gfx);
	}
};

void TransformConstantBuffer::Bind(Graphics& gfx) noexcept {
	transMatBuf->Update(gfx,
		DirectX::XMMatrixTranspose(
			parent.GetTransformXM() * gfx.GetCamera() * gfx.GetProjection()
			)
		);
	transMatBuf->Bind(gfx);
}

std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> TransformConstantBuffer::transMatBuf;
