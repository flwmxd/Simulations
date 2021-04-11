#include "Skeleton.h"
#include "opengl/OGL.h"
#ifndef M_PI
#define M_PI 3.14159265f
#endif
#include <glm/gtc/type_ptr.hpp>
#include "engine/Camera.h"
#include "engine/Transform.h"
#include "ik/IkEffector.h"
#include "engine/Layer.h"


Skeleton::Skeleton(const std::string& path, Camera* camera, Layer* layer)
	:object(path), camera(camera), layer(layer)
{
	cylinder.init();
	//effector.init();
	timePoint = std::chrono::steady_clock::now();

	ik = std::make_unique<InverseKinematics>(&object);

	linesCommand = std::make_unique<BatchCommand>(BatchCommand::Type::LINES);
}

auto Skeleton::draw(opengl::Shader* shader, const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) -> void
{

	const auto current = std::chrono::steady_clock::now();
	const auto elapsed_duration = std::chrono::duration_cast<std::chrono::milliseconds>(current - timePoint);
	if (!pause) {
		frame = static_cast<int>((static_cast<double>(elapsed_duration.count()) / 1000.0) / object.frame_time()) % object.frames();
	}
	shader->setUniformValue("lightEnable", false);
	jointList.clear();
	drawJoint(shader, object.root_joint(),m,v,p, frame);
	cylinder.resetTransform();

	if (ik->getEffector() != nullptr) 
	{
		//effector.draw(shader,m, v, p, ik->getEffector()->getPosition(), glm::vec4{ 0.5,1.0,0.3,1 });
	}

	computeIK();
}


auto Skeleton::drawJoint(opengl::Shader* shader, const std::shared_ptr<bvh11::Joint>& joint, const glm::mat4& m, const glm::mat4& v, const glm::mat4& p,int32_t frame, bool parentSelected ) -> void
{
	constexpr double radius = 0.025;

	const Eigen::Matrix4f transform = object.GetTransformationRelativeToParent(joint, frame).matrix();
	auto data = transform.data();
	auto tran = m * glm::mat4(
		data[0], data[1], data[2], data[3],
		data[4], data[5], data[6], data[7],
		data[8], data[9], data[10], data[11],
		data[12], data[13], data[14], data[15]
	) ;

	auto rotation = glm::mat3(1);// jacobian->getLocalRotation(joint);
	auto rx = rotation * glm::vec3(1.f, 0.f, 0.f);
	auto ry = rotation * glm::vec3(0.f, 1.f, 0.f);
	auto rz = rotation * glm::vec3(0.f, 0.f, 1.f);
/*
	auto trans = jacobian->getTranslation(joint);
	auto model =  glm::mat4(rotation) * glm::translate(m, trans);*/

	layer->getFlatShader()->useProgram();
	layer->getFlatShader()->setUniformMatrix4f("model", glm::value_ptr(tran));
	layer->getFlatShader()->setUniformMatrix4f("view", glm::value_ptr(v));
	layer->getFlatShader()->setUniformMatrix4f("projection", glm::value_ptr(p));

	linesCommand->addVertex({ 0,0,0 }, glm::vec4{ 1,0,0,1 });
	linesCommand->addVertex({ 1,0,0 }, glm::vec4{ 1,0,0,1 });

	linesCommand->addVertex({ 0,0,0 }, glm::vec4{ 0,1,0,1 });
	linesCommand->addVertex({ 0,1,0 }, glm::vec4{ 0,1,0,1 });

	linesCommand->addVertex({ 0,0,0 }, glm::vec4{ 0,0,1,1 });
	linesCommand->addVertex({ 0,0,1 }, glm::vec4{ 0,0,1,1 });

	linesCommand->flush(*layer->getFlatShader());

	//jointList.emplace_back(std::make_pair(cylinder.getTransform(), joint));

	//sphere.draw(shader, tran, v, p, glm::vec3{ 0,0,0 }, joint->isSelected() ? glm::vec4{ 1,0,0,1 } : glm::vec4{ 1,1,0,1 });

	for (auto & child : joint->children())
	{
		cylinder.draw(shader, tran, v, p, Eigen::Vector3f::Zero(), child->offset(), joint->isSelected() ? glm::vec4{1,0,0,1} : glm::vec4{ 1,1,0,1 });
		drawJoint(shader, child, tran,v,p, frame, joint->isSelected());
	}

	if (joint->has_end_site())
	{
		auto end = joint->end_site();
		cylinder.draw(shader, tran, v, p, Eigen::Vector3f::Zero(), end, joint->isSelected() ? glm::vec4{ 1,0,0,1 } : glm::vec4{ 1,1,0,1 });
		//sphere.draw(shader, tran, v, p, glm::vec3{ end.x(), end.y(), end.z() }, joint->isSelected() ? glm::vec4{ 1,0,0,1 } : glm::vec4{ 0,0,0,1 });
	}

	//jacobian->compute({ constrainX,constrainY,constrainZ }, delta, frame);
}

auto Skeleton::reset() -> void
{
	
}


auto Skeleton::onMouseClick(MouseClickEvent* event) -> bool
{

	for (auto & jonit : jointList)
	{
		auto translation = Transform::getTranslation(jonit.first);
		auto projectionPoint = convertWorldToNormalized(camera->getViewMatrix() * jonit.first, camera->getProjection(), translation);

		auto start = projectionPoint + glm::vec3{ -15,-15,0 };
		auto end = projectionPoint + glm::vec3{ 15,15 ,0 };
		if (event->position.x >= start.x &&
			event->position.y >= start.y &&
			event->position.x <= end.x && 
			event->position.y <= end.y
			)
		{
			jonit.second->setSelected(true);
			return true;
		}
	}

	return false;
}

auto Skeleton::computeIK() -> void
{
	ik->update(frame);
	ik->compute(delta, frame);
}

auto Skeleton::computeIKWithSingleJoint(const glm::vec3& d,const std::shared_ptr<bvh11::Joint> & joint) -> void
{
	delta = d;
	ik->beginDrag(delta, joint,frame);
	ik->bindJoint(joint,false);
}

auto Skeleton::setCamera(Camera* camera) -> void
{
	this->camera = camera;
}

auto Skeleton::bindJoint(std::shared_ptr<bvh11::Joint> joint) -> void
{
	ik->bindJoint(joint,true,frame);
}

