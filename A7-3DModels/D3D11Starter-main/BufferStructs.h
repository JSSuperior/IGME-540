#pragma once

#include <DirectXMath.h>

struct VertexShaderData {
	DirectX::XMFLOAT4 colorTint;
	//DirectX::XMFLOAT3 offset;
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 projMatrix;
	DirectX::XMFLOAT4X4 viewMatrix;
};