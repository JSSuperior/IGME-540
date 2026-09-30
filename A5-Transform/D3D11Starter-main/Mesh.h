#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <memory>
#include <vector>
#include <string>

#include "Vertex.h"
#include "Graphics.h"

class Mesh {
public:
	// Constructor/Destructor
	Mesh(std::string name, Vertex vertices[], int vertexSize, int indices[], int indexSize);
	~Mesh(); // will be empty

	// Getters
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetVertexBuffer();
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetIndexBuffer();
	int GetVertexCount();
	int GetIndexCount();
	std::string GetMeshName();

	// Methods
	void Draw();

private:
	// Buffers
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

	// Counts
	unsigned int meshIndexCount;
	unsigned int meshVertexCount;

	// Mesh name
	std::string meshName;
};