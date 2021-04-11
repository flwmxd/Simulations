#pragma once
#include <glm/vec2.hpp>
#include <cmath>

#ifndef M_PI 
#define M_PI    3.14159265358979323846f 
#endif


namespace Poly6 
{
	 auto calculate(const glm::vec2& distance, float smooth) -> float;
	 auto calculateGradient(const glm::vec2& distance, float smooth)->glm::vec2;
	 auto calculateLaplacian(const glm::vec2& distance, float smooth)-> float;
};


namespace Spiky
{
	 auto calculateGradient(const glm::vec2& distance, float smooth) ->glm::vec2;
};

namespace Viscosity
{
	 auto calculate(const glm::vec2& distance, float smooth) -> float;
};


