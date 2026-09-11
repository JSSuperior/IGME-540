#include <d3d11.h>
#include <wrl/client.h>
#include <memory>
#include <vector>
#include <string>

#include "Vertex.h"
#include "Graphics.h"
#include "Mesh.h"

/// <summary>
/// Constructor for creating new meshes (also initializes/sets up buffers)
/// </summary>
/// <param name="vertices">Vertex array</param>
/// <param name="vertexSize">int</param>
/// <param name="indices">int array</param>
/// <param name="indexSize">int</param>
Mesh::Mesh(std::string name, Vertex vertices[], int vertexSize, int indices[], int indexSize)
{
	// Storing mesh name
	meshName = name;

	// Storing vertex and index count
	meshVertexCount = vertexSize;
	meshIndexCount = indexSize;

	// Creating vertex buffer
	{
		D3D11_BUFFER_DESC vbd {};
		vbd.Usage = D3D11_USAGE_IMMUTABLE;	// Will NEVER change
		vbd.ByteWidth = sizeof(Vertex) * vertexSize;       // 3 = number of vertices in the buffer
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Tells Direct3D this is a vertex buffer
		vbd.CPUAccessFlags = 0;	// Note: We cannot access the data from C++ (this is good)
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;

		// Create the proper struct to hold the initial vertex data
		// - This is how we initially fill the buffer with data
		// - Essentially, we're specifying a pointer to the data to copy
		D3D11_SUBRESOURCE_DATA initialVertexData {};
		initialVertexData.pSysMem = vertices; // pSysMem = Pointer to System Memory

		// Actually create the buffer on the GPU with the initial data
		// - Once we do this, we'll NEVER CHANGE DATA IN THE BUFFER AGAIN
		Graphics::Device->CreateBuffer(&vbd, &initialVertexData, vertexBuffer.GetAddressOf());
	}

	// Creating index buffer
	{
		D3D11_BUFFER_DESC ibd {};
		ibd.Usage = D3D11_USAGE_IMMUTABLE;	// Will NEVER change
		ibd.ByteWidth = sizeof(unsigned int) * indexSize;	// 3 = number of indices in the buffer
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;	// Tells Direct3D this is an index buffer
		ibd.CPUAccessFlags = 0;	// Note: We cannot access the data from C++ (this is good)
		ibd.MiscFlags = 0;
		ibd.StructureByteStride = 0;

		// Specify the initial data for this buffer, similar to above
		D3D11_SUBRESOURCE_DATA initialIndexData {};
		initialIndexData.pSysMem = indices; // pSysMem = Pointer to System Memory

		// Actually create the buffer with the initial data
		// - Once we do this, we'll NEVER CHANGE THE BUFFER AGAIN
		Graphics::Device->CreateBuffer(&ibd, &initialIndexData, indexBuffer.GetAddressOf());
	}
}

// Will be left empty b/c smart pointers
Mesh::~Mesh()
{
}

/// <summary>
/// Returns ComPtr for vertex buffer
/// </summary>
/// <returns>ComPtr</returns>
Microsoft::WRL::ComPtr<ID3D11Buffer> Mesh::GetVertexBuffer()
{
	return vertexBuffer;
}

/// <summary>
/// Returns ComPtr for index buffer
/// </summary>
/// <returns>ComPtr</returns>
Microsoft::WRL::ComPtr<ID3D11Buffer> Mesh::GetIndexBuffer()
{
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

/// <summary>
/// Returns stored name of mesh
/// </summary>
/// <returns>string</returns>
std::string Mesh::GetMeshName()
{
	return meshName;
}

/// <summary>
/// Draws mesh
/// </summary>
void Mesh::Draw()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	Graphics::Context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	Graphics::Context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	Graphics::Context->DrawIndexed(
		meshIndexCount,     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);    // Offset to add to each index when looking up vertices
}
