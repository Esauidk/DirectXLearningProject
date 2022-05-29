#include "Texture.h"
#include "Surface.h"
#include "GraphicsMacros.h"

namespace wrl = Microsoft::WRL;

Texture::Texture(Graphics& gfx, const Surface& s) {
	INFOMAN(gfx);

	// Allocate DirectX Texture Resource

	// Describes the texture data
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = s.GetWidth();
	textureDesc.Height = s.GetHeight();
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// Holds the texture data
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = s.GetBufferPtr();
	// Describing the distance between each data entry in our buffer (i.e. distance between chars, distance between ints, etc)
	sd.SysMemPitch = s.GetWidth() * sizeof(Surface::Color);

	// Pointer to DirectX texture structure
	wrl::ComPtr<ID3D11Texture2D> pTexture;
	GFX_THROW_INFO(GetDevice(gfx)->CreateTexture2D(&textureDesc, &sd, &pTexture));

	// NEED TO BIND TEXTURE VIEW TO SHADER, NOT THE ACTUAL TEXTURE

	D3D11_SHADER_RESOURCE_VIEW_DESC textViewDesc = {};
	textViewDesc.Format = textureDesc.Format;
	textViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	textViewDesc.Texture2D.MostDetailedMip = 0;
	textViewDesc.Texture2D.MipLevels = 1;
	GFX_THROW_INFO(GetDevice(gfx)->CreateShaderResourceView(pTexture.Get(), &textViewDesc, &pTextureView));

}

void Texture::Bind(Graphics& gfx) noexcept {
	GetContext(gfx)->PSSetShaderResources(0u, 1u, pTextureView.GetAddressOf());
}
