#include "Game.h"
#include "Graphics.h"
#include "Vertex.h"
#include "Input.h"
#include "PathHelpers.h"
#include "Window.h"
#include "Mesh.h"
#include "BufferStructs.h"

#include <DirectXMath.h>
#include <vector>
#include <string>

// This code assumes files are in "ImGui" subfolder!
// Adjust as necessary for your own folder structure and project setup
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

// Needed for a helper function to load pre-compiled shader files
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// The constructor is called after the window and graphics API
// are initialized but before the game loop begins
// --------------------------------------------------------
Game::Game()
{
	// Create geometry
	CreateGeometry();

	// Initialize ImGui itself & platform/renderer backends
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(Window::Handle());
	ImGui_ImplDX11_Init(Graphics::Device.Get(), Graphics::Context.Get());

	// Pick a style (uncomment one of these 3)
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();
	//ImGui::StyleColorsClassic()

	// Set initial graphics API state
	//  - These settings persist until we change them
	//  - Some of these, like the primitive topology & input layout, probably won't change
	//  - Others, like setting shaders, will need to be moved elsewhere later
	{
		// Tell the input assembler (IA) stage of the pipeline what kind of
		// geometric primitives (points, lines or triangles) we want to draw.  
		// Essentially: "What kind of shape should the GPU draw with our vertices?"
		Graphics::Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Ensure the pipeline knows how to interpret all the numbers stored in
		// the vertex buffer. For this course, all of your vertices will probably
		// have the same layout, so we can just set this once at startup.
		Graphics::Context->IASetInputLayout(inputLayout.Get());
	}

	// Creating buffer size
	int size = sizeof(VertexShaderData);
	size = (size + 15) / 16 * 16;

	// Defining buffer settings
	D3D11_BUFFER_DESC cbDesc{};
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.ByteWidth = size;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	// Actually creating buffer
	Graphics::Device->CreateBuffer(&cbDesc, 0, constantBuffer.GetAddressOf());

	// Binding constant buffer to pipeline stage
	Graphics::Context->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());

	// Setup cameras
	cameras.push_back(std::make_shared<Camera>(Window::AspectRatio(), 0.25f, DirectX::XMFLOAT3(-0.5,0,-1)));
	cameras.push_back(std::make_shared<Camera>(Window::AspectRatio(), 0.50f, DirectX::XMFLOAT3(0.5, 0, -1)));
	activeCameraNum = 0;
}


// --------------------------------------------------------
// Clean up memory or objects created by this class
// 
// Note: Using smart pointers means there probably won't
//       be much to manually clean up here!
// --------------------------------------------------------
Game::~Game()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

// --------------------------------------------------------
// Creates the geometry we're going to draw
// --------------------------------------------------------
void Game::CreateGeometry()
{
	// Create input layout for vertices
	ID3DBlob* vertexShaderBlob;
	D3DReadFileToBlob(FixPath(L"VertexShader.cso").c_str(), &vertexShaderBlob);

	D3D11_INPUT_ELEMENT_DESC inputElements[3] = {};

	// Set up the first element - a position, which is 3 float values
	inputElements[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;				// Most formats are described as color channels; really it just means "Three 32-bit floats"
	inputElements[0].SemanticName = "POSITION";							// This is "POSITION" - needs to match the semantics in our vertex shader input!
	inputElements[0].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;	// How far into the vertex is this?  Assume it's after the previous element

	// Second element, uv
	inputElements[1].Format = DXGI_FORMAT_R32G32_FLOAT;			
	inputElements[1].SemanticName = "TEXCOORD";							
	inputElements[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	// Third element, normals
	inputElements[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElements[2].SemanticName = "NORMAL";							
	inputElements[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	// Create the input layout, verifying our description against actual shader code
	Graphics::Device->CreateInputLayout(
		inputElements,							// An array of descriptions
		3,										// How many elements in that array?
		vertexShaderBlob->GetBufferPointer(),	// Pointer to the code of a shader that uses this layout
		vertexShaderBlob->GetBufferSize(),		// Size of the shader code that uses this layout
		inputLayout.GetAddressOf());			// Address of the resulting ID3D11InputLayout pointer

	// Loading shaders
	Microsoft::WRL::ComPtr<ID3D11VertexShader> shader1 = LoadVertexShader(L"VertexShader.cso");
	Microsoft::WRL::ComPtr<ID3D11PixelShader> shader2 = LoadPixelShader(L"PixelShader.cso");

	// Creating materials
	std::shared_ptr<Material> material1 = std::make_shared<Material>(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), shader1, shader2);
	std::shared_ptr<Material> material2 = std::make_shared<Material>(DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), shader1, shader2);
	std::shared_ptr<Material> material3 = std::make_shared<Material>(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), shader1, shader2);

	// Create meshes from obj files
	std::shared_ptr<Mesh> sphere = std::make_shared<Mesh>("sphere", FixPath("../../Assets/Meshes/sphere.obj").c_str());
	std::shared_ptr<Mesh> cube = std::make_shared<Mesh>("cube", FixPath("../../Assets/Meshes/cube.obj").c_str());
	std::shared_ptr<Mesh> cylinder = std::make_shared<Mesh>("cylinder", FixPath("../../Assets/Meshes/cylinder.obj").c_str());
	std::shared_ptr<Mesh> helix = std::make_shared<Mesh>("helix", FixPath("../../Assets/Meshes/helix.obj").c_str());
	std::shared_ptr<Mesh> quad = std::make_shared<Mesh>("quad", FixPath("../../Assets/Meshes/quad.obj").c_str());
	std::shared_ptr<Mesh> quad_double_sided = std::make_shared<Mesh>("double sided quad", FixPath("../../Assets/Meshes/quad_double_sided.obj").c_str());
	std::shared_ptr<Mesh> torus = std::make_shared<Mesh>("torus", FixPath("../../Assets/Meshes/torus.obj").c_str());

	// Adding meshes to mesh vector
	meshes.push_back(sphere);
	meshes.push_back(cube);
	meshes.push_back(cylinder);
	meshes.push_back(helix);
	meshes.push_back(quad);
	meshes.push_back(quad_double_sided);
	meshes.push_back(torus);

	// adding game entities
	gameEntities.push_back(std::make_shared<GameEntity>(sphere, material1));
	gameEntities.push_back(std::make_shared<GameEntity>(cube, material2));
	gameEntities.push_back(std::make_shared<GameEntity>(cylinder, material3));
	gameEntities.push_back(std::make_shared<GameEntity>(helix, material1));
	gameEntities.push_back(std::make_shared<GameEntity>(quad, material2));
	gameEntities.push_back(std::make_shared<GameEntity>(quad_double_sided, material3));
	gameEntities.push_back(std::make_shared<GameEntity>(torus, material1));

	// Spreading out game entities to see
	gameEntities[0]->GetTransform()->MoveAbsolute(-9, 0, 0);
	gameEntities[1]->GetTransform()->MoveAbsolute(-6, 0, 0);
	gameEntities[2]->GetTransform()->MoveAbsolute(-3, 0, 0);
	gameEntities[3]->GetTransform()->MoveAbsolute(0, 0, 0);
	gameEntities[4]->GetTransform()->MoveAbsolute(3, 0, 0);
	gameEntities[5]->GetTransform()->MoveAbsolute(6, 0, 0);
	gameEntities[6]->GetTransform()->MoveAbsolute(9, 0, 0);
}


// --------------------------------------------------------
// Handle resizing to match the new window size
//  - Eventually, we'll want to update our 3D camera
// --------------------------------------------------------
void Game::OnResize()
{
	for (std::shared_ptr<Camera> camera : cameras) {
		if (camera) {
			camera->UpdateProjectionMatrix(Window::AspectRatio());
		}
	}
}


Microsoft::WRL::ComPtr<ID3D11VertexShader> Game::LoadVertexShader(const std::wstring filePath)
{
	// Create vertex shader smart pointer being returned
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;

	// Load the vertex shader file and store it (using custom path)
	ID3DBlob* vertexShaderBlob;
	D3DReadFileToBlob(FixPath(filePath).c_str(), &vertexShaderBlob);
	Graphics::Device->CreateVertexShader(
		vertexShaderBlob->GetBufferPointer(),	// Get a pointer to the blob's contents
		vertexShaderBlob->GetBufferSize(),		// How big is that data?
		0,										// No classes in this shader
		vertexShader.GetAddressOf());			// The address of the ID3D11VertexShader pointer

	// Return new vertex shader
	return vertexShader;
}

Microsoft::WRL::ComPtr<ID3D11PixelShader> Game::LoadPixelShader(const std::wstring filePath)
{
	// Create pixel shader smart pointer being returned
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;

	// Load the pixel shader file and store it (using custom path)
	ID3DBlob* pixelShaderBlob;
	D3DReadFileToBlob(FixPath(filePath).c_str(), &pixelShaderBlob);
	Graphics::Device->CreatePixelShader(
		pixelShaderBlob->GetBufferPointer(),	// Pointer to blob's contents
		pixelShaderBlob->GetBufferSize(),		// How big is that data?
		0,										// No classes in this shader
		pixelShader.GetAddressOf());			// Address of the ID3D11PixelShader pointer

	// Return new pixel shader
	return pixelShader;
}


// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	Game::ImGuiUpdate(deltaTime);

	//gameEntities[0]->GetTransform()->SetPosition(sinf(totalTime), 0, 0);
	//gameEntities[1]->GetTransform()->SetRotation(0, 0, totalTime);
	//gameEntities[2]->GetTransform()->SetScale(sinf(totalTime),sinf(totalTime),0);

	cameras[activeCameraNum]->Update(deltaTime);

	// Example input checking: Quit if the escape key is pressed
	if (Input::KeyDown(VK_ESCAPE))
		Window::Quit();
}

void Game::ImGuiUpdate(float deltaTime)
{
	// Put this all in a helper method that is called from Game::Update()
	// Feed fresh data to ImGui
	ImGuiIO & io = ImGui::GetIO();
	io.DeltaTime = deltaTime;
	io.DisplaySize.x = (float)Window::Width();
	io.DisplaySize.y = (float)Window::Height();

	// Reset the frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// Determine new input capture
	Input::SetKeyboardCapture(io.WantCaptureKeyboard);
	Input::SetMouseCapture(io.WantCaptureMouse);

	// Show the demo window
	//ImGui::ShowDemoWindow();

	// Create custom ImGui window
	ImGui::Begin("My Cool Window");

	// Assignment 2 UI components
	if(ImGui::CollapsingHeader("Window Info/Misc")) {
		// Framerate & Window res
		ImGui::Text("Framerate: %f fps", ImGui::GetIO().Framerate);
		ImGui::Text("Window Resolution: %dx%d", Window::Width(), Window::Height());

		// RGBA color picker
		//ImGui::ColorEdit4("RGBA color editor", &color.x);

		// Toggle demo window button
		if (ImGui::Button("Show Demo Window")) {
			showDemoWindow = !showDemoWindow;
		}

		// Custom widgets
		ImGui::Checkbox("My cool checkbox", &isChecked);
		ImGui::DragFloat("My cool dragfloat", &dragFloatValue);
		ImGui::InputFloat("My cool float input", &floatInputValue);

	}

	// Assignment 3 UI components
	if (ImGui::CollapsingHeader("Mesh Info")) {
		for (std::shared_ptr<Mesh> mesh : meshes) { 
			// Had some help from here converting strings to char*
			// https://stackoverflow.com/questions/7352099/stdstring-to-char
			ImGui::Text(mesh->GetMeshName().c_str());
			ImGui::BulletText("Vertices: %i", mesh->GetVertexCount());
			ImGui::BulletText("Indices: %i", mesh->GetIndexCount());
			ImGui::BulletText("Triangles: %i", mesh->GetIndexCount() / 3);
		}
	}

	// Assignment 4 UI components
	if (ImGui::CollapsingHeader("Mesh Values")) {
		//ImGui::DragFloat3("Offset", offset, 0.1f);
		ImGui::ColorEdit4("Color Tint", color);
	}

	// I feel like there has to be a better way to do this, but I can't think of one atm
	// unsure if it warrant creating its own variables in header
	if (ImGui::CollapsingHeader("Game Entities")) {
		for (int i = 0; i < gameEntities.size(); i++) {
			ImGui::Text("Game Entity: %i", i);
			ImGui::PushID(i);

			// Get game entity transform
			std::shared_ptr<Transform> transform = gameEntities[i]->GetTransform();
			
			// Convert and store temporarily as arrays
			float position[3] = { transform->GetPosition().x, transform->GetPosition().y, transform->GetPosition().z};
			float rotation[3] = { transform->GetPitchYawRoll().x, transform->GetPitchYawRoll().y, transform->GetPitchYawRoll().z };
			float scale[3] = { transform->GetScale().x, transform->GetScale().y, transform->GetScale().z};

			// Sliders
			ImGui::DragFloat3("Position", position, 0.1f);
			ImGui::DragFloat3("Rotation (Radians)", rotation, 0.1f);
			ImGui::DragFloat3("Scale", scale, 0.1f);

			// Update transform values
			transform->SetPosition(DirectX::XMFLOAT3(position));
			transform->SetRotation(DirectX::XMFLOAT3(rotation));
			transform->SetScale(DirectX::XMFLOAT3(scale));
			ImGui::PopID();
		}
	}

	if (ImGui::CollapsingHeader("Camera Stuff")) {
		// Active Camera Info
		ImGui::Text("Active Camera: %i", activeCameraNum);
		XMFLOAT3 cameraPos = cameras[activeCameraNum]->GetTransform()->GetPosition();
		ImGui::Text("Camera position: (x: %f, y: %f, z: %f)", cameraPos.x, cameraPos.y, cameraPos.z);
		ImGui::Text("Camera fov: %f", cameras[activeCameraNum]->GetFov());

		// Spacing
		ImGui::Dummy(ImVec2(0.0f, 20.0f));

		// Create list of selectable cameras
		for (int i = 0; i < cameras.size(); i++) {
			ImGui::PushID(i);

			ImGui::Text("Camera %i", i);
			ImGui::SameLine();
			if (ImGui::Button("Set active camera")) {
				activeCameraNum = i;
			}

			ImGui::PopID();
		}
	}
	

	ImGui::End();

	// Show demo window if toggled
	if (showDemoWindow == true) {
		ImGui::ShowDemoWindow();
	}
} 

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Frame START
	// - These things should happen ONCE PER FRAME
	// - At the beginning of Game::Draw() before drawing *anything*
	{
		// Clear the back buffer (erase what's on screen) and depth buffer
		const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };
		Graphics::Context->ClearRenderTargetView(Graphics::BackBufferRTV.Get(),	color);
		Graphics::Context->ClearDepthStencilView(Graphics::DepthBufferDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	//for (std::shared_ptr<Mesh> mesh : meshes) {
	//	mesh->Draw();
	//}
	//triangle->Draw();
	for (std::shared_ptr<GameEntity> gameEntity : gameEntities) {
		// Set the active vertex and pixel shaders
		//  - Once you start applying different shaders to different objects,
		//    these calls will need to happen multiple times per frame
		Graphics::Context->VSSetShader(gameEntity->GetMaterial()->GetVertexShader().Get(), 0, 0);
		Graphics::Context->PSSetShader(gameEntity->GetMaterial()->GetPixelShader().Get(), 0, 0);

		// Creating vertex shader data
		VertexShaderData vsData{};
		vsData.colorTint = gameEntity->GetMaterial()->GetColorTint();
		vsData.worldMatrix = gameEntity->GetTransform()->GetWorldMatrix();

		// needs to be for active camera
		vsData.projMatrix = cameras[activeCameraNum]->GetProjMatrix();
		vsData.viewMatrix = cameras[activeCameraNum]->GetViewMatrix();

		// mapping/passing info to buffer
		D3D11_MAPPED_SUBRESOURCE map{};
		Graphics::Context->Map(constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
		memcpy(map.pData, &vsData, sizeof(vsData));
		Graphics::Context->Unmap(constantBuffer.Get(), 0);

		gameEntity->Draw();
	}

	// DRAW ImGui UI
	{
		ImGui::Render(); // Turns this frame’s UI into renderable triangles
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData()); // Draws it to the scree
	}

	// Frame END
	// - These should happen exactly ONCE PER FRAME
	// - At the very end of the frame (after drawing *everything*)
	{
		// Present at the end of the frame
		bool vsync = Graphics::VsyncState();
		Graphics::SwapChain->Present(
			vsync ? 1 : 0,
			vsync ? 0 : DXGI_PRESENT_ALLOW_TEARING);

		// Re-bind back buffer and depth buffer after presenting
		Graphics::Context->OMSetRenderTargets(
			1,
			Graphics::BackBufferRTV.GetAddressOf(),
			Graphics::DepthBufferDSV.Get());
	}
}
