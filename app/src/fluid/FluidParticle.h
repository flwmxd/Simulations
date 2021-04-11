#pragma once

#include <glm/glm.hpp>
#include <vector>

class FluidParticle final
{
public:

	FluidParticle(const glm::vec2 & position,float mass);

	inline auto& getPosition() const { return position; }
	inline auto setPosition(const glm::vec2& position) { this->position = position; }

	inline auto& getVelocity() const { return velocity; }
	inline auto& getNormal() const { return normal; }
	inline auto& getRenderColor()const { return renderColor; }
	inline auto setNormal(const glm::vec2& normal) { this->normal = normal; }
	
	inline auto setDensity(float density) { this->density = density; }
	inline auto getDensity() const { return density; }

	inline auto setPressure(float pressure) { this->pressure = pressure; }
	inline auto getPressure() const { return pressure; }

	inline auto setVelocity(const glm::vec2& velocity) { this->velocity = velocity; }
	inline auto setForce(const glm::vec2 & force) { this->force = force; }

	inline auto getMass() const { return mass; }
	inline auto getForce() const { return force; }

private:
	friend class ModelLayerFluid;

	glm::vec2 position = { 0,0 };
	glm::vec2 velocity = { 0,0 };

	glm::vec2 velocity12Plus = { 0,0 };
	glm::vec2 velocity12Minus = { 0,0 };


	glm::vec2 force =	{ 0,0 };
	glm::vec2 normal = { 0,0 };
	glm::vec2 color = {0,0};


	float mass = 0;
	float density = 0;
	float pressure = 0;

	glm::vec4 renderColor = {1,1,1,1.f};


};