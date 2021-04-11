


#include "Model.h"
#include "opengl/Shader.h"
#include "glm/gtc/type_ptr.hpp"
#include "ModelLayer.h"
#include "Application.h"
#include "Interpolation.h"

Model::Model(const std::string& name)
{

	objModel = glmReadOBJ(name.c_str());
	glmUnitize(objModel);
	glmFacetNormals(objModel);
	glmVertexNormals(objModel, 90.0);
	glmLoadInVBO(objModel);
	aabb.min = glm::vec3{ 1e37f,1e37f,1e37f };
	aabb.max = glm::vec3{ -1e37f,-1e37f,-1e37f };

	
	for (auto i = 1; i <= objModel->numvertices; i++) {

		auto x =  objModel->vertices[3 * i + 0];
		auto y =  objModel->vertices[3 * i + 1];
		auto z =  objModel->vertices[3 * i + 2];


		if (aabb.max.x < x)
			aabb.max.x = x;
		if (aabb.min.x > x)
			aabb.min.x = x;

		if (aabb.max.y < y )
			aabb.max.y = y ;
		if (aabb.min.y > y )
			aabb.min.y = y ;

		if (aabb.max.z < z)
			aabb.max.z = z;
		if (aabb.min.z > z)
			aabb.min.z = z;
	}

	origin = aabb.min;

	app->getRenderer().getImLayer().setModel(this);

	for (auto i = 1; i <= objModel->numvertices; i++) {
		auto x =  objModel->vertices[3 * i + 0];
		auto y =  objModel->vertices[3 * i + 1];
		auto z =  objModel->vertices[3 * i + 2];
		//local stu position;
		meshCoordinates.emplace_back((glm::vec3{x,y,z} - origin) /  (aabb.max - aabb.min));
	}



}

auto Model::draw(opengl::Shader* shader, const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) -> void
{
	if (objModel == nullptr) {
		return;
	}
	glm::mat4 modelViewMatrix = v * m;
	glm::mat3 normalMatrix = glm::inverse(glm::mat3(modelViewMatrix)); // Normal Matrix
	shader->useProgram();
	// Send the model, view and projection matrices to the shader 
	shader->setUniformMatrix4f("model", glm::value_ptr(m));
	shader->setUniformMatrix4f("view", glm::value_ptr(v));
	shader->setUniformMatrix4f("projection", glm::value_ptr(p));
	shader->setUniformMatrix3f("normalMatrix", glm::value_ptr(normalMatrix));
	glmDrawVBO(objModel, shader->getProgramId());
}

auto Model::draw(opengl::Shader* shader, const glm::mat4& m, const glm::mat4& v, const glm::mat4& p, BatchCommand& command) -> void
{
	for (auto & c : controlPoints)
	{
		c.draw(shader, m, v, p, command);
	}
}

auto Model::setOrder(int32_t order,ModelLayer * parent) -> void
{
	this->order = order;
	auto delta = (aabb.max - aabb.min) / (order - 1.0f);
	parent->getLines().clear();
	controlPoints.clear();
	for (int32_t i = 0;i<order ;i++)
	{
		for (int32_t j = 0; j < order; j++)
		{
			for (int32_t k = 0; k < order; k++)
			{
				controlPoints.emplace_back(origin + glm::vec3{ i,j,k } * delta);
			}
		}
	}
	
	for (auto i = 0; i < getOrderS() - 1; i++) {
		for (auto j = 0; j < getOrderT(); j++) {
			for (auto k = 0; k < getOrderU(); k++) {
				parent->getLines().emplace_back(&getControlPoints()[k + (j * getOrderU()) + (i * (getOrderS() * getOrderU()))]);
				parent->getLines().emplace_back(&getControlPoints()[k + (j * getOrderU()) + ((i + 1) * (getOrderS() * getOrderU()))]);
			}
		}
	}

	for (auto i = 0; i < getOrderS(); i++) {
		for (auto j = 0; j < getOrderT() - 1; j++) {
			for (auto k = 0; k < getOrderU(); k++) {
				parent->getLines().emplace_back(&getControlPoints()[k + (j * getOrderS()) + (i * (getOrderT() * getOrderU()))]);
				parent->getLines().emplace_back(&getControlPoints()[k + ((j + 1) * getOrderS()) + (i * (getOrderT() * getOrderU()))]);
			}
		}
	}

	for (auto i = 0; i < getOrderS(); i++) {
		for (auto j = 0; j < getOrderT(); j++) {
			for (auto k = 0; k < getOrderU() - 1; k++) {
				parent->getLines().emplace_back(&getControlPoints()[k + (j * getOrderT()) + (i * (getOrderT() * getOrderU()))]);
				parent->getLines().emplace_back(&getControlPoints()[(k + 1) + (j * getOrderT()) + (i * (getOrderT() * getOrderU()))]);
			}
		}
	}

}

auto Model::onMouseMove(MouseMoveEvent* event) -> bool
{

	for (auto& c : controlPoints)
	{
		if (c.onMouseMove(event)) {
			return true;
		}
	}

	return false;
}

auto Model::onMouseClick(MouseClickEvent* event) -> bool
{
	for (auto& c : controlPoints)
	{
		if (c.onMouseClick(event)) {
			if (current != nullptr) {
				current->setSelected(false);
			}
			app->getRenderer().getImLayer().setCurrentPoint(&c);
			current = &c;
			current->setSelected(true);
			return true;
		}
	}
	return false;
}

auto Model::onMouseRelease(MouseReleaseEvent* event) -> bool
{

	for (auto& c : controlPoints)
	{
		if (c.onMouseRelease(event)) {
			return true;
		}
	}

	return false;
}


auto Model::deform() -> void
{
	Interpolation::deform3D(objModel, controlPoints, meshCoordinates, order,order,order);
	glmLoadInVBO(objModel);
}
