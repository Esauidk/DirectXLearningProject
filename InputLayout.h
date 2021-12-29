#pragma once
#include "Bindable.h"

class InputLayout : public Bindable
{
public:
	// Defines the Input Layout for the shaders
	//
	// gfx: A reference to a graphics instance
	// layout: a collection of descriptions that make up the input layout
	// pVertexShaderByteCode: The byte code of the vertex shader in which input layout is being defined
	InputLayout(Graphics& gfx,
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
		ID3DBlob* pVertexShaderByteCode);
	void Bind(Graphics& gfx) noexcept override;
private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
};

