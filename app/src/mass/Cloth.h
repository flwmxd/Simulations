#pragma once
#include "model/Mesh.h"
#include "Particle.h"
#include "Spring.h"
#include "model/Sphere.h"

class Cloth : public Mesh
{
public:
	Cloth(const std::string & name);
	auto computeNormals() -> void;
	auto onImGui() -> void override;

	auto clearForces() -> void;
	auto addForces() -> void;
	auto groundCollision(float floorY)-> void;
	auto calculateNextPosition() -> void;
	auto generateBuffer() -> void;
	auto updatePosition(const glm::vec3 & clothPosition) -> void;
	auto collisionWithMesh(const Sphere * mesh) -> void;
	inline auto getMass() const { return mass; }

	virtual auto draw(opengl::Shader* shader, const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) -> void override;
	friend class ModelLayerCloth;

	auto saveToObj(const std::string& name) -> void;


private:
	std::vector<std::shared_ptr<Particle>> particles;
	std::vector < std::vector<std::shared_ptr<Particle>>> twoDimParticles;
	std::vector<Spring> springs;
	bool play = false;

	bool lock1 = false;
	bool lock2 = false;
	bool lock3 = false;
	bool lock4 = false;
	bool windSimulation = false;
	bool collisionWithFloor = true;


	glm::vec3 position = { 0,5,0 };
	glm::vec3 windForce = { 0,0,0 };

	float mass = DEFAULT_MASS;
	float gravity = GRAVITY;
	float friction = 0.001;

};