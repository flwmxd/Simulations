#include "Spring.h"
#include "Particle.h"
#include "glm/glm.hpp"

Spring::Spring()
{
}

Spring::Spring(const std::shared_ptr<Particle>& x1, const std::shared_ptr<Particle>& x2, double spring)
{
	p1 = x1;
	p2 = x2;
	restLength = glm::distance(p1->getPosition(), p2->getPosition());
	springConstant = spring;
}

auto Spring::addForce() -> void
{
	
	float norm = glm::distance(p1->getPosition(), p2->getPosition());

	glm::vec3 pos = p1->getPosition() - p2->getPosition();//dp
	glm::vec3 velocity = p1->getVelocity() - p2->getVelocity();//dv

	glm::vec3 vec = pos;
	vec *= (2.0f / glm::length(pos));

	//Hookean formula   ->  F = -k*dx 
	//
	glm::vec3 formula = -(glm::vec3(DEFAULT_K * (norm - restLength)) +
		glm::vec3(M_KD * (glm::dot(pos, velocity) / norm)));

	glm::vec3 dampingForce = vec * formula;

	glm::vec3 force = (float)DEFAULT_K * (norm - (float)restLength) * (pos / norm);

	p1->addForce(force);
	p2->addForce(-force);
	p1->addForce(dampingForce);
	p2->addForce(-dampingForce);
}
