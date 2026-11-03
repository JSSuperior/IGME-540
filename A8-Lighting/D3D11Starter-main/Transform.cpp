#include "Transform.h"

// Constructor
Transform::Transform()
{
	// Initializing transformation data
	position = DirectX::XMFLOAT3(0, 0, 0);
	rotation = DirectX::XMFLOAT3(0, 0, 0);
	scale = DirectX::XMFLOAT3(1, 1, 1);

	// Initializing Matrices
	DirectX::XMStoreFloat4x4(&worldMatrix, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&worldInverseTransposeMatrix, DirectX::XMMatrixIdentity());
}

// Destructor
Transform::~Transform() 
{

}

// Getters
DirectX::XMFLOAT3 Transform::GetPosition()
{
	return position;
}

DirectX::XMFLOAT3 Transform::GetPitchYawRoll()
{
	return rotation;
}

DirectX::XMFLOAT3 Transform::GetScale()
{
	return scale;
}

DirectX::XMFLOAT4X4 Transform::GetWorldMatrix()
{
	// Creating all three matrices
	DirectX::XMMATRIX t = DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&position));
	DirectX::XMMATRIX r = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMLoadFloat3(&rotation));
	DirectX::XMMATRIX s = DirectX::XMMatrixScalingFromVector(DirectX::XMLoadFloat3(&scale));

	// Multiply matrices and store result
	DirectX::XMMATRIX world = s * r * t;
	DirectX::XMStoreFloat4x4(&worldMatrix, world);
	DirectX::XMStoreFloat4x4(&worldInverseTransposeMatrix, DirectX::XMMatrixInverse(0, DirectX::XMMatrixTranspose(world)));

	// return world matrix
	return worldMatrix;
}

DirectX::XMFLOAT4X4 Transform::GetWorldInverseTransposeMatrix()
{
	// not doing anything crazy here since leaving it for later
	return worldInverseTransposeMatrix;
}

DirectX::XMFLOAT3 Transform::GetRight()
{
	DirectX::XMFLOAT3 right;
	DirectX::XMStoreFloat3(&right, DirectX::XMVector3Rotate(DirectX::XMVectorSet(1, 0, 0, 0), DirectX::XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z)));
	return right;
}

DirectX::XMFLOAT3 Transform::GetUp()
{
	DirectX::XMFLOAT3 up;
	DirectX::XMStoreFloat3(&up, DirectX::XMVector3Rotate(DirectX::XMVectorSet(0, 1, 0, 0), DirectX::XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z)));
	return up;
}

DirectX::XMFLOAT3 Transform::GetForward()
{
	DirectX::XMFLOAT3 forward;
	DirectX::XMStoreFloat3(&forward, DirectX::XMVector3Rotate(DirectX::XMVectorSet(0, 0, 1, 0), DirectX::XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z)));
	return forward;
}

// Setters
void Transform::SetPosition(float x, float y, float z)
{
	position = DirectX::XMFLOAT3(x, y, z);
}

void Transform::SetPosition(DirectX::XMFLOAT3 _position)
{
	position = _position;
}

void Transform::SetRotation(float pitch, float yaw, float roll)
{
	rotation = DirectX::XMFLOAT3(pitch, yaw, roll);
}

void Transform::SetRotation(DirectX::XMFLOAT3 _rotation)
{
	rotation = _rotation;
}

void Transform::SetScale(float x, float y, float z)
{
	scale = DirectX::XMFLOAT3(x, y, z);
}

void Transform::SetScale(DirectX::XMFLOAT3 _scale)
{
	scale = _scale;
}

// Transformers
void Transform::MoveAbsolute(float x, float y, float z)
{
	DirectX::XMStoreFloat3(&position, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&position), DirectX::XMVectorSet(x, y, z, 0)));
}

void Transform::MoveAbsolute(DirectX::XMFLOAT3 offset)
{
	DirectX::XMStoreFloat3(&position, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&position), DirectX::XMLoadFloat3(&offset)));
}

void Transform::MoveRelative(float x, float y, float z)
{
	DirectX::XMVECTOR _offset = DirectX::XMVectorSet(x, y, z, 0);
	DirectX::XMVECTOR desiredDirection = DirectX::XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);

	DirectX::XMStoreFloat3(&position, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&position), DirectX::XMVector3Rotate(_offset, desiredDirection)));
}

void Transform::MoveRelative(DirectX::XMFLOAT3 offset)
{
	DirectX::XMVECTOR _offset = DirectX::XMLoadFloat3(&offset);
	DirectX::XMVECTOR desiredDirection = DirectX::XMQuaternionRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);

	DirectX::XMStoreFloat3(&position, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&position), DirectX::XMVector3Rotate(_offset, desiredDirection)));
}

void Transform::Rotate(float pitch, float yaw, float roll)
{
	DirectX::XMStoreFloat3(&rotation, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&rotation), DirectX::XMVectorSet(pitch, yaw, roll, 0)));
}

void Transform::Rotate(DirectX::XMFLOAT3 _rotation)
{
	DirectX::XMStoreFloat3(&rotation, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&rotation), DirectX::XMLoadFloat3(&_rotation)));
}

void Transform::Scale(float x, float y, float z)
{
	DirectX::XMStoreFloat3(&scale, DirectX::XMVectorMultiply(DirectX::XMLoadFloat3(&scale), DirectX::XMVectorSet(x, y, z, 0)));
}

void Transform::Scale(DirectX::XMFLOAT3 _scale)
{
	DirectX::XMStoreFloat3(&scale, DirectX::XMVectorMultiply(DirectX::XMLoadFloat3(&scale), DirectX::XMLoadFloat3(&_scale)));
}
