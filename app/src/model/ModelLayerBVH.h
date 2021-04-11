

#pragma once
#include "engine/Layer.h"
#include "Skeleton.h"
#include "event/EventHandler.h"
#include "engine/Camera.h"
#include "Sphere.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Floor.h"
#include <string>

class ModelLayerBVH : public Layer
{
public:
	friend class ImLayer;
	virtual auto init() -> void override;
	virtual auto draw(const Timestep& timestep) -> void override;
	auto loadModel(const std::string& name) -> void;
	inline auto& getSkeleton() { return skeleton; }
private:
	std::unique_ptr<opengl::Shader> shader;
	std::unique_ptr<Skeleton> skeleton;
	std::unique_ptr <Camera> camera;
	Floor floor;
	glm::mat4 projection;
	glm::mat4 view;
	glm::vec3 cameraPos;
	EventHandler handler;
	double limitedFPS = 0;
	Cylinder sphere;
	bool mouseClicked = false;
	glm::vec2 lastPosition;
}; 