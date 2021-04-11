#include "Interpolation.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

auto Interpolation::factorials(int32_t n) ->int32_t
{
	double fact = 1;
	for (int32_t i = n; i > 0; i--)
	{
		fact *= i;
	}
	return fact;
}

auto Interpolation::bernstein(int32_t i, int32_t l, double s) -> float
{
	double binomial = factorials(l) / (factorials(l - i) * factorials(i));
	return binomial * std::pow(s, (double)i) * std::pow(1 - s, (double)(l - i));
}

auto Interpolation::deform3D(GLMmodel* objModel, const std::vector<ControlPoint>& controlPoints, std::vector<glm::vec3>& localSTU, int32_t s, int32_t t, int32_t u) -> void
{
	for (auto v = 1; v <= objModel->numvertices; v++) {
		auto& x = objModel->vertices[3 * v + 0];
		auto& y = objModel->vertices[3 * v + 1];
		auto& z = objModel->vertices[3 * v + 2];

		float localS = localSTU[v - 1].x;
		float localT = localSTU[v - 1].y;
		float localU = localSTU[v - 1].z;

		glm::vec3 point = {};
		for (int i = 0; i < s; i++) {
			for (int j = 0; j < t; j++) {
				for (int k = 0; k < u; k++) {
					//
					point += controlPoints[k + (j * t) + (i * t * u)].getPoint() * (
						Interpolation::bernstein(i, s - 1, localS) *
						Interpolation::bernstein(j, t - 1, localT) *
						Interpolation::bernstein(k, u - 1, localU)
						);
				}
			}
		}
		x = point.x;
		y = point.y;
		z = point.z;
	}
}

auto Interpolation::bilinearFilter(int32_t x, int32_t y, float ux, float uy, int32_t cells, const std::vector<ControlPoint>& gridPoints) -> glm::vec2
{
	auto index = x * (cells + 1) + y;

	//should use the origin point to calculate the weight;
	auto origin1 = gridPoints[index].getOriginPoint();
	auto origin2 = gridPoints[index + cells + 1].getOriginPoint();
	auto origin3 = gridPoints[index + 1].getOriginPoint();
	auto origin4 = gridPoints[index + cells + 2].getOriginPoint();

	auto p1 =(gridPoints[index].getPoint());
	auto p2 =(gridPoints[index + cells + 1].getPoint());
	auto p3 =(gridPoints[index + 1].getPoint());
	auto p4 =(gridPoints[index + cells + 2].getPoint());
	

	//weight = (current - origin) / (gridMax - gridMin);
	float weightX = (ux - origin1.x) / (origin2.x - origin1.x);
	float weightY = (uy - origin1.y) / (origin3.y - origin2.y);
 	auto delta1 = glm::lerp(p1,p2, weightX);
	auto delta2 = glm::lerp(p3,p4, weightX);
	return glm::lerp(delta1, delta2, weightY);
}

auto Interpolation::convertPointToGrid(float v, float grid) ->int32_t
{
	v =  std::round((v + 0.5f) * grid);
	
	return (int32_t)v;
}

auto Interpolation::convertGridToIndex(int32_t x, int32_t y,int32_t cells) ->int32_t
{
	return x * cells + y;
}
//https://zhuanlan.zhihu.com/p/140907023
auto Interpolation::barycentricInterpolation(const ControlPoint* a, const ControlPoint* b, const ControlPoint* c, const glm::vec3& pos) -> glm::vec2
{

	auto xa = a->getOriginPoint().x;
	auto xb = b->getOriginPoint().x;
	auto xc = c->getOriginPoint().x;
	auto ya = a->getOriginPoint().y;
	auto yb = b->getOriginPoint().y;
	auto yc = c->getOriginPoint().y;

	auto alpha = (-(pos.x - xb) * (yc - yb) + (pos.y - yb) * (xc - xb)) / (-(xa - xb) * (yc - yb) + (ya - yb) * (xc - xb));

	auto beta = (-(pos.x - xc) * (ya - yc) + (pos.y - yc) * (xa - xc)) / (-(xb - xc) * (ya - yc) + (yb - yc) * (xa - xc));

	auto gamma = 1 - alpha - beta;


	return alpha * a->getPoint() + beta * b->getPoint() + gamma * c->getPoint();

}
