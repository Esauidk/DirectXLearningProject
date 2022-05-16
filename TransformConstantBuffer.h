#pragma once
#include "Bindable.h"
#include "Drawable.h"
#include "ConstantBuffer.h"
#include <DirectXMath.h>

class TransformConstantBuffer : public Bindable
{
public:
	// Creates a constant buffer for the transform matrix
	//
	// gfx: A reference to a Graphics instance
	// parent: The object/model this transform matrix applies to
	TransformConstantBuffer(Graphics& gfx, const Drawable& parent);
	void Bind(Graphics& gfx) noexcept override;
private:
	static std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> transMatBuf;
	const Drawable& parent;
};

