//////////////////////////////////////////////////////////////////////////////
// This file is part of the FFD Course work									//
// Author Prime Zeng														//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <thread>
#include <mutex>
#include <future>
#include <condition_variable>
#include <unordered_set>
#include <vector>
#include "opengl/Shader.h"
#include "libobj/libobj.h"
#include "opengl/BatchCommand.h"
#include "opengl/Renderer.h"
#include "event/EventDispatcher.h"
#include "engine/Timer.h"
#include "engine/Window.h"
#include "model/ModelLayer.h"
#include "model/ModelLayer2D.h"
#include "model/ModelLayerBVH.h"
#include "model/ModelLayerCloth.h"
#include "fluid/ModelLayerFluid.h"

class Editor;

class Application final 
{
public:
	auto run() -> void;
	auto init() -> void;
	auto draw(const Timestep& timestep) -> void;

	inline auto& getEventDispatcher() { return eventDispatcher; }
	inline auto& getRenderer() { return renderer; }
	inline auto& getWindow() { return window; }
	inline auto& getTimer() { return timer; }
	inline auto& getEditor() { return editor; }

private:
	ModelLayer layer;
	ModelLayer2D layer2D;
	ModelLayerBVH layerBVH;
	ModelLayerCloth layerCloth;
	ModelLayerFluid layerFluid;

	Timer timer;

	Renderer renderer;
	EventDispatcher eventDispatcher;
	Window window;
	bool running = true;

	uint64_t updates = 0;
	uint64_t frames = 0;
	float secondTimer = 0.0f;


	std::unique_ptr<Editor> editor;
};
extern std::unique_ptr<Application> app;


