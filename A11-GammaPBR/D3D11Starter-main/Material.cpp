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

	// defaults for scale and offset
	scale = DirectX::XMFLOAT2(1,1);
	offset = DirectX::XMFLOAT2(0,0);
}

Material::~Material()
{
}

DirectX::XMFLOAT4 Material::GetColorTint()
{
	return colorTint;
}

DirectX::XMFLOAT2 Material::GetScale()
{
	return scale;
}

DirectX::XMFLOAT2 Material::GetOffset()
{
	return offset;
}

Microsoft::WRL::ComPtr<ID3D11VertexShader> Material::GetVertexShader()
{
	return vertexShader;
}

Microsoft::WRL::ComPtr<ID3D11PixelShader> Material::GetPixelShader()
{
	return pixelShader;
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>* Material::GetSRVs()
{
	return textureSRVs;
}

unsigned int Material::GetSRVsLength()
{
	return std::size(textureSRVs);
}

void Material::SetColorTint(float r, float g, float b, float a)
{
	colorTint = DirectX::XMFLOAT4(r, g, b, a);
}

void Material::SetColorTint(DirectX::XMFLOAT4 _colorTint)
{
	colorTint = _colorTint;
}

void Material::SetScale(float x, float y)
{
	scale = DirectX::XMFLOAT2(x, y);
}

void Material::SetScale(DirectX::XMFLOAT2 _scale)
{
	scale = _scale;
}

void Material::SetOffset(float x, float y)
{
	offset = DirectX::XMFLOAT2(x, y);
}

void Material::SetOffset(DirectX::XMFLOAT2 _offset)
{
	offset = _offset;
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
	// Binding textures
	for (unsigned int i = 0; i < std::size(textureSRVs); i++) {
		Graphics::Context->PSSetShaderResources(i, 1, textureSRVs[i].GetAddressOf());
	}

	// Binding samplers
	for (unsigned int i = 0; i < std::size(samplers); i++) {
		Graphics::Context->PSSetSamplers(i, 1, samplers[i].GetAddressOf());
	}
}
