#include "Sampler.h"
#include "GraphicsMacros.h"

Sampler::Sampler(Graphics& gfx) {
	INFOMAN(gfx);

	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	GFX_THROW_INFO(GetDevice(gfx)->CreateSamplerState(&sampDesc, &pSampler));
}

void Sampler::Bind(Graphics& gfx) noexcept {
	GetContext(gfx)->PSSetSamplers(0u, 1u, pSampler.GetAddressOf());
}
