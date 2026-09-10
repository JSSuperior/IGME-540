#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <memory>
#include <vector>

#include "Vertex.h"
#include "Graphics.h"

class Mesh {
public:
	// Constructor/Destructor
	// might need to wrap vector in smart pointer?
	// could also do it without vectors
	//Mesh(std::vector<Vertex> &verticices, std::vector<int> &indicies);
	Mesh(Vertex verticies[], const unsigned int vertexSize, int indicies[], const unsigned int indexSize);
	~Mesh(); // will be empty

	// Methods
	// Might need to adjust the reference?
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetVertexBuffer();
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetIndexBuffer();
	int GetVertexCount();
	int GetIndexCount();
	void Draw();

private:
	// Buffers
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

	// Counts
	unsigned int meshIndexCount;
	unsigned int meshVertexCount;
};