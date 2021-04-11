


#pragma once
#include "opengl/VertexArray.h"
#include "engine/RenderObject.h"

class Primitive : public RenderObject
{
public:
	virtual auto init() -> void;
	virtual auto draw(opengl::Shader* shader, const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) -> void;

protected:
	std::unique_ptr<VertexArray> vertexArray;
};