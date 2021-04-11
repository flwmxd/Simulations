
#include "ModelLayer.h"
#include "Application.h"
auto ModelLayer::init() -> void
{
	Layer::init();

	shader = std::make_unique<opengl::Shader>("shader/vertShader.glsl", "shader/fragShader.glsl");
	shader->compile();

	lightAmbient = glm::vec4(0.1, 0.1, 0.1, 0.5);
	lightDiffuse = glm::vec4(0.8, 1.0, 1.0, 1.0);
	lightSpecular = glm::vec4(0.8, 1.0, 1.0, 1.0);
	


	lightPosition = glm::vec4(-20, -10, 0, 0);
	shader->setUniform4f("lightPos", glm::value_ptr(lightPosition));

	handler.mouseClickHandler = [&](auto event) {
		if (!active) {
			return false;
		}
		for (auto & model : models)
		{
			if (model->onMouseClick(event)) { return true; }
		}
		if (event->buttonId == 0) {
			mouseDown = true;
			xdiff = event->position.x - yrot;
			ydiff = -event->position.y + xrot;
		}
		return false;
	};
	handler.mouseRelaseHandler = [&](auto event) {
		if (!active) {
			return false;
		}
		for (auto& model : models)
		{
			if (model->onMouseRelease(event)) { return true; }
		}
		if (event->buttonId == 0) {
			mouseDown = false;
		}
		return false;
	};
	handler.mouseMoveHandler = [&](auto event) {
		if (!active) {
			return false;
		}
		for (auto& model : models)
		{
			if (model->onMouseMove(event)) { return true; }
		}
		if (mouseDown) {
			yrot = (event->position.x - xdiff) / 10;
			xrot = (event->position.y + ydiff) / 10;
		}
		return false;
	};


	handler.mouseScrollHandler = [&](auto event) {
		if (isActive())
			camera->onMouseScrolled(event->getYOffset());
			return isActive();
	};

	pointCommand = std::make_unique<BatchCommand>(BatchCommand::Type::POINTS);
	linesCommand = std::make_unique<BatchCommand>(BatchCommand::Type::LINES);
	camera = std::make_unique<Camera>(glm::perspective(45.0f, 4.0f / 3.0f, 0.0f, 100.0f), glm::vec3(0, 0, 3));

	app->getEventDispatcher().addEventHandler(&handler);

}

auto ModelLayer::draw(const Timestep& timestep) -> void
{
	model = glm::mat4(1.0f);
	glm::mat4 xRotMat = glm::rotate(glm::mat4(1.0f), xrot, glm::normalize(glm::vec3(glm::inverse(model) * glm::vec4(1, 0, 0, 1))));
	model = model * xRotMat;
	glm::mat4 yRotMat = glm::rotate(glm::mat4(1.0f), yrot, glm::normalize(glm::vec3(glm::inverse(model) * glm::vec4(0, 1, 0, 1))));
	model = model * yRotMat;
	shader->useProgram();
	shader->setUniform4f("lightPos", glm::value_ptr(lightPosition));
	shader->setUniform4f("light_ambient", glm::value_ptr(lightAmbient));
	shader->setUniform4f("light_diffuse", glm::value_ptr(lightDiffuse));
	shader->setUniform4f("light_specular", glm::value_ptr(lightSpecular));

	for (auto& m : models)
	{
		m->draw(shader.get(), model, camera->getViewMatrix(), camera->getProjection());
	}

	flatShader->useProgram();
	flatShader->setUniformMatrix4f("model", glm::value_ptr(model));
	flatShader->setUniformMatrix4f("view", glm::value_ptr(camera->getViewMatrix()));
	flatShader->setUniformMatrix4f("projection", glm::value_ptr(camera->getProjection()));
	for (auto& m : models)
	{
		m->draw(flatShader.get(), model, camera->getViewMatrix(), camera->getProjection(), *pointCommand);
	}

	for (auto& m : lines)
	{
		m->draw(flatShader.get(), model, camera->getViewMatrix(), camera->getProjection(), *linesCommand);
	}
	linesCommand->flush(*flatShader);
	pointCommand->flush(*flatShader);
}

auto ModelLayer::loadModel(const std::string & name) -> void
{
	models.clear();
	lines.clear();

	auto model = std::make_shared<Model>(name);
	model->setOrder(3,this);
	
	models.emplace_back(model);
}


