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
#include <cstdint>

struct FramebufferArgs
{
	uint32_t width, height;
	uint32_t samples = 1;
	bool swapChainTarget = false;
};

class Framebuffer
{
public:
	Framebuffer(const FramebufferArgs& initArgs);
	virtual ~Framebuffer();
	virtual auto bind() -> void ;
	virtual auto unbind() -> void ;
	virtual auto resize(uint32_t width, uint32_t height) -> void ;

	inline auto getColorAttachmentRendererID() const { return colorAttachment; };
	inline auto& getArgs() const { return args; };
	auto invalidate() -> void;
private:
	FramebufferArgs args;
	uint32_t bufferId = 0;
	uint32_t colorAttachment = 0;
	uint32_t depthAttachment = 0;
};

