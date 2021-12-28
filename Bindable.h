#pragma once
#include "Graphics.h"

class Bindable
{
public:
	// Binds the class implementing this to the Direct3D pipeline
	//
	// gfx: A reference to a graphics instance
	virtual void Bind(Graphics& gfx) noexcept = 0;
	virtual ~Bindable() = default;
protected:
	// Getters for the *important* private fields of Graphics
	static ID3D11DeviceContext* GetContext(Graphics& gfx) noexcept;
	static ID3D11Device* GetDevice(Graphics& gfx) noexcept;
	static DxgiInfoManager& GetInfoManager(Graphics& gfx) noexcept(!IS_DEBUG);
};
