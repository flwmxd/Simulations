
#include "ModelLayer2D.h"
#include "Application.h"
#include "FFDPolygon.h"

auto ModelLayer2D::init() -> void
{
	
	Layer::init();
	
	projection = glm::ortho(-1.0, 1.0, -1.0, 1.0, -100.0, 100.0);

	// Camera matrix
	view = glm::lookAt(glm::vec3(0, 0, 3), // Camera position in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	handler.mouseClickHandler = [&](auto event) {
		for (auto & model : tranglesModels)
		{
			if (model->onMouseClick(event)) { return true; }
		}
		if (event->buttonId == 0) {
			mouseDown = true;
		}
		return false;
	};
	handler.mouseRelaseHandler = [&](auto event) {
		for (auto& model : tranglesModels)
		{
			if (model->onMouseRelease(event)) { return true; }
		}
		if (event->buttonId == 0) {
			mouseDown = false;
		}
		return false;
	};
	handler.mouseMoveHandler = [&](auto event) {
		for (auto& model : tranglesModels)
		{
			if (model->onMouseMove(event)) { return true; }
		}
		return false;
	};

	pointCommand = std::make_unique<BatchCommand>(BatchCommand::Type::POINTS);
	linesCommand = std::make_unique<BatchCommand>(BatchCommand::Type::LINES);
	tranglesCommand = std::make_unique<BatchCommand>(BatchCommand::Type::TRIANGLES);

	app->getEventDispatcher().addEventHandler(&handler);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	tranglesModels.emplace_back(&polygon);
	app->getRenderer().getImLayer().setPolygon(&polygon);
	polygon.setParent(this);
	
}

auto ModelLayer2D::draw(const Timestep& timestep) -> void
{
	flatShader->useProgram();
	flatShader->setUniformMatrix4f("model", glm::value_ptr(model));
	flatShader->setUniformMatrix4f("view", glm::value_ptr(view));
	flatShader->setUniformMatrix4f("projection", glm::value_ptr(projection));
	for (auto& m : points)
	{
		m->draw(flatShader.get(), model, view, projection, *pointCommand);
	}

	
	for (auto & m : tranglesModels)
	{
		m->draw(flatShader.get(), model, view, projection, *tranglesCommand);
		m->draw(flatShader.get(), model, view, projection, *pointCommand);
	}

	for (auto& m : lines)
	{
		m->draw(flatShader.get(), model, view, projection, *linesCommand);
	}


	linesCommand->flush(*flatShader);
	pointCommand->flush(*flatShader);

	if(polygon.isActive())
		tranglesCommand->flush(*flatShader);

}

auto ModelLayer2D::loadModel(const std::string& name) -> void
{
	if (name == "") 
	{
		polygon.generateRandom();
	}
	else 
	{
		polygon.loadFile(name);
	}

}

auto ModelLayer2D::addModel(const std::shared_ptr<RenderObject>& model, bool batch) -> void
{

}

auto ModelLayer2D::setCell(int32_t cell) -> void
{
	lines.clear();
	this->cell = cell;
	polygon.setCell(cell);
	

	
}

auto ModelLayer2D::setTriangleCage(int32_t vertexCount) -> void
{
	this->vertexCount = vertexCount;
	lines.clear();
	polygon.genTriangleCage(vertexCount,lines);

}

