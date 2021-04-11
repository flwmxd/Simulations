
#pragma once
#include "Primitive.h"
#include "Material.h"


class Floor : public Primitive 
{
public:
	friend class ModelLayerCloth;
	auto init() -> void override;
	//need to refactor later.......
	auto initWthCloth() -> void;
	auto draw(opengl::Shader* shader, const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) -> void override;
	auto onImGui() -> void;
private:
	Material material;
	bool active = true;
}; 