#include "GameEntity.h"
#include "BufferStructs.h"
#include "Graphics.h"

// Used demo as reference
// https://github.com/vixorien/ggp-demos/tree/main/GGP/04%20-%20Transform

// Constructor
GameEntity::GameEntity(std::shared_ptr<Mesh> _mesh) : mesh(_mesh)
{
	transform = std::make_shared<Transform>();
}

// Destructor
GameEntity::~GameEntity()
{
}

// Getters
std::shared_ptr<Mesh> GameEntity::GetMesh()
{
	return mesh;
}

std::shared_ptr<Transform> GameEntity::GetTransform()
{
	return transform;
}

// Setter(s)
void GameEntity::SetMesh(std::shared_ptr<Mesh> _mesh)
{
	mesh = _mesh;
}

// Draw
void GameEntity::Draw()
{
	mesh->Draw();
}
