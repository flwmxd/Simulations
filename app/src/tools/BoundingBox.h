#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "opengl/Vertex.h"

constexpr float M_INFINITY = (float)HUGE_VAL;
class BoundingBox
{
public:
	/// Construct with zero size.
	BoundingBox() noexcept :
		minValue(M_INFINITY, M_INFINITY, M_INFINITY),
		maxValue(-M_INFINITY, -M_INFINITY, -M_INFINITY)
	{
	}

	/// Copy-construct from another bounding box.
	BoundingBox(const BoundingBox& box) noexcept :
		minValue(box.minValue),
		maxValue(box.maxValue)
	{
	}


	/// Construct from minimum and maxValueimum vectors.
	BoundingBox(const glm::vec3& min, const glm::vec3& maxValue) noexcept :
		minValue(min),
		maxValue(maxValue)
	{
	}

	/// Construct from minimum and maxValueimum floats (all dimensions same.)
	BoundingBox(float min, float max) noexcept :
		minValue(glm::f64vec3(min, min, min)),
		maxValue(glm::f64vec3(max, max, max))
	{
	}


	auto define(const std::vector<Vertex>& vertex) -> void;
	auto merge(const std::vector<Vertex>& vertex) -> void;
	auto merge(const Vertex & v) -> void;

	auto clear() -> void;

	inline auto center() { return (maxValue + minValue) * 0.5; }
	inline auto size() { return maxValue - minValue; }
	inline auto halfSize() { return 0.5 * (maxValue - minValue) ; }

	inline auto getMaxValue() const { return maxValue; }
	inline auto getMinValue() const { return minValue; }

private:
	glm::f64vec3 minValue;
	glm::f64vec3 maxValue;
};