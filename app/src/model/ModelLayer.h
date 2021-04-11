


#pragma once
#include "engine/Layer.h"
#include "opengl/Shader.h"
#include "Model.h"
#include "event/EventHandler.h"
#include "engine/Camera.h"


class ModelLayer : public Layer
{
public:
	friend class ImLayer;
	virtual auto init() -> void override;
	virtual auto draw(const Timestep& timestep) -> void override;
	auto loadModel(const std::string & name) -> void;
	inline auto& getLines() { return lines; }
private:
	EventHandler handler;


	std::unique_ptr<opengl::Shader> shader;
	std::unique_ptr<Camera> camera;

	std::vector<std::shared_ptr<RenderObject>> models;
	std::vector<RenderObject*> lines;
	std::unique_ptr<BatchCommand> pointCommand;
	std::unique_ptr<BatchCommand> linesCommand;


	glm::mat4 model = glm::mat4(1.0f);

	float xrot = 0;
	float yrot = 0;
	float xdiff = 0;
	float ydiff = 0;
	bool mouseDown = false;


	glm::vec4 lightPosition;

	glm::vec4 lightAmbient;
	glm::vec4 lightDiffuse;
	glm::vec4 lightSpecular;

	Model* m = nullptr;

	
	

};