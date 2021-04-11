#include "SmoothingKernel.h"
#include <glm/glm.hpp>

//poly6(r,h) = 315/64*π*(h^9) if 0 ≤ r ≤ h  then  (h^2 - r62) otherwise 0
auto Poly6::calculate(const glm::vec2& distance, float smooth) -> float
{
	float r2 = distance.x * distance.x + distance.y * distance.y;
	float h2 = smooth * smooth;
	if (r2 < 0 || r2 > h2) return 0.0f;
	return 315.0f / (64.0f * M_PI * std::pow(smooth, 9)) * std::pow(h2 - r2, 3);
}

//∇Poly6(r,h) = -r * (945/ 32 * π * h^9) * (h^2 - r^2)^2
auto Poly6::calculateGradient(const glm::vec2& distance, float smooth) ->glm::vec2
{
	float h2 = smooth * smooth;
	auto t1 = -1 * (945.0f / (32.0f * M_PI * (float)std::pow(smooth, 9.0f)));
	float r2 = distance.x * distance.x + distance.y * distance.y;

	if (r2 > h2 || r2 <= 1e-8)
	{
		return {0,0};
	}
	
	float t3 = std::pow(h2 - r2, 2);
	return t1 * distance * t3;
}

auto Poly6::calculateLaplacian(const glm::vec2& distance, float smooth) -> float
{
	float coefficient = -945.0 / (32.0 * M_PI * std::pow(smooth, 9));
	float hSquared = smooth * smooth;
	float radiusSquared = distance.x * distance.x + distance.y * distance.y;
	return coefficient * (hSquared - radiusSquared) * (3.0 * hSquared - 7.0 * radiusSquared);
}

//∇Spiky(r,h) = -r * (45/π * h^6 * r) * (h - r)^2
auto Spiky::calculateGradient(const glm::vec2& distance, float smooth) -> glm::vec2
{
	float r = glm::length(distance);
	if (r == 0.0f) return { 0,0 };

	float t1 = -45.0f / (M_PI * std::pow(smooth, 6));
	glm::vec2 t2 = distance / r;
	float t3 = std::pow(smooth - r, 2);

	return  t1 * t3 * t2 ;
}


auto Viscosity::calculate(const glm::vec2& distance, float smooth) -> float
{
	float r = glm::length(distance);
	return 45.0f / (M_PI * std::pow(smooth, 6)) * (smooth - r);
}
