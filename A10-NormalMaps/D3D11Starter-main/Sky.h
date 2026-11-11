#include "Mesh.h"
#include <d3d11.h>
#include <wrl/client.h>
#include <memory>
#include "Graphics.h"
#include "BufferStructs.h"
#include "Camera.h"
#include <WICTextureLoader.h>

#pragma once

using namespace Microsoft::WRL;

class Sky
{
public:
	// Constructor/Destructor
	Sky(ComPtr<ID3D11SamplerState> _samplerOptions, 
		ComPtr<ID3D11PixelShader> _pixelShader, 
		ComPtr<ID3D11VertexShader> _vertexShader, 
		std::shared_ptr<Mesh> _mesh,
		const wchar_t* right,
		const wchar_t* left,
		const wchar_t* up,
		const wchar_t* down,
		const wchar_t* front,
		const wchar_t* back);
	~Sky();

	// Misc methods
	void Draw(std::shared_ptr<Camera> _camera);
	ComPtr<ID3D11ShaderResourceView> CreateCubemap(
		const wchar_t* right,
		const wchar_t* left,
		const wchar_t* up,
		const wchar_t* down,
		const wchar_t* front,
		const wchar_t* back);

private:
	// Members
	ComPtr<ID3D11SamplerState> samplerOptions;
	ComPtr<ID3D11ShaderResourceView> cubeMapTexture;
	ComPtr<ID3D11DepthStencilState> depthStencilState;
	ComPtr<ID3D11RasterizerState> rasterizerOptions;
	ComPtr<ID3D11PixelShader> pixelShader;
	ComPtr<ID3D11VertexShader> vertexShader;
	std::shared_ptr<Mesh> mesh;
};