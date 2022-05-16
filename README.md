# DirectXLearningProject
A repo containing files involved with my exploration of DirectX after following along with PlanetChilli's tutorial with Direct3D

The Drawable & Bindable Graphics framework was designed by PlanetChilli (Otherwise known as ChiliTomatoNoodle on yt)


# Direct X Pipeline
**Before rendering an object in Direct X, there are components that need to be initialized**:
- Device
- Swap Chain
- Context


## Device
**The device is a component of Direct3D that allows you to allocate data structures and store information on the GPU about the current rendering state**


Examples of uses of the device are:
- RenderTargetView
- DepthStencilState
- CreateTexture2D
- CreateBuffer
- CreateVertexShader
- CreatePixelShader
- CreateInputLayout

[**For more information about the device**](https://docs.microsoft.com/en-us/windows/win32/direct3d9/direct3d-devices)

## Swap Chain
The swap chain represent the current frame being presented to you, the viewer. The swap chain has a lot of settings that can be adjusted but the idea is that you have two(or more) buffers, the **front** and **back** buffers. 

The names **front** and **back** represent which buffer is the one being presented. The **back** buffer will be where current draw calls are being directed to while the **front** buffer will be what the viewer sees. 

Once the drawing of the **back** buffer has been completed, the buffers will be *switched* and the previous **back** buffer will be the current **front** buffer while the previous **front** buffer will be the current **back** buffer

![A representation of a triple buffer swap chain](https://upload.wikimedia.org/wikipedia/en/thumb/d/df/Swap_chain_depiction.svg/1920px-Swap_chain_depiction.svg.png "A representation of a triple buffer swap chain")

[**For more information about the swap chain**](https://docs.microsoft.com/en-us/windows/win32/direct3d9/what-is-a-swap-chain-)

## Context
**The context holds all the resources being used in pipeline**:
- VertexBuffer
: A collection of vertex data of the current object
- IndexBuffer
: A collection of indexes representing the order of verticies to use when drawing the desired topology shape
- VerexShader
: GPU instructions used to perform mathmatical operations to each vertex positioning before presentation
- PixelShader
: GPU instructions used to perform mathmatical operations to each vertex coloring before presentation
- Topology
: The geometric shape to use when rendering verticies (EX: Lines & Triangles)
- InputLayout
: A definition of the low level layout of data being sent to the GPU and their shaders (Vertex Data Definition, Color Data Definition, etc)
- ConstantBuffer
: A buffer that is filled with custom data and available to the designated shader at all times. (Data can only be updated by replacing the buffer with an updated version)

**The context allows you to attach created data structures(listed above) and attach them to the current render pipeline**

[**For more information about the context**](https://docs.microsoft.com/en-us/windows/win32/direct3d11/overviews-direct3d-11-devices-intro)

## Extra

**There are more parts of the pipeline not covered in this readme**


![**Image of Direct3D Pipeline**](https://docs.microsoft.com/en-us/windows/win32/direct3d11/images/d3d11-pipeline-stages.jpg "A diagram of the pipeline order")


[**For more information of the pipeline**](https://docs.microsoft.com/en-us/windows/win32/direct3d11/overviews-direct3d-11-graphics-pipeline)



## Creating an Object
**Creating an object involves a combination of using the device to allocate data structures(like buffers) and using the context to attach them to the pipeline. Every object must go through creating, defining, and attaching pipeline resources before being raterized to the swap chain buffers**

### Example of creating a box

```cpp
struct Vertex
{
   struct {
	float x;
	float y;
	float z;
   } pos;		
};

////Create Vertex Buffer (2d Triangle)
const Vertex vertices[] = {

	{-1.0f, -1.0f, -1.0f},
	{1.0f, -1.0f, -1.0f},
	{-1.0f, 1.0f, -1.0f},
	{1.0f, 1.0f, -1.0f},
	{-1.0f, -1.0f, 1.0f},
	{1.0f, -1.0f, 1.0f},
	{-1.0f, 1.0f, 1.0f},
	{1.0f, 1.0f, 1.0f}

};



wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
D3D11_BUFFER_DESC bd = {};
bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
bd.Usage = D3D11_USAGE_DEFAULT;
bd.CPUAccessFlags = 0u;
bd.MiscFlags = 0u;
bd.ByteWidth = sizeof(vertices);
bd.StructureByteStride = sizeof(Vertex);
D3D11_SUBRESOURCE_DATA sd = {};
sd.pSysMem = vertices;
GFX_THROW_INFO(pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));

//// Attach Vertex Buffer to Pipeline
const UINT stride = sizeof(Vertex);
const UINT offset = 0u;
pContext->IASetVertexBuffers(0u,1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

//// Create Index Buffer (Used for reusing predfined vertices for multiple triangles)
const unsigned short indices[] = {
	0,2,1, 2,3,1,
	1,3,5, 3,7,5,
	2,6,3, 3,6,7,
	4,5,7, 4,7,6,
	0,4,2, 2,4,6,
	0,1,4, 1,5,4,
};

wrl::ComPtr<ID3D11Buffer> pIndexBuffer;
D3D11_BUFFER_DESC ibd = {};
ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
ibd.Usage = D3D11_USAGE_DEFAULT;
ibd.CPUAccessFlags = 0u;
ibd.MiscFlags = 0u;
ibd.ByteWidth = sizeof(indices);
ibd.StructureByteStride = sizeof(unsigned short);
D3D11_SUBRESOURCE_DATA isd = {};
isd.pSysMem = indices;
GFX_THROW_INFO(pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer));

//// Attach Index Buffer to Pipeline
pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

//// Create const buffer for transformation matrix (Do matrix transformation instead of moving sending a new vertex buffer)
struct ConstantBuffer {
	dx::XMMATRIX transform;
};

const ConstantBuffer cb = {
	{
		dx::XMMatrixTranspose(
			dx::XMMatrixRotationZ(angle) *
			dx::XMMatrixRotationX(angle) *
			dx::XMMatrixTranslation(x, 0.0f, z + 4.0f) *
			dx::XMMatrixPerspectiveLH(1.0f, 3.0f/ 4.0f, 0.5f, 10.0f)
		)
	}
};


wrl::ComPtr<ID3D11Buffer> pConstantBuffer;
D3D11_BUFFER_DESC cbd = {};
cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
cbd.Usage = D3D11_USAGE_DYNAMIC;
cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
cbd.MiscFlags = 0u;
cbd.ByteWidth = sizeof(cb);
cbd.StructureByteStride = 0u;
D3D11_SUBRESOURCE_DATA csd = {};
csd.pSysMem = &cb;
GFX_THROW_INFO(pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer));

//// Attach const buffer to vertex shader
pContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());

//// Create another constant buffer for tagging triangle indexes to colors (individual colors per cube face)
struct ConstantBuffer2 {
  struct {
	float r;
	float g;
	float b;
	float a;
  }face_colors[6];
};

const ConstantBuffer2 cb2 = {
   {
	{1.0f, 0.0f, 1.0f},
	{1.0f, 0.0f, 0.0f},
	{0.0f, 1.0f, 0.0f},
	{0.0f, 0.0f, 1.0f},
	{1.0f, 1.0f, 0.0f},
	{0.0f, 1.0f, 1.0f},
   }
};

wrl::ComPtr<ID3D11Buffer> pConstantBuffer2;
D3D11_BUFFER_DESC cbd2 = {};
cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
cbd2.Usage = D3D11_USAGE_DEFAULT;
cbd2.CPUAccessFlags = 0u;
cbd2.MiscFlags = 0u;
cbd2.ByteWidth = sizeof(cb2);
cbd2.StructureByteStride = 0u;
D3D11_SUBRESOURCE_DATA csd2 = {};
csd2.pSysMem = &cb2;
GFX_THROW_INFO(pDevice->CreateBuffer(&cbd2, &csd2, &pConstantBuffer2));

//// Attach const buffer to vertex shader
pContext->PSSetConstantBuffers(0u, 1u, pConstantBuffer2.GetAddressOf());

//// Create Pixel Shader
wrl::ComPtr<ID3D11PixelShader> pPixelShader;
wrl::ComPtr<ID3DBlob> pBlob;
GFX_THROW_INFO(D3DReadFileToBlob(L"PixelShader.cso", &pBlob));
GFX_THROW_INFO(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));

//// Attach Pixel Shader
pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

// Create Vertex Shader
wrl::ComPtr<ID3D11VertexShader> pVertexShader;
GFX_THROW_INFO(D3DReadFileToBlob(L"VertexShader.cso", &pBlob));
GFX_THROW_INFO(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));

// Attach Vertex Shader to Pipeline
//pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

//// Input (vertex) layout (2d position only)
wrl::ComPtr<ID3D11InputLayout> pInputLayout;
const D3D11_INPUT_ELEMENT_DESC ied[] = {
//	// Reads positions as 2 4 byte values
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
//	// Reads color as 4 byte values and normalizes them from 0 - 1
	 {"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

GFX_THROW_INFO(pDevice->CreateInputLayout(
	ied, (UINT)std::size(ied),
	pBlob->GetBufferPointer(),
	pBlob->GetBufferSize(),
	&pInputLayout
));

//// Bind Input Layout
pContext->IASetInputLayout(pInputLayout.Get());

// Bind Render Target
// **COME BACK TO THIS IN CASE ERROR** 
pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDSV.Get());

// Set primitive topology to triangle list (groups of verticies make multiple triangles)
pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);



GFX_THROW_INFO_ONLY(pContext->DrawIndexed(count, 0u, 0u));
```

### Vertex Buffer
**Device Step**
```cpp
struct Vertex
{
   struct {
	float x;
	float y;
	float z;
   } pos;		
};

////Create Vertex Buffer (2d Triangle)
const Vertex vertices[] = {

	{-1.0f, -1.0f, -1.0f},
	{1.0f, -1.0f, -1.0f},
	{-1.0f, 1.0f, -1.0f},
	{1.0f, 1.0f, -1.0f},
	{-1.0f, -1.0f, 1.0f},
	{1.0f, -1.0f, 1.0f},
	{-1.0f, 1.0f, 1.0f},
	{1.0f, 1.0f, 1.0f}

};



wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
D3D11_BUFFER_DESC bd = {};
bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
bd.Usage = D3D11_USAGE_DEFAULT;
bd.CPUAccessFlags = 0u;
bd.MiscFlags = 0u;
bd.ByteWidth = sizeof(vertices);
bd.StructureByteStride = sizeof(Vertex);
D3D11_SUBRESOURCE_DATA sd = {};
sd.pSysMem = vertices;
GFX_THROW_INFO(pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));
```
**Context Step**
```cpp
//// Attach Vertex Buffer to Pipeline
const UINT stride = sizeof(Vertex);
const UINT offset = 0u;
pContext->IASetVertexBuffers(0u,1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
```

### Index Buffer
**Device Step**
```cpp
//// Create Index Buffer (Used for reusing predfined vertices for multiple triangles)
const unsigned short indices[] = {
	0,2,1, 2,3,1,
	1,3,5, 3,7,5,
	2,6,3, 3,6,7,
	4,5,7, 4,7,6,
	0,4,2, 2,4,6,
	0,1,4, 1,5,4,
};

wrl::ComPtr<ID3D11Buffer> pIndexBuffer;
D3D11_BUFFER_DESC ibd = {};
ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
ibd.Usage = D3D11_USAGE_DEFAULT;
ibd.CPUAccessFlags = 0u;
ibd.MiscFlags = 0u;
ibd.ByteWidth = sizeof(indices);
ibd.StructureByteStride = sizeof(unsigned short);
D3D11_SUBRESOURCE_DATA isd = {};
isd.pSysMem = indices;
GFX_THROW_INFO(pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer));
```
**Context Step**
```cpp
//// Attach Index Buffer to Pipeline
pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
```
### Constant Buffers (Vertex and Pixel)
**Device Step**
```cpp
//// Create const buffer for transformation matrix (Do matrix transformation instead of moving sending a new vertex buffer)
struct ConstantBuffer {
	dx::XMMATRIX transform;
};

const ConstantBuffer cb = {
	{
		dx::XMMatrixTranspose(
			dx::XMMatrixRotationZ(angle) *
			dx::XMMatrixRotationX(angle) *
			dx::XMMatrixTranslation(x, 0.0f, z + 4.0f) *
			dx::XMMatrixPerspectiveLH(1.0f, 3.0f/ 4.0f, 0.5f, 10.0f)
		)
	}
};


wrl::ComPtr<ID3D11Buffer> pConstantBuffer;
D3D11_BUFFER_DESC cbd = {};
cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
cbd.Usage = D3D11_USAGE_DYNAMIC;
cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
cbd.MiscFlags = 0u;
cbd.ByteWidth = sizeof(cb);
cbd.StructureByteStride = 0u;
D3D11_SUBRESOURCE_DATA csd = {};
csd.pSysMem = &cb;
GFX_THROW_INFO(pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer));
```
**Context Step**
```cpp
//// Attach const buffer to vertex shader
pContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
```
**Device Step**
```cpp
//// Create another constant buffer for tagging triangle indexes to colors (individual colors per cube face)
struct ConstantBuffer2 {
  struct {
	float r;
	float g;
	float b;
	float a;
   }face_colors[6];
};

const ConstantBuffer2 cb2 = {
	{
		{1.0f, 0.0f, 1.0f},
		{1.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 1.0f},
		{1.0f, 1.0f, 0.0f},
		{0.0f, 1.0f, 1.0f},
	}
};

wrl::ComPtr<ID3D11Buffer> pConstantBuffer2;
D3D11_BUFFER_DESC cbd2 = {};
cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
cbd2.Usage = D3D11_USAGE_DEFAULT;
cbd2.CPUAccessFlags = 0u;
cbd2.MiscFlags = 0u;
cbd2.ByteWidth = sizeof(cb2);
cbd2.StructureByteStride = 0u;
D3D11_SUBRESOURCE_DATA csd2 = {};
csd2.pSysMem = &cb2;
GFX_THROW_INFO(pDevice->CreateBuffer(&cbd2, &csd2, &pConstantBuffer2));
```

**Context Step**
```cpp
//// Attach const buffer to vertex shader
pContext->PSSetConstantBuffers(0u, 1u, pConstantBuffer2.GetAddressOf());
```

### Input Layout
**Device Step**
```cpp
//// Input (vertex) layout (2d position only)
wrl::ComPtr<ID3D11InputLayout> pInputLayout;
const D3D11_INPUT_ELEMENT_DESC ied[] = {
//	// Reads positions as 2 4 byte values
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
//	// Reads color as 4 byte values and normalizes them from 0 - 1
	 {"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
};

GFX_THROW_INFO(pDevice->CreateInputLayout(
	ied, (UINT)std::size(ied),
	pBlob->GetBufferPointer(),
	pBlob->GetBufferSize(),
	&pInputLayout
));
```
**Context Step**
```cpp
//// Bind Input Layout
pContext->IASetInputLayout(pInputLayout.Get());
```
### Topology
**Context Step**
```cpp
// Set primitive topology to triangle list (groups of verticies make multiple triangles)
pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
```
# Drawable & Bindable Framework Design
**The framework is designed to shorten and abstract the creation of objects in DirectX. There are 3 parts of it, the Graphics interface, the Bindable Interface, and the Drawable Interface. These all work together to allow multiple object creation in a single 3D scene**

## Graphics
**This interface is the main controller of 3D scene**

### Pipeline setup
**This interface will handle the creation of the crucial components of the pipeline (Device, Swap Chain, Context)**

Along with that, it will keep a reference to:
- Device
- Swap Chain
- Context
- RenderTargetView
: A pipeline handled data structure for referencing the current back buffer
- DepthStencilView
: A pipeline handled data structure for referncing the current depth view frame
- A projection matrix
: Used for translating the vertex position to 3D camera space (perspective, orthographic)

There are also five key functions to the Graphics interface:
- `EndFrame()`
: Presents the current back buffer by switching the front and back buffer
- `ClearBuffer()`
: Wipes everything on the current back buffer
- `DrawIndex()`
: Draws onto the back buffer with the resources attached to the pipeline (assumes that indexing is done, can be changed to work without indexing)
- `SetProjection()`
: Updates the current projection matrix in the 3D Scene
- `GetProjection()`
: Returns the current project matrix being used in the 3D scene

## Bindable
**This interface represents one of the data structures that can be attached to the pipline**

Some examples of Bindable structures are:
- VertexBuffer
- ConstantBuffers
- IndexBuffer
- InputLayout
- Shaders (Vertex and Pixel)
- Topology
- etc

The only function required of a Bindable is called `Bind()`. The expected implementation is that the context step of attaching to the pipeline is done in this function while the device step is handled in the constructor.

### Helper Functions
**There are helper functions that only Bindable objects can call, these are implemented by the Bindable interface and allow Bindables to interact with the Graphics instance:**
- `GetContext()`
: Returns the current reference to the Context of a Graphics instance
- `GetDevice()`
: Returns the current reference to the Device of a Graphics instance

## Drawable
**This interface represents a drawable object such as a Box, Triangle, or any other 3D(or 2D) object**

There are 5 functions tied to a Drawable:
- `GetTransformXM()`
: Returns the current transformation matrix of the object (rotation + translation + etc, excluding the projection matrix stored in Graphics)
- `Draw()`
: Binds the Drawable's bindables to the pipeline and tells the Graphics instance to draw it
- `AddBind()`
: Adds a bindable to the Drawable's collection
- `AddIndexBuffer()`
: Performs the same as AddBind() except with the condition that only 1 IndexBuffer can be added to a Drawable object
- `Update()`
: Performs changes to a Drawable's state based off a change of time (movement, rotation, scaling, etc)

The Drawable interface already implements the crucial functions listed, new Drawables will only need to implement certain functions due to behavior varying between Drawables:
- `GetTransformXM()`
- `Update()`

The expected behavior is that creating Bindables and adding them to Drawables are done in a Drawable constructor

### Example of the Drawable & Bindable Framework
**Box.h**
```cpp
#include "Drawable.h"
class Box : public Drawable
{
public:
	Box(Graphics& gfx,
		std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist);
	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	float r;
	// z-axis
	float roll = 0.0f;
	// x-axis
	float pitch = 0.0f;
	// y-axis
	float yaw = 0.0f;
	float theta;
	float phi;
	float chi;

	// z-axis
	float droll;
	// x-axis
	float dpitch;
	// y-axis
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;
};
```
**Box.cpp (constructor)**
```cpp
Box::Box(Graphics& gfx,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist) :
	r(rdist(rng)),
	droll(ddist(rng)),
	dpitch(ddist(rng)),
	dyaw(ddist(rng)),
	dphi(odist(rng)),
	dtheta(odist(rng)),
	dchi(odist(rng)),
	chi(adist(rng)),
	theta(adist(rng)),
	phi(adist(rng))
{
	struct Vertex {
		struct {
			float x;
			float y;
			float z;
		}pos;
		struct {
			unsigned char r;
			unsigned char g;
			unsigned char b;
		}color;
	};

	const std::vector<Vertex> vertices =
	{
		{-1.0f, -1.0f, -1.0f, 255,0,255 },
		{1.0f, -1.0f, -1.0f, 255,0,0 },
		{-1.0f, 1.0f, -1.0f, 0,255,0},
		{1.0f, 1.0f, -1.0f,  0,0,255},
		{-1.0f, -1.0f, 1.0f, 255,255,0},
		{1.0f, -1.0f, 1.0f, 0,255,255},
		{-1.0f, 1.0f, 1.0f, 255,255,255},
		{1.0f, 1.0f, 1.0f, 0,255,255}
	};

	AddBind(std::make_unique<VertexBuffer<Vertex>>(gfx, vertices));

	auto pvs = std::make_unique<VertexShader>(gfx, L"VertexShader.cso");
	auto pvbc = pvs->GetByteCode();
	AddBind(std::move(pvs));

	AddBind(std::make_unique<PixelShader>(gfx, L"PixelShader.cso"));

	const std::vector<unsigned short> indices =
	{
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
	};

	AddIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

	struct FaceColorConstantBuffer {
		struct {
			float r;
			float g;
			float b;
			float a;
		}face_colors[6];
	};

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	AddBind(std::make_unique<InputLayout>(gfx, ied, pvbc));

	AddBind(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_unique<TransformConstantBuffer>(gfx, *this));
}
```
**Box.cpp (Update)**
```cpp
void Box::Update(float dt) noexcept {
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}
```
**Box.cpp (GetTransformXM)**
```cpp
DirectX::XMMATRIX Box::GetTransformXM() const noexcept {
	return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		DirectX::XMMatrixTranslation(r, 0.0f, 0.0f)*
		DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi)*
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f);

}
```

**Main**
```cpp
std::mt19937 rng(std::random_device{}());
std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 2.0f);
std::uniform_real_distribution<float> rdist(6.0f, 20.0f);

for (auto i = 0; i < 150; i++) {
	boxes.push_back(std::make_unique<Box>(
		(Graphics instance here),
		rng,
		adist,
		ddist,
		odist,
		rdist
	));
}

(Graphics instance here).SetProjecton(DirectX::XMMatrixPerspectiveLH(1.0, 3.0f / 4.0f, 0.5f, 40.0f));
```
**Some other function that executes a frame**
```cpp
auto dt = timer.Mark();
(Graphics instance here).ClearBuffer(0.07f, 0.0f, 0.12f);
for (auto& b : boxes) {
	b->Update(dt/8);
	b->Draw((Graphics instance here));
}
(Graphics instance here).EndFrame();
```

![Gif of Floating Boxes](https://media.giphy.com/media/hVsP8KfbcwItVhMg5s/giphy-downsized.gif)

[GIF Source Link](https://media.giphy.com/media/hVsP8KfbcwItVhMg5s/giphy-downsized.gif)
