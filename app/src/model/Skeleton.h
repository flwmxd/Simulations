#pragma once

#include "engine/RenderObject.h"
#include <bvh11.hpp>
#include "Sphere.h"
#include "Cylinder.h"
#include "engine/Timer.h"
#include "ik/InverseKinematics.h"

class Camera;
class Layer;
class Skeleton : public RenderObject
{
public:
	friend class ImLayer;
	Skeleton(const std::string & path, Camera* camera,Layer * layer);
	auto draw(opengl::Shader* shader, const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) -> void override;
	auto drawJoint(opengl::Shader* shader, const std::shared_ptr<bvh11::Joint> & jont,const glm::mat4& m, const glm::mat4& v, const glm::mat4& p, int32_t frame,bool parentSelected = false) -> void;
	auto reset() -> void;
	auto onMouseClick(MouseClickEvent* event) -> bool override;
	inline auto& getBvhObject() { return object; }
	inline auto& getInverseKinematics() { return ik; }

	auto computeIK() -> void;
	auto computeIKWithSingleJoint(const glm::vec3& delta, const std::shared_ptr<bvh11::Joint>& joint) -> void;
	
	auto setCamera(Camera* camera) -> void;
	auto bindJoint(std::shared_ptr<bvh11::Joint> joint) -> void;

private:
	
	std::chrono::steady_clock::time_point timePoint;
	bvh11::BvhObject object;
	Timer timer;
	Cylinder cylinder;
	//Sphere sphere;
	//Sphere effector;
	int32_t frame = 0;
	bool pause = false;
	std::vector<std::pair<glm::mat4, std::shared_ptr<bvh11::Joint>>> jointList;
	std::unique_ptr<BatchCommand> linesCommand;
	Camera* camera = nullptr;
	std::unique_ptr<InverseKinematics> ik;
	glm::vec3 delta;
	Layer* layer;
};