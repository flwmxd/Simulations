#include "ModelLayerBVH.h"
#include "Application.h"

auto ModelLayerBVH::init() -> void 
{
	Layer::init();
	CHECK_GL_ERROR();
	shader = std::make_unique<opengl::Shader>("shader/vertIkShader.glsl","shader/fragIkShader.glsl");
	shader->compile();

	projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.f, 100.0f);
	camera = std::make_unique<Camera>(projection, glm::vec3(0, 0, 3));

	handler.mouseScrollHandler = [&](auto event) {
		if(isActive())
			camera->onMouseScrolled(event->getYOffset());
		return isActive();
	};

	handler.mouseClickHandler = [&](auto event) {
		mouseClicked = true;
		lastPosition = event->position;
		

		return skeleton != nullptr && skeleton->onMouseClick(event);
	};

	handler.mouseRelaseHandler = [&](auto event) {
		mouseClicked = false;
		return false;
	};

	handler.mouseMoveHandler = [&](auto event) {
		if (isActive() && mouseClicked) {
			auto delta = event->position - lastPosition;
			camera->onMouseMove(delta.x, delta.y,false);
			lastPosition = event->position;
		}
		return isActive() && mouseClicked;
	};

	
	handler.keyPressedHandler = [&](auto event) {
		if (isActive()) {
			camera->onKeyDown(event->getKeyCode());
		}
		return false;
	};
	
	app->getEventDispatcher().addEventHandler(&handler);
	sphere.init();
	floor.init();
}

auto ModelLayerBVH::draw(const Timestep& timestep) -> void
{
	if (skeleton) {
		shader->useProgram();
		shader->setUniform3f("light.direction", 0.0f, -0.1f, 0.0f);
		shader->setUniform3f("viewPos",glm::value_ptr(camera->getPosition()));
		shader->setUniform3f("light.ambient", 0.3f, 0.3f, 0.2f);
		shader->setUniform3f("light.diffuse", 0.7f, 0.7f, 0.7f);
		shader->setUniform3f("light.specular", 1.0f, 1.0f, 1.0f);
		shader->setUniform3f("material.ambient", 0.2f, 0.9f, 0.0f);
		shader->setUniform3f("material.diffuse", 0.2f, 0.9f, 0.0f);
		shader->setUniform3f("material.specular", 0.2f, 0.9f, 0.0f); // specular lighting doesn't have full effect on this object's material
		shader->setUniformValue("material.shininess", 32.0f);

		floor.draw(shader.get(),glm::mat4(1), camera->getViewMatrix(), camera->getProjection());
		skeleton->draw(shader.get(), glm::mat4(1), camera->getViewMatrix(), camera->getProjection());
	}
}

auto ModelLayerBVH::loadModel(const std::string& name) -> void
{
	skeleton = std::make_unique<Skeleton>(name,camera.get(),this);
}
