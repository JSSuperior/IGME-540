#pragma once

#include <DirectXMath.h>
#include <d3d11.h>
#include <wrl/client.h>
#include <memory>
#include "Graphics.h"

class Material
{
public:
	// Constructor/Destructor
	Material(DirectX::XMFLOAT4 _colorTint, Microsoft::WRL::ComPtr<ID3D11VertexShader> _vertexShader, Microsoft::WRL::ComPtr<ID3D11PixelShader> _pixelShader);
	~Material();

	// Getters
	// might need to revisit later
	DirectX::XMFLOAT4 GetColorTint();
	DirectX::XMFLOAT2 GetScale();
	DirectX::XMFLOAT2 GetOffset();
	Microsoft::WRL::ComPtr<ID3D11VertexShader> GetVertexShader();
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetPixelShader();
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>* GetSRVs();
	unsigned int GetSRVsLength();

	// Setters
	void SetColorTint(float r, float g, float b, float a);
	void SetColorTint(DirectX::XMFLOAT4 _colorTint);
	void SetScale(float x, float y);
	void SetScale(DirectX::XMFLOAT2 _scale);
	void SetOffset(float x, float y);
	void SetOffset(DirectX::XMFLOAT2 _offset);
	void SetVertexShader(Microsoft::WRL::ComPtr<ID3D11VertexShader> _vertexShader);
	void SetPixelShader(Microsoft::WRL::ComPtr<ID3D11PixelShader> _pixelShader);
	void AddTextureSRV(unsigned int slot, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv);
	void AddSampler(unsigned int slot, Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler);
	void BindTexturesAndSamplers();
private:
	// Members
	DirectX::XMFLOAT4 colorTint;
	DirectX::XMFLOAT2 scale;
	DirectX::XMFLOAT2 offset;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureSRVs[128];
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplers[16];
};

