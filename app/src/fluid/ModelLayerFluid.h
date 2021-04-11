

#pragma once
#include "engine/Layer.h"
#include "event/EventHandler.h"
#include "engine/Camera.h"
#include "opengl/BatchCommand.h"
#include "engine/RenderObject.h"
#include <string>
#include <imgui.h>
#include <memory>

#include "FluidParticle.h"
#include "Grid.h"
#include "SmoothingKernel.h"


class ModelLayerFluid : public Layer
{
public:
	friend class ImLayer;
	virtual auto init() -> void override;
	virtual auto draw(const Timestep& timestep) -> void override;
	auto onImGui() -> void override;
	auto onImGuiEnd() -> void override;
	auto start() -> void;
private:
	auto update(const Timestep& timestep) -> void;
	auto initParticles() -> void;
	auto updateGrid() -> void;
	auto drawBox() -> void;
	auto drawParticles() -> void;

	auto findNeighborhoods() -> void;
	auto calculateDensity() -> void;
	auto calculatePressure()-> void;
	auto calculateForceDensity()-> void;


	auto integrationStep(const Timestep & timestep)-> void;
	auto collisionHandling()-> void;


	auto updateConfig() -> void;

	std::unique_ptr<opengl::Shader> shader;
	std::unique_ptr <Camera> camera;
	std::unique_ptr<BatchCommand> linesCommand;
	std::unique_ptr<BatchCommand> pointCommand;
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 model;
	glm::vec3 cameraPos;



	EventHandler handler;

	bool mouseClicked = false;
	glm::vec2 lastPosition;


	struct{
		glm::vec2 box[2];
		int32_t numberParticles = 30;
		//some constants......
		float stiffness = 5.f;
		float viscosity = 12.f;
		float tension = 10.f;
		float gravity = 99.80f;
		float surfaceThreshold = 7.065;
	

		float restDensity = 1000.f;
		float spacing = 1.f / numberParticles;
		float kernelRange = 2.f * spacing;

		float volume = spacing * spacing;
		float mass = volume * restDensity;


		bool spikyWithPressure = false;
		bool calViscosity = false;
		bool calSurface = false;
		
	}fluidConfig;


	Grid grid;
	int32_t totalParticles = 0;

	std::vector < std::shared_ptr<FluidParticle>> particles;
	std::vector<std::vector<int32_t>> neighborhoods;

	bool pause = false;
}; 