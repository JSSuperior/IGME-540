#include <d3d11.h>
#include <wrl/client.h>
#include <memory>
#include <vector>

#include "Vertex.h"
#include "Graphics.h"
#include "Mesh.h"

Mesh::Mesh(Vertex verticies[], const unsigned int vertexSize, int indicies[], const unsigned int indexSize)
{


	meshIndexCount = indexSize;
	meshVertexCount = vertexSize;
}

// Will be left empty b/c smart pointers
Mesh::~Mesh()
{
}

Microsoft::WRL::ComPtr<ID3D11Buffer> Mesh::GetVertexBuffer()
{
	// need to adjust to return address?
	return vertexBuffer;
}

Microsoft::WRL::ComPtr<ID3D11Buffer> Mesh::GetIndexBuffer()
{
	// need to adjust to return address?
	return indexBuffer;
}

/// <summary>
/// Returns vertex count of mesh
/// </summary>
/// <returns>int</returns>
int Mesh::GetVertexCount()
{
	return meshVertexCount;
}

/// <summary>
/// Returns index count of mesh
/// </summary>
/// <returns>int</returns>
int Mesh::GetIndexCount()
{
	return meshIndexCount;
}

void Mesh::Draw()
{
}


