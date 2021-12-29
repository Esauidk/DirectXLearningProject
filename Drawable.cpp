#include "Drawable.h"
#include "GraphicsMacros.h"
#include "IndexBuffer.h"
#include <assert.h>
#include <typeinfo>

void Drawable::Draw(Graphics& gfx) const noexcept(!IS_DEBUG) {
	for (auto& bind : binds) {
		bind->Bind(gfx);
	}

	gfx.DrawIndexed(pIndexBuffer->GetCount());
}

void Drawable::AddBind(std::unique_ptr<Bindable> bind) noexcept(!IS_DEBUG) {
	assert("Must only use AddIndexBuffer() to bind an index buffer" && typeid(*bind) != typeid(IndexBuffer));

	binds.push_back(std::move(bind));
}

void Drawable::AddIndexBuffer(std::unique_ptr<IndexBuffer> ibuf) noexcept {
	assert("Index Buffer has already been added" && pIndexBuffer == nullptr);
	pIndexBuffer = ibuf.get();
	binds.push_back(std::move(ibuf));
}