#pragma once
#include "Config.h"
#include <glm/glm.hpp>
#include <vector>
class Cloth;
class Particle final
{
public:

	friend class Cloth;

	Particle();
	Particle(const glm::vec3 & position, Cloth * cloth);

	auto addForce(const glm::vec3& force) -> void;
	auto calculateNextPosition() -> void;

	inline auto& getPosition() const { return position; }
	inline auto& getVelocity() const { return velocity; }
	inline auto& getNormal() const { return normal; }
	inline auto setNormal(const glm::vec3& normal) { this->normal = normal; }
	inline auto setTextCoord(const glm::vec2& textCoord) { this->textCoord = textCoord; }
	inline auto& getTextCoord() const { return textCoord; }
	inline auto setMoveable(bool b) { movable = b; }
private:
	glm::vec3 oldPosition = {0,0,0};
	glm::vec3 position = { 0,0,0 };

	glm::vec3 velocity = { 0,0,0 };
	glm::vec3 oldVelocity = { 0,0,0 };

	glm::vec3 force = { 0,0,0 };
	glm::vec3 normal = { 0,0,0 };

	bool movable = true;

	Cloth* cloth = nullptr;
	
	glm::vec2 textCoord = {0,0};

};