#include "Camera.h"

// Constructor/Destructor
Camera::Camera(float aspectRatio, float fov, DirectX::XMFLOAT3 initialPos)
{
    // Create transform and set position
    transform = std::make_shared<Transform>();
    transform->SetPosition(initialPos);
    this->fov = fov;

    // Setup stuff
    UpdateProjectionMatrix(aspectRatio);
    UpdateViewMatrix();
}

Camera::~Camera()
{
}

// Getters
DirectX::XMFLOAT4X4 Camera::GetViewMatrix()
{
    return viewMatrix;
}

DirectX::XMFLOAT4X4 Camera::GetProjMatrix()
{
    return projMatrix;
}

std::shared_ptr<Transform> Camera::GetTransform()
{
    return transform;
}

float Camera::GetFov()
{
    return fov;
}

// Methods
void Camera::UpdateProjectionMatrix(float aspectRatio)
{
    // Calculate projection matrix
    DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, nearClipDistance, farClipDistance);

    // Store it
    DirectX::XMStoreFloat4x4(&projMatrix, proj);
}

void Camera::UpdateViewMatrix()
{
    // Get transforms
    DirectX::XMFLOAT3 pos = transform->GetPosition();
    DirectX::XMFLOAT3 dir = transform->GetForward();

    // Calculate view matrix
    DirectX::XMMATRIX view = DirectX::XMMatrixLookToLH(DirectX::XMLoadFloat3(&pos), DirectX::XMLoadFloat3(&dir), DirectX::XMVectorSet(0, 1, 0, 0));

    // Store it
    DirectX::XMStoreFloat4x4(&viewMatrix, view);
}

void Camera::Update(float dt)
{
    float speed = dt * movementSpeed;

    // Handling key input
    if (Input::KeyDown('W')) { transform->MoveRelative(0, 0, speed); }
    if (Input::KeyDown('S')) { transform->MoveRelative(0, 0, -speed); }
    if (Input::KeyDown('A')) { transform->MoveRelative(-speed, 0, 0); }
    if (Input::KeyDown('D')) { transform->MoveRelative(speed, 0, 0); }
    if (Input::KeyDown(' ')) { transform->MoveAbsolute(0, speed, 0); }
    if (Input::KeyDown('X')) { transform->MoveAbsolute(0, -speed, 0); }

    // Handling mouse input
    if (Input::MouseLeftDown()) {
        float dx = Input::GetMouseXDelta() * mouseLookSpeed;
        float dy = Input::GetMouseYDelta() * mouseLookSpeed;

        // add clamp somewhere here

        transform->Rotate(dy, dx, 0);
    }

    // Update view matrix
    UpdateViewMatrix();
}
