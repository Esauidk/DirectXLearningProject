#pragma once

#include "Graphics.h"
#include <DirectXMath.h>

class Bindable;
class IndexBuffer;

class Drawable {
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	// Returns the matrix that represents the transform of this current model
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
	// Binds all parts pipeline parts related to this model and draws the object
	//
	// gfx: A reference to a Graphics instance
	void Draw(Graphics& gfx) const noexcept(!IS_DEBUG);
	// Updates the information of the model in a form of time
	//
	// dt: The change of time since the last call
	virtual void Update(float dt) noexcept = 0;
	// Adds a bindable to this object's collection (I.E. attaching it to this object)
	//
	// bind: The bindable to be added to this object's collection
	// WARNING: Use AddIndexBuffer() to add an IndexBuffer
	// AddIndexBuffer() handles extra operations needed for using the idex buffer
	void AddBind(std::unique_ptr<Bindable> bind) noexcept(!IS_DEBUG);
	// Attachs an indexbuffer to this object
	//
	// ibuf: The index buffer for this object
	// WARNING: ONLY 1 call of AddIndexBuffer() is allowed
	void AddIndexBuffer(std::unique_ptr<class IndexBuffer> ibuf) noexcept;
	virtual ~Drawable() = default;
private:
	const IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> binds;
};
