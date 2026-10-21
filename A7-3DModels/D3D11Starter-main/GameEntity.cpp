#include "GameEntity.h"
#include "BufferStructs.h"
#include "Graphics.h"

// Used demo as reference
// https://github.com/vixorien/ggp-demos/tree/main/GGP/04%20-%20Transform

// Constructor
GameEntity::GameEntity(shared_ptr<Mesh> _mesh, shared_ptr<Material> _material) : mesh(_mesh)
{
	transform = make_shared<Transform>();
	material = _material;
}

// Destructor
GameEntity::~GameEntity()
{
}

// Getters
shared_ptr<Mesh> GameEntity::GetMesh()
{
	return mesh;
}

shared_ptr<Material> GameEntity::GetMaterial()
{
	return material;
}

shared_ptr<Transform> GameEntity::GetTransform()
{
	return transform;
}

// Setter(s)
void GameEntity::SetMesh(shared_ptr<Mesh> _mesh)
{
	mesh = _mesh;
}

void GameEntity::SetMaterial(shared_ptr<Material> _material)
{
	material = _material;
}

// Draw
void GameEntity::Draw()
{
	mesh->Draw();
}
