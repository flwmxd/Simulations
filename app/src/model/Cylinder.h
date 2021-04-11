
#pragma once
#include "Primitive.h"
#include <Eigen/Core>
#include <Eigen/Geometry>

class Cylinder : public Primitive
{
public:
	auto init() -> void override;
	auto draw(opengl::Shader* shader, const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) -> void override;
	auto draw(opengl::Shader* shader, const glm::mat4& m, const glm::mat4& v, const glm::mat4& p, const Eigen::Vector3f& p1, const Eigen::Vector3f& p2,const glm::vec4 & color) -> void;
	auto draw(opengl::Shader* shader, double radius, double height, int32_t resolution = 30) -> void;
	inline auto& getTransform() const { return transform; }
	auto resetTransform() -> void;
private:
	glm::mat4 transform;
};