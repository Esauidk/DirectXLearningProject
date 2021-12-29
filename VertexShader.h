#pragma once
#include "Bindable.h"

class VertexShader : public Bindable
{
public:
	// Reads the Vertex Shader data from disk
	// Creates the nessecary resources to prepare for being attached to the pipeline
	//
	// gfx: An reference to a graphics instance
	// shaderPath: A string path to the vertex shader to be read
	VertexShader(Graphics& gfx, const std::wstring shaderPath);
	void Bind(Graphics& gfx) noexcept override;
	// Returns the address of the vertex shader raw data currently stored
	ID3DBlob* GetByteCode() const noexcept;
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
};

