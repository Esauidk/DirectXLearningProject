#include "Graphics.h"
#include <sstream>
#include <cmath>
#include "GraphicsMacros.h"
#include "imgui-1.88/imgui_impl_dx11.h"
#include "imgui-1.88/imgui_impl_win32.h"

namespace wrl = Microsoft::WRL;

namespace dx = DirectX;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")


Graphics::Graphics(HWND hWnd) {

	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.Flags = 0; 

	UINT swapCreateFlags = 0u;
#ifndef NDEBUG
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr;

	// Creates device with swap chain(front and back buffer) with render context
	GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		swapCreateFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext
	));

	wrl::ComPtr<ID3D11Resource> pBackBuffer;
	GFX_THROW_INFO(pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
	if (pBackBuffer != nullptr) {
		GFX_THROW_INFO(pDevice->CreateRenderTargetView(
			pBackBuffer.Get(),
			nullptr,
			&pTarget
		));
	}
	else {
		throw GFX_EXCEPT((E_INVALIDARG));
	}

	// Creates depth stencil state
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	wrl::ComPtr<ID3D11DepthStencilState> pDSState;
	GFX_THROW_INFO(pDevice->CreateDepthStencilState(&dsDesc, &pDSState));

	// Bind Depth State
	pContext->OMSetDepthStencilState(pDSState.Get(), 1u);

	// Create depth stencil texture
	wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = 800u;
	descDepth.Height = 600u;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	GFX_THROW_INFO(pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

	// Grab view of the depth stencil texture so we can add it to the pipeline
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;
	GFX_THROW_INFO(pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &pDSV));

	// Attach view to pipeline (specifically Output Merger)
	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDSV.Get());

	// configure viewport
	D3D11_VIEWPORT vp;
	vp.Width = 800.0f;
	vp.Height = 600.0f;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pContext->RSSetViewports(1u, &vp);

	ImGui_ImplDX11_Init(pDevice.Get(), pContext.Get());
}

void Graphics::EndFrame() {
	if (imguiEnabled) {
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	HRESULT hr;
#ifdef NDEBUG
	infoManager.Set();
#endif
	if (FAILED(hr = pSwap->Present(1u, 0u))) {
		if (hr == DXGI_ERROR_DEVICE_REMOVED) {
			throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
		}
		else {
			throw GFX_EXCEPT(hr);
		}
	}
}

void Graphics::BeginFrame(float red, float green, float blue) noexcept {
	if (imguiEnabled) {
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	const float color[] = { red, green, blue, 1.0f };
	pContext->ClearRenderTargetView(pTarget.Get(), color);
	pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Graphics::DrawIndexed(UINT count) noexcept(!IS_DEBUG) {

	HRESULT hr;

	//struct Vertex
	//{
	//	struct {
	//		float x;
	//		float y;
	//		float z;
	//	} pos;

	//	/*struct {
	//		unsigned char r;
	//		unsigned char g;
	//		unsigned char b;
	//		unsigned char a;
	//	} color;*/
	//	
	//};

	////Create Vertex Buffer (2d Triangle)
	//const Vertex vertices[] = {

	//	{-1.0f, -1.0f, -1.0f},
	//	{1.0f, -1.0f, -1.0f},
	//	{-1.0f, 1.0f, -1.0f},
	//	{1.0f, 1.0f, -1.0f},
	//	{-1.0f, -1.0f, 1.0f},
	//	{1.0f, -1.0f, 1.0f},
	//	{-1.0f, 1.0f, 1.0f},
	//	{1.0f, 1.0f, 1.0f}
	//
	//};



	//wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
	//D3D11_BUFFER_DESC bd = {};
	//bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//bd.Usage = D3D11_USAGE_DEFAULT;
	//bd.CPUAccessFlags = 0u;
	//bd.MiscFlags = 0u;
	//bd.ByteWidth = sizeof(vertices);
	//bd.StructureByteStride = sizeof(Vertex);
	//D3D11_SUBRESOURCE_DATA sd = {};
	//sd.pSysMem = vertices;
	//GFX_THROW_INFO(pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));

	//// Attach Vertex Buffer to Pipeline
	//const UINT stride = sizeof(Vertex);
	//const UINT offset = 0u;
	//pContext->IASetVertexBuffers(0u,1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

	//// Create Index Buffer (Used for reusing predfined vertices for multiple triangles)
	//const unsigned short indices[] = {
	//	0,2,1, 2,3,1,
	//	1,3,5, 3,7,5,
	//	2,6,3, 3,6,7,
	//	4,5,7, 4,7,6,
	//	0,4,2, 2,4,6,
	//	0,1,4, 1,5,4,
	//};

	//wrl::ComPtr<ID3D11Buffer> pIndexBuffer;
	//D3D11_BUFFER_DESC ibd = {};
	//ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//ibd.Usage = D3D11_USAGE_DEFAULT;
	//ibd.CPUAccessFlags = 0u;
	//ibd.MiscFlags = 0u;
	//ibd.ByteWidth = sizeof(indices);
	//ibd.StructureByteStride = sizeof(unsigned short);
	//D3D11_SUBRESOURCE_DATA isd = {};
	//isd.pSysMem = indices;
	//GFX_THROW_INFO(pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer));

	//// Attach Index Buffer to Pipeline
	//pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

	//// Create const buffer for transformation matrix (Do matrix transformation instead of moving sending a new vertex buffer)
	//struct ConstantBuffer {
	//	dx::XMMATRIX transform;
	//};

	//const ConstantBuffer cb = {
	//	{
	//		dx::XMMatrixTranspose(
	//			dx::XMMatrixRotationZ(angle) *
	//			dx::XMMatrixRotationX(angle) *
	//			dx::XMMatrixTranslation(x, 0.0f, z + 4.0f) *
	//			dx::XMMatrixPerspectiveLH(1.0f, 3.0f/ 4.0f, 0.5f, 10.0f)
	//		)
	//	}
	//};


	//wrl::ComPtr<ID3D11Buffer> pConstantBuffer;
	//D3D11_BUFFER_DESC cbd = {};
	//cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//cbd.Usage = D3D11_USAGE_DYNAMIC;
	//cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//cbd.MiscFlags = 0u;
	//cbd.ByteWidth = sizeof(cb);
	//cbd.StructureByteStride = 0u;
	//D3D11_SUBRESOURCE_DATA csd = {};
	//csd.pSysMem = &cb;
	//GFX_THROW_INFO(pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer));

	//// Attach const buffer to vertex shader
	//pContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());

	//// Create another constant buffer for tagging triangle indexes to colors (individual colors per cube face)
	//struct ConstantBuffer2 {
	//	struct {
	//		float r;
	//		float g;
	//		float b;
	//		float a;
	//	}face_colors[6];
	//};

	//const ConstantBuffer2 cb2 = {
	//	{
	//		{1.0f, 0.0f, 1.0f},
	//		{1.0f, 0.0f, 0.0f},
	//		{0.0f, 1.0f, 0.0f},
	//		{0.0f, 0.0f, 1.0f},
	//		{1.0f, 1.0f, 0.0f},
	//		{0.0f, 1.0f, 1.0f},
	//	}
	//};

	//wrl::ComPtr<ID3D11Buffer> pConstantBuffer2;
	//D3D11_BUFFER_DESC cbd2 = {};
	//cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//cbd2.Usage = D3D11_USAGE_DEFAULT;
	//cbd2.CPUAccessFlags = 0u;
	//cbd2.MiscFlags = 0u;
	//cbd2.ByteWidth = sizeof(cb2);
	//cbd2.StructureByteStride = 0u;
	//D3D11_SUBRESOURCE_DATA csd2 = {};
	//csd2.pSysMem = &cb2;
	//GFX_THROW_INFO(pDevice->CreateBuffer(&cbd2, &csd2, &pConstantBuffer2));

	//// Attach const buffer to vertex shader
	//pContext->PSSetConstantBuffers(0u, 1u, pConstantBuffer2.GetAddressOf());

	//// Create Pixel Shader
	//wrl::ComPtr<ID3D11PixelShader> pPixelShader;
	//wrl::ComPtr<ID3DBlob> pBlob;
	//GFX_THROW_INFO(D3DReadFileToBlob(L"PixelShader.cso", &pBlob));
	//GFX_THROW_INFO(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));

	//// Attach Pixel Shader
	//pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

	//// Create Vertex Shader
	//wrl::ComPtr<ID3D11VertexShader> pVertexShader;
	//GFX_THROW_INFO(D3DReadFileToBlob(L"VertexShader.cso", &pBlob));
	//GFX_THROW_INFO(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));

	//// Attach Vertex Shader to Pipeline
	//pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

	//// Input (vertex) layout (2d position only)
	//wrl::ComPtr<ID3D11InputLayout> pInputLayout;
	//const D3D11_INPUT_ELEMENT_DESC ied[] = {
	//	// Reads positions as 2 4 byte values
	//	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	//	// Reads color as 4 byte values and normalizes them from 0 - 1
	//	// {"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	//};

	//GFX_THROW_INFO(pDevice->CreateInputLayout(
	//	ied, (UINT)std::size(ied),
	//	pBlob->GetBufferPointer(),
	//	pBlob->GetBufferSize(),
	//	&pInputLayout
	//));

	//// Bind Input Layout
	//pContext->IASetInputLayout(pInputLayout.Get());
	
	// Bind Render Target
	// **COME BACK TO THIS IN CASE ERROR** 
	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDSV.Get());

	// Set primitive topology to triangle list (groups of verticies make multiple triangles)
	//pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	

	GFX_THROW_INFO_ONLY(pContext->DrawIndexed(count, 0u, 0u));
}

void Graphics::SetProjecton(DirectX::FXMMATRIX proj) noexcept {
	projection = proj;
}

DirectX::XMMATRIX Graphics::GetProjection() const noexcept {
	return projection;
}

void Graphics::SetCamera(DirectX::FXMMATRIX camOri) noexcept {
	camera = camOri;
}

DirectX::XMMATRIX Graphics::GetCamera() const noexcept {
	return camera;
}

void Graphics::EnableImgui() noexcept {
	imguiEnabled = true;
}

void Graphics::DisableImgui() noexcept {
	imguiEnabled = false;
}

bool Graphics::IsImguiEnabled() const noexcept {
	return imguiEnabled;
}


Graphics::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept : Exception(line,file), hr(hr) {
	for (const auto& m : infoMsgs) {
		info += m;
		info.push_back('\n');
	}

	if (!info.empty()) {
		info.pop_back();
	}
}

const char* Graphics::HrException::what() const noexcept { 
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl;
	if (!info.empty()) {
		oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	}
		oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

std::string Graphics::HrException::GetErrorInfo() const noexcept {
	return info;
}
const char* Graphics::HrException::GetType() const noexcept {
	return "Graphics HResult Exception";
}

HRESULT Graphics::HrException::GetErrorCode() const noexcept { return hr; }

std::string Graphics::HrException::GetErrorDescription() const noexcept { 
	return TranslateErrorCode(hr);
}
const char* Graphics::DeviceRemovedException::GetType() const noexcept { 
	return "Graphics Device Removed Exception";
}

std::string Graphics::Exception::TranslateErrorCode(HRESULT hr) noexcept {
	char* pMsgBuf = nullptr;
	return std::system_category().message(hr);
}

Graphics::InfoException::InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept : Exception(line, file) {
	for (const auto& m : infoMsgs) {
		info += m;
		info.push_back('\n');
	}

	if (!info.empty()) {
		info.pop_back();
	}
}

const char* Graphics::InfoException::what() const noexcept {
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
 	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::InfoException::GetType() const noexcept {
	return "Graphics Info Exception";
}

std::string Graphics::InfoException::GetErrorInfo() const noexcept {
	return info;
}
