#pragma once

#include <wrl/client.h>
#include <DirectXMath.h>
#include <memory>
#include "Mesh.h"
#include "Transform.h"

// Used demo as reference
// https://github.com/vixorien/ggp-demos/tree/main/GGP/04%20-%20Transform
class GameEntity
{
public:
	// Constructor/destructor
	GameEntity(std::shared_ptr<Mesh> _mesh);
	~GameEntity();

	// Getters
	std::shared_ptr<Mesh> GetMesh();
	std::shared_ptr<Transform> GetTransform();

	// Setter(s)
	void SetMesh(std::shared_ptr<Mesh> _mesh);

	// Other methods
	void Draw();
private:
	// pointers to mesh and transform
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Transform> transform;
};

