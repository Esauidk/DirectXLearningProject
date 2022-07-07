#include "AssetTest.h"
#include "BindableBase.h"
#include "GraphicsMacros.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

AssetTest::AssetTest(Graphics& gfx, std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist,
	DirectX::XMFLOAT3 material,
	float scale) : TestObject(gfx, rng, adist, ddist, odist, rdist) {
	
	namespace dx = DirectX;

	if (!IsStaticInitialized()) {
		// Define the vertex details
		struct Vertex {
			dx::XMFLOAT3 pos;
			dx::XMFLOAT3 norm;
		};

		// Use Assimp to read and load in the model
		Assimp::Importer imp;
		const auto pModel = imp.ReadFile("models\\suzanne.obj", aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

		// Extract the mesh from the model(there could be more than 1 mesh)
		const auto pMesh = pModel->mMeshes[0];

		// Go through each Assimp vertices and take information to create our defined vertices
		std::vector<Vertex> vertices;
		// Allocate ahead of time as a performance save
		vertices.reserve(pMesh->mNumVertices);

		for (unsigned int i = 0; i < pMesh->mNumVertices; i++) {
			vertices.push_back({
				// Scale up vertices
				{pMesh->mVertices[i].x * scale, pMesh->mVertices[i].y * scale, pMesh->mVertices[i].z * scale},
				// Reinterpret straight to XMFLOAT3 since we don't need to apply scaling to them
				{*reinterpret_cast<dx::XMFLOAT3*>(&pMesh->mNormals[i])}
			});
		}

		// Go through each face of the triangles and grab the indicies
		std::vector<unsigned short> indices;
		// Since we are rendering triangles, each face will be 3 indicies
		indices.reserve(pMesh->mNumFaces * 3);
		for (unsigned int i = 0; i < pMesh->mNumFaces; i++) {
			const auto& face = pMesh->mFaces[i];

			assert(face.mNumIndices == 3);

			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		AddStaticBind(std::make_unique<VertexBuffer<Vertex>>(gfx, vertices));

		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

		auto pvs = std::make_unique<VertexShader>(gfx, L"PhongLightingVS.cso");
		auto pvsbc = pvs->GetByteCode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"PhongLightingPS.cso"));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};

		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		struct PSMaterialConstant
		{
			DirectX::XMFLOAT3 color = {0.0f, 0.0f, 0.0f};
			float specularIntensity = 0.6f;
			float specularPower = 30.0f;
			float padding[3];
		} pmc;

		pmc.color = material;
		AddStaticBind(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(gfx, pmc, 1u));
	}
	else {
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<TransformConstantBuffer>(gfx, *this));
}
