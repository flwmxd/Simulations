

#pragma once
#include <glm/glm.hpp>


class IkEffector 
{
public:
	friend class ImLayer;
	IkEffector(const glm::vec3& angle);
	inline auto getPosition() const { return position; }
	inline auto getAngle() const { return angle; }
private:
	glm::vec3 angle;
	glm::vec3 position;
};