#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include <memory>
#include "Mesh.h"

class Game
{
public:
	// Basic OOP setup
	Game();
	~Game();
	Game(const Game&) = delete; // Remove copy constructor
	Game& operator=(const Game&) = delete; // Remove copy-assignment operator

	// Primary functions
	void Update(float deltaTime, float totalTime);
	void ImGuiUpdate(float deltaTime);
	void Draw(float deltaTime, float totalTime);
	void OnResize();

private:
	// Initialization helper methods - feel free to customize, combine, remove, etc.
	void LoadShaders();
	void CreateGeometry();

	// Note the usage of ComPtr below
	//  - This is a smart pointer for objects that abide by the
	//     Component Object Model, which DirectX objects do
	//  - More info here: https://github.com/Microsoft/DirectXTK/wiki/ComPtr

	// Buffers to hold actual geometry data
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

	// Vertex color buffer
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;

	// Shaders and shader-related constructs
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

	// Assignment Variables (not sure if putting them here is the best practice, its been a hot minute since I've done c++)
	//XMFLOAT4 color(0.0f, 0.0f, 0.0f, 0.0f);		// Color picker value
	bool showDemoWindow = false;				// Demo window toggle
	bool isChecked = false;						// Boolean for checkbox widget
	float dragFloatValue = 0.0;					// Drag float widget value
	float floatInputValue = 0.0;				// Float input widget value

	// Color and Vector
	float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float offset[3] = { 0.0f, 0.0f, 0.0f };

	// Mesh storage vector
	std::vector<std::shared_ptr<Mesh>> meshes;
};

