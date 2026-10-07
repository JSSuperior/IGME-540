#pragma once

#include <wrl/client.h>
#include <DirectXMath.h>
#include <memory>
#include "Input.h"
#include "Transform.h"

class Camera
{
public:
	// Constructor/Destructor
	Camera(float aspectRatio, float fov, DirectX::XMFLOAT3 initialPos); // keeping it simple for now
	~Camera();

	// Getters
	DirectX::XMFLOAT4X4 GetViewMatrix();
	DirectX::XMFLOAT4X4 GetProjMatrix();
	std::shared_ptr<Transform> GetTransform();
	float GetFov();

	// Methods
	void UpdateProjectionMatrix(float aspectRatio);
	void UpdateViewMatrix();
	void Update(float dt);

private:
	std::shared_ptr<Transform> transform;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projMatrix;
	float fov = 0.25f; // radians
	float nearClipDistance = 0.1f;
	float farClipDistance = 30;
	float movementSpeed = 3.0f;
	float mouseLookSpeed = 0.005f;
};

