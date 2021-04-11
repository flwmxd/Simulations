#include "Particle.h"
#include "Cloth.h"

Particle::Particle()
{
}

Particle::Particle(const glm::vec3& pos, Cloth* initCloth)
	:position(pos),oldPosition(pos), cloth(initCloth)
{

}

auto Particle::addForce(const glm::vec3& f) -> void
{
	force += f;
}

auto Particle::calculateNextPosition() -> void
{
	if (!movable) {
		return;
	}
	glm::vec3 acceleration = force / cloth->getMass();
	glm::vec3 tmp = position;
	 
	//x = v0*t + 1/2 * a* t^2;
	//xi = xi + dx + 1/2 * a* t^2;
	position = position + (position - oldPosition) * 0.99f + acceleration * (float)DELTA_T;
	oldPosition = tmp;
}
