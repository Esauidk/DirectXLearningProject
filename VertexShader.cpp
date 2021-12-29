#include "VertexShader.h"
#include "GraphicsMacros.h"

VertexShader::VertexShader(Graphics& gfx, const std::wstring shaderPath) {
	INFOMAN(gfx);
	
	GFX_THROW_INFO(D3DReadFileToBlob(shaderPath.c_str(), &pBlob));
	GFX_THROW_INFO(GetDevice(gfx)->CreateVertexShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&pVertexShader
	));
}

void VertexShader::Bind(Graphics& gfx) noexcept {
	GetContext(gfx)->VSSetShader(pVertexShader.Get(), nullptr, 0u);
}

ID3DBlob* VertexShader::GetByteCode() const noexcept {
	return pBlob.Get();
}
