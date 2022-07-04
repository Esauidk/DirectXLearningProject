#pragma once
#include "Bindable.h"
#include "GraphicsMacros.h"

// Type of C defines what kind of data is stored in the buffer
template<typename C> class ConstantBuffer : public Bindable {
public:
	// Updates the data in this particular buffer (OLD DATA WILL BE WIPED)
	// 
	// gfx: A reference to the graphics instance 
	// consts: The new data asked to be copied into the buffer
	// THE DATA IN CONSTS WILL BE COPIED

	void Update(Graphics& gfx, const C& consts) {
		INFOMAN(gfx);

		D3D11_MAPPED_SUBRESOURCE msr;
		GFX_THROW_INFO(GetContext(gfx)->Map(
			pConstantBuffer.Get(),
			0u,
			D3D11_MAP_WRITE_DISCARD,
			0u,
			&msr
		));
		memcpy(msr.pData, &consts, sizeof(consts));

		GetContext(gfx)->Unmap(pConstantBuffer.Get(), 0u);
	}

	// Constructs a buffer and inputs the given data into it
	//
	// gfx: A reference to a graphics instance
	// consts: The data asked to be copied into the buffer
	// THE DATA IN CONSTS WILL BE COPIED

	ConstantBuffer(Graphics& gfx, const C& consts, UINT slot = 0u) : slot(slot) {
		INFOMAN(gfx);

		D3D11_BUFFER_DESC cbd = {};
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(consts);
		cbd.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &consts;
		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&cbd, &csd, &pConstantBuffer));
	}

	// Constructs an *empty* buffer
	//
	// gfx: A reference to a graphics instance
	ConstantBuffer(Graphics& gfx, UINT slot = 0u) : slot(slot) {
		INFOMAN(gfx);

		D3D11_BUFFER_DESC cbd = {};
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(C);
		cbd.StructureByteStride = 0u;


		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&cbd, nullptr, &pConstantBuffer));
	}
protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
	UINT slot;
};

template<typename C> class VertexConstantBuffer : public ConstantBuffer<C> {
	using ConstantBuffer<C>::pConstantBuffer;
	using ConstantBuffer<C>::slot;
	using Bindable::GetContext;


public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind(Graphics& gfx) noexcept override {
		GetContext(gfx)->VSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
	}
};

template<typename C> class PixelConstantBuffer : public ConstantBuffer<C> {
	using ConstantBuffer<C>::pConstantBuffer;
	using ConstantBuffer<C>::slot;
	using Bindable::GetContext;

public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind(Graphics& gfx) noexcept override {
		GetContext(gfx)->PSSetConstantBuffers(slot, 1u, pConstantBuffer.GetAddressOf());
	}
};
