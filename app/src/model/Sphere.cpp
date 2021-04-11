

#include "Sphere.h"
#include "opengl/Buffer.h"
#include "opengl/Vertex.h"
#include "opengl/OGL.h"
#include <cmath>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

#ifndef M_PI
#define M_PI 3.14159265f
#endif


Sphere::Sphere(const std::string& name)
	:Mesh(name,true)
{

}

auto Sphere::draw(opengl::Shader* shader, const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) -> void 
{
	auto model = glm::mat4(1.f);
	model *= glm::translate(glm::mat4(1.f), postion);
	model = model * m;
	model *= glm::scale(glm::mat4(1.0f), { radius,radius, radius });
	Mesh::draw(shader, model, v, p);
}


auto Sphere::onImGui() -> void
{
	ImGui::DragFloat3("Sphere Position", &postion[0],0.01);
	ImGui::DragFloat("Radius", &radius,0.1);
}

