

#pragma once
#include "glm/glm.hpp"
#include "opengl/Shader.h"
#include "opengl/BatchCommand.h"
#include "event/Event.h"
struct AABB
{
	glm::vec3 min;
	glm::vec3 max;
};

struct AABB2D
{
	glm::vec2 min;
	glm::vec2 max;
};



class RenderObject 
{
public:
	virtual ~RenderObject() = default;
	virtual auto draw(opengl::Shader* shader, const glm::mat4& p) -> void;;
	virtual auto draw(opengl::Shader* shader, const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) -> void {};
	virtual auto draw(opengl::Shader* shader, const glm::mat4& m, const glm::mat4& v, const glm::mat4& p, BatchCommand& command) -> void {};
	virtual auto onMouseMove(MouseMoveEvent* event) -> bool { return false; }
	virtual auto onMouseClick(MouseClickEvent* event) -> bool { return false; }
	virtual auto onMouseRelease(MouseReleaseEvent* event) -> bool { return false; }

	virtual auto onImGui() -> void {};
	static auto convertWorldToNormalized(const glm::mat4& modelView, const glm::mat4& projection, const glm::vec3& point)->glm::vec3;
	static auto convertNormalizedToWorld(const glm::mat4& modelView, const glm::mat4& projection, const glm::vec3& normalizedPosition)->glm::vec4;

protected:



};

