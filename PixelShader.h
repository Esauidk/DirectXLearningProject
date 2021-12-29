#pragma once
#include "Bindable.h"

class PixelShader : public Bindable
{
public:
	// Reads the Pixel Shader data from disk
	// Creates the nessecary resources to prepare for being attached to the pipeline
	//
	// gfx: An reference to a graphics instance
	// shaderPath: A string path to the pixel shader to be read
	PixelShader(Graphics& gfx, const std::wstring shaderPath);
	void Bind(Graphics& gfx) noexcept override;
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
};

