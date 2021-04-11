//////////////////////////////////////////////////////////////////////////////
// This file is part of the FFD Course work									//
// Author Prime Zeng														//
//////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "Application.h"
#include "editor/Editor.h"
#include "model/Point.h"
#include "opengl/Shader.h"

std::unique_ptr<Application> app;

int32_t main()
{
	app = std::make_unique<Application>();
	app->run();
	return 0;
}

auto Application::run() -> void
{
	init();
	double lastFrameTime = 0;
	while (running)
	{
		Timestep timestep = timer.stop() / 1000000.f;
		draw(timestep);
		

		window.update();
		eventDispatcher.dispatchEvents();

		lastFrameTime += timestep;
		if (lastFrameTime - secondTimer > 1.0f)
		{
			secondTimer += 1.0f;
			frames = 0;
			updates = 0;
		}
	}
}

auto Application::draw(const Timestep & timestep) -> void
{
	renderer.draw(timestep);
	editor->onImGui();
	renderer.flush();
}

auto Application::init() -> void
{
	window.init(800, 600);
	editor = std::make_unique<Editor>(window.getWidth(),window.getHeight());

	editor->init();

	renderer.init();

	layerBVH.init();
	layerCloth.init();
	layerFluid.init();

	layer.init();
	layer.loadModel("models/400.obj");

	layer2D.init();
	layer2D.loadModel("");
	layer2D.setCell(1);


	renderer.getImLayer().setModelLayerFluid(&layerFluid);
	renderer.getImLayer().setModelLayerCloth(&layerCloth);
	renderer.getImLayer().setModelLayerBVH(&layerBVH);
	renderer.getImLayer().setModelLayer(&layer);
	renderer.getImLayer().setModelLayer2D(&layer2D);


	renderer.addLayer(&layerFluid);
	renderer.addLayer(&layerBVH);
	renderer.addLayer(&layer);
	renderer.addLayer(&layer2D);
	renderer.addLayer(&layerCloth);

	glPointSize(20);
}

