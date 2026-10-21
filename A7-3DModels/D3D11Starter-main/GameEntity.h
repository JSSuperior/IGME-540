#pragma once

#include <wrl/client.h>
#include <DirectXMath.h>
#include <memory.h>
#include "Mesh.h"
#include "Transform.h"
#include "Material.h"

using namespace std;
// Used demo as reference
// https://github.com/vixorien/ggp-demos/tree/main/GGP/04%20-%20Transform
class GameEntity
{
public:
	// Constructor/destructor
	GameEntity(shared_ptr<Mesh> _mesh, shared_ptr<Material> _material);
	~GameEntity();

	// Getters
	shared_ptr<Mesh> GetMesh();
	shared_ptr<Material> GetMaterial();
	shared_ptr<Transform> GetTransform();

	// Setter(s)
	void SetMesh(shared_ptr<Mesh> _mesh);
	void SetMaterial(shared_ptr<Material> _material);

	// Other methods
	void Draw();
private:
	// pointers to mesh and transform
	shared_ptr<Mesh> mesh;
	shared_ptr<Transform> transform;
	shared_ptr<Material> material;
};

