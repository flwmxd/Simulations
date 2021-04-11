
#pragma once
#include <cstdint>
#include <cmath>
#include <vector>
#include "model/Point.h"

namespace Interpolation
{
	auto factorials(int32_t n)->int32_t;
	auto bernstein(int32_t i, int32_t l, double s) -> float;
	auto deform3D(GLMmodel* model, const std::vector<ControlPoint>& points, std::vector<glm::vec3>& localSTU,int32_t s,int32_t t,int32_t u) -> void;
	auto bilinearFilter(int32_t x, int32_t y, float u, float v, int32_t cells, const std::vector<ControlPoint>& gridPoints)->glm::vec2;
	auto convertPointToGrid(float v, float grid)->int32_t;
	auto convertGridToIndex(int32_t x,int32_t y,int32_t cell)->int32_t;
	auto barycentricInterpolation(const ControlPoint * a, const ControlPoint* b, const ControlPoint * c,const glm::vec3& position) -> glm::vec2;
};