#pragma once
#include "Bindable.h"
#include "GraphicsMacros.h"

template<typename V>
class VertexBuffer : public Bindable
{
public:
	VertexBuffer(Graphics& gfx, const std::vector<V>& vertices) : objectStride(sizeof(V)) {
		INFOMAN(gfx);

		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = UINT(objectStride * vertices.size());
		bd.StructureByteStride = objectStride;
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertices.data();
		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&bd, &sd, &pVertexBuffer));
	}
	
	void Bind(Graphics& gfx) noexcept override {
		const UINT offset = 0u;
		GetContext(gfx)->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &objectStride, &offset);
	}

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	UINT objectStride;

};
