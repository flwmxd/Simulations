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

#pragma once
#include "engine/ImLayer.h"
#include "engine/RenderObject.h"
#include "BatchCommand.h"
#include <vector>
#include <memory>
#include "Shader.h"


class Recorder;
class Renderer 
{
public:
	auto init() -> void;
	auto draw(const Timestep& step) -> void;
	auto onImGui(const Timestep& step) -> void;
	auto flush() -> void;
	auto addLayer(Layer* layer)-> void;
	inline auto& getImLayer() { return layer; }
	auto setWireFrame(bool b)-> void;
	auto captureScreen(bool b) -> void;
	auto endRecord() -> void;

	inline auto isRecording() const { return recording; };

private:
	std::vector<Layer*> layers;
	ImLayer layer;
	std::unique_ptr<BatchCommand> linesCommand;
	std::unique_ptr<BatchCommand> pointCommand;
	std::shared_ptr<Recorder> recorder;
	bool recording = false;
};