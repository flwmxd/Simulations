



#pragma once
#include "engine/RenderObject.h"
#include "opengl/Shader.h"
#include "libobj/libobj.h"
#include "Point.h"
#include "event/EventHandler.h"



class ModelLayer;

class Model : public RenderObject 
{
public:
	friend class ImLayer;
	Model(const std::string& name);
	virtual auto draw(opengl::Shader* shader, const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) -> void override;
	virtual auto draw(opengl::Shader* shader, const glm::mat4& m, const glm::mat4& v, const glm::mat4& p, BatchCommand & command) -> void override;
	inline auto& getAABB() const { return aabb; }
	auto setOrder(int32_t order, ModelLayer* parent)-> void;
	virtual auto onMouseMove(MouseMoveEvent* event) -> bool override;
	virtual auto onMouseClick(MouseClickEvent* event) -> bool override;
	virtual auto onMouseRelease(MouseReleaseEvent* event) -> bool override;

	inline auto getOrderS() const { return order; }
	inline auto getOrderT() const { return order; }
	inline auto getOrderU() const { return order; }

	auto deform() -> void;
	inline auto& getControlPoints()  { return controlPoints; }



private:
	GLMmodel* objModel = nullptr;
	AABB aabb;

	glm::vec3 origin;

	EventHandler eventHandler;
	
	ControlPoint* current = nullptr;

	std::vector<glm::vec3> meshCoordinates;

	std::vector<ControlPoint> controlPoints;

	int32_t order = 2;


};