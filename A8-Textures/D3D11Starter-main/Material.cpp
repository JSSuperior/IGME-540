#include "Material.h"

using namespace DirectX;
//using namespace std;

Material::Material(DirectX::XMFLOAT4 _colorTint, Microsoft::WRL::ComPtr<ID3D11VertexShader> _vertexShader, Microsoft::WRL::ComPtr<ID3D11PixelShader> _pixelShader)
{
	// Setting color tint
	colorTint = _colorTint;

	// Setting vertex shader
	vertexShader = _vertexShader;

	// Setting pixel shader
	pixelShader = _pixelShader;
}

Material::~Material()
{
}

DirectX::XMFLOAT4 Material::GetColorTint()
{
	return colorTint;
}

Microsoft::WRL::ComPtr<ID3D11VertexShader> Material::GetVertexShader()
{
	return vertexShader;
}

Microsoft::WRL::ComPtr<ID3D11PixelShader> Material::GetPixelShader()
{
	return pixelShader;
}

void Material::SetColorTint(float r, float g, float b, float a)
{
	colorTint = DirectX::XMFLOAT4(r, g, b, a);
}

void Material::SetColorTint(DirectX::XMFLOAT4 _colorTint)
{
	// Theres probably a more effecient way to do this
	colorTint = _colorTint;
}

void Material::SetVertexShader(Microsoft::WRL::ComPtr<ID3D11VertexShader> _vertexShader)
{
	vertexShader = _vertexShader;
}

void Material::SetPixelShader(Microsoft::WRL::ComPtr<ID3D11PixelShader> _pixelShader)
{
	pixelShader = _pixelShader;
}

void Material::AddTextureSRV(unsigned int slot, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv)
{
	textureSRVs[slot] = srv;
}

void Material::AddSampler(unsigned int slot, Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler)
{
	samplers[slot] = sampler;
}

void Material::BindTexturesAndSamplers()
{
	
}
