#pragma once
#include "Bindable.h"
#include "Drawable.h"
#include "ConstantBuffer.h"
#include <DirectXMath.h>

class TransformConstantBuffer : public Bindable
{

private:
	struct Transforms {
		DirectX::XMMATRIX modelViewProj;
		DirectX::XMMATRIX model;
	};

public:
	// Creates a constant buffer for the transform matrix
	//
	// gfx: A reference to a Graphics instance
	// parent: The object/model this transform matrix applies to
	TransformConstantBuffer(Graphics& gfx, const Drawable& parent, UINT slot = 0u);
	void Bind(Graphics& gfx) noexcept override;
private:
	static std::unique_ptr<VertexConstantBuffer<Transforms>> transMatBuf;
	const Drawable& parent;
};

