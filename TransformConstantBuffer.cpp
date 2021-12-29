#include "TransformConstantBuffer.h"
#include "GraphicsMacros.h"

TransformConstantBuffer::TransformConstantBuffer(Graphics& gfx, const Drawable& parent) : transMatBuf(gfx), parent(parent) {};

void TransformConstantBuffer::Bind(Graphics& gfx) noexcept {
	transMatBuf.Update(gfx,
			DirectX::XMMatrixTranspose(
				parent.GetTransformXM() * gfx.GetProjection()
			)
		);
	transMatBuf.Bind(gfx);
}