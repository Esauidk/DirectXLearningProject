#pragma once
#include "Bindable.h"

class IndexBuffer : public Bindable
{
public:
	// Constructs an index buffer with data given
	//
	// gfx: A reference to a graphics instance
	// indices: A collection of indices defining how the model is created with the given vertices
	IndexBuffer(Graphics& gfx, const std::vector<unsigned short>& indices);
	void Bind(Graphics& gfx) noexcept override;
	// A getter for the amount of indices in the buffer
	UINT GetCount() const noexcept;
private:
	UINT count;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
};

