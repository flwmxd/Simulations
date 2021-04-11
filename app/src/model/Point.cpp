


#include "glm/gtc/type_ptr.hpp"
#include "Point.h"
#include "Application.h"

bool operator >=(const glm::vec2& pos, const glm::vec3& pos2) {
	return pos.x >= pos2.x && pos.y >= pos2.y;
}

bool operator >=(const glm::vec2& pos, const glm::vec2& pos2) {
	return pos.x >= pos2.x && pos.y >= pos2.y;
}

bool operator <=(const glm::vec2& pos, const glm::vec3& pos2) {
	return pos.x <= pos2.x && pos.y <= pos2.y;
}
bool operator <=(const glm::vec2& pos, const glm::vec2& pos2) {
	return pos.x <= pos2.x && pos.y <= pos2.y;
}

ControlPoint::ControlPoint(const glm::vec3& point)
	:point(point),originPoint(point)
{

}

auto ControlPoint::draw(opengl::Shader* shader, const glm::mat4& m, const glm::mat4& v, const glm::mat4& p, BatchCommand& command) -> void
{
	projectionPoint = convertWorldToNormalized(v * m, p, point);
	command.addVertex(point, color);
}


auto ControlPoint::onMouseMove(MouseMoveEvent* event) -> bool 
{
	return false;
}

auto ControlPoint::onMouseClick(MouseClickEvent* event) -> bool 
{
	auto start = projectionPoint + glm::vec3{ -5,-5,0};
	auto end = projectionPoint + glm::vec3{ 5,5 ,0 };
	return event->position >= start && event->position <= end;
}

auto ControlPoint::onMouseRelease(MouseReleaseEvent* event) -> bool 
{
	
	return false;
}

auto ControlPoint::setSelected(bool selected) ->void
{
	select = selected;
	if (selected) {
		color = { 1.f,0,0,1.f };
	}
	else {
		color = { 1.f,1.f,1.f,1.f };
	}
}

auto ControlPoint::setColor(const glm::vec4& color) -> void
{
	this->color = color;
}

auto ControlPoint::setPosition(const glm::vec2& pos) -> void
{
	point = { pos,1 };
}

auto ControlPoint::reset() -> void
{
	point = originPoint;
}

auto ControlPoint::setAdjacents(ControlPoint* a) -> void
{
	adjacents.emplace_back(a);
}

//
auto ControlPoint::sortByDistance() -> void
{
	std::sort(adjacents.begin(), adjacents.end(), [&](const ControlPoint * a,const ControlPoint* b) {
		auto x = std::pow((a->point.x - point.x), 2);
		auto y = std::pow((a->point.y - point.y), 2);

		auto x2 = std::pow((b->point.x - point.x), 2);
		auto y2 = std::pow((b->point.y - point.y), 2);

		return std::sqrt(x + y) < std::sqrt(x2 + y2);
	});
}

auto ControlPoint::operator==(const glm::vec2& v) ->bool
{
	return v.x == point.x && v.y == point.y || v.x == originPoint.x && v.y == originPoint.y;
}
