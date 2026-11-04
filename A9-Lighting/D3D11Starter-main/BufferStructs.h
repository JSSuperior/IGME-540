#pragma once

#include <DirectXMath.h>
#include "Lights.h"

struct VertexShaderData {
	//DirectX::XMFLOAT4 colorTint;
	//DirectX::XMFLOAT3 offset;
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 projMatrix;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 worldInvTranspose;
};

struct PixelShaderData {
	Light light;
	DirectX::XMFLOAT4 colorTint;
	DirectX::XMFLOAT4 ambientColor;
	DirectX::XMFLOAT2 totalTime;
	DirectX::XMFLOAT2 scale;
	DirectX::XMFLOAT2 offset;
	DirectX::XMFLOAT2 padding;
	DirectX::XMFLOAT3 cameraPos;
};