
#pragma once
#include "Mesh.h"

class Sphere : public Mesh 
{
public:
	Sphere(const std::string& name);
	auto draw(opengl::Shader* shader, const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) -> void override;
	auto onImGui() -> void override;
	inline auto& getPosition() const { return postion; }
	inline auto& getRadius() const { return radius; }
private:
	float radius = 0.025;
	glm::vec3 postion = {0,0,0};
	std::vector<uint32_t> indices;
}; 