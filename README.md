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
**The framework is designed to shorten and abstract the creation of objects in DirectX**


*To be filled in*
## Graphics
## Bindable
## Drawable
