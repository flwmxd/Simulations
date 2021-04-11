//////////////////////////////////////////////////////////////////////////////
// This file is part of the PharaohStroy MMORPG client                      //
// Copyright ?2020-2022 Prime Zeng                                          //
//                                                                          //
// This program is free software: you can redistribute it and/or modify     //
// it under the terms of the GNU Affero General Public License as           //
// published by the Free Software Foundation, either version 3 of the       //
// License, or (at your option) any later version.                          //
//                                                                          //
// This program is distributed in the hope that it will be useful,          //
// but WITHOUT ANY WARRANTY; without even the implied warranty of           //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
// GNU Affero General Public License for more details.                      //
//                                                                          //
// You should have received a copy of the GNU Affero General Public License //
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    //
//////////////////////////////////////////////////////////////////////////////

#include "Renderer.h"
#include "OGL.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Application.h"
#include "tools/Recorder.h"



auto Renderer::init() -> void
{
	layer.init();
	linesCommand = std::make_unique<BatchCommand>(BatchCommand::Type::LINES);
	pointCommand = std::make_unique<BatchCommand>(BatchCommand::Type::POINTS);

	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	//glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);    // Make round points, not square points
	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);     // Antialias the lines
	glEnable(GL_BLEND);
	CHECK_GL_ERROR();
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	CHECK_GL_ERROR();
	 
	
}

auto Renderer::draw(const Timestep& step) -> void
{
	if (recorder && recording)
	{
		recorder->getFrame();
	}
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (auto layer : layers)
	{ 
		if(layer->isActive())
			layer->draw(step);
	}
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	onImGui(step);
}

auto Renderer::onImGui(const Timestep& step) -> void
{
	layer.draw(step);
}

auto Renderer::flush() -> void
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

auto Renderer::addLayer(Layer* layer) -> void
{
	layers.emplace_back(layer);
}

auto Renderer::setWireFrame(bool b) -> void
{
	if (b) 
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else 
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

auto Renderer::captureScreen(bool b) -> void
{
	recording = b;
	if(recording)
	{
		recorder = std::make_shared<Recorder>("output.mp4", "avc1", 20, 
			app->getWindow().getWidth(), 
			app->getWindow().getHeight());
	}
	else 
	{
		if (recorder) 
		{
			recorder->end();
			recorder = nullptr;
		}
	}
}

auto Renderer::endRecord() -> void
{
	recording = false;
	
}
