



#pragma once
#include "engine/RenderObject.h"
#include "opengl/Shader.h"
#include "libobj/libobj.h"


class ControlPoint : public RenderObject
{
public:
	friend class ImLayer;
	ControlPoint() = default;
	ControlPoint(const glm::vec3& point);
	virtual auto draw(opengl::Shader* shader, const glm::mat4& m, const glm::mat4& v, const glm::mat4& p, BatchCommand& command) -> void ;
	inline auto& getPoint() const { return point; }
	inline auto& getOriginPoint() const { return originPoint; }
	virtual auto onMouseMove(MouseMoveEvent* event) -> bool override;
	virtual auto onMouseClick(MouseClickEvent* event) -> bool override;
	virtual auto onMouseRelease(MouseReleaseEvent* event) -> bool override;
	virtual auto setSelected(bool selected)->void;
	auto setColor(const glm::vec4& color) -> void;
	auto setPosition(const glm::vec2& pos) -> void;
	auto operator==(const glm::vec2& v)->bool;
	auto reset() -> void;
	auto setAdjacents(ControlPoint*) -> void;
	//sort before

	inline auto& getAdjacents() { sortByDistance(); return adjacents; }
	auto sortByDistance() -> void;
private:
	glm::vec3 point;
	glm::vec3 originPoint;
	glm::mat4 matrix;
	glm::mat4 modelView;
	glm::vec3 projectionPoint;
	glm::mat4 projection;
	bool clicked = false;
	bool select = true;
	glm::vec4 color = { 1,1,1,1 };
	std::vector<ControlPoint*> adjacents;
};