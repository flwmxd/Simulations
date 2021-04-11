

#pragma once

#include <glm/glm.hpp>


namespace MathTools 
{
	auto pseudoInverse(const glm::mat4& jacobian)->glm::mat4;
};