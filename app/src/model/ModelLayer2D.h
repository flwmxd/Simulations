


#pragma once
#include "engine/Layer.h"
#include "opengl/Shader.h"
#include "Model.h"
#include "event/EventHandler.h"
#include "FFDPolygon.h"

class ModelLayer2D : public Layer
{
public:
	friend class ImLayer;
	virtual auto init() -> void override;
	virtual auto draw(const Timestep& timestep) -> void override;
	auto loadModel(const std::string & name) -> void;
	auto addModel(const std::shared_ptr<RenderObject>& model,bool batch = false) -> void;
	auto setCell(int32_t cell) -> void;
	auto setTriangleCage(int32_t vertexCount) -> void;
	inline auto& getLines() { return lines; }
private:
	EventHandler handler;
	std::vector<std::shared_ptr<RenderObject>> points;
	std::vector<RenderObject * > lines;
	std::vector<std::shared_ptr<RenderObject>> tranglesModels;

	std::unique_ptr<BatchCommand> pointCommand;
	std::unique_ptr<BatchCommand> linesCommand;
	std::unique_ptr<BatchCommand> tranglesCommand;

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	int32_t vertexCount = 8;

	int32_t cell = 3;
	bool mouseDown = false;

	FFDPolygon polygon;
};