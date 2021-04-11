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

#include "Framebuffer.h"
#include "OGL.h"
#include <stdexcept>

static const uint32_t MAX_FRAME_BUFFER_SIZE = 8192;


Framebuffer::Framebuffer(const FramebufferArgs& initArgs)
	:args(initArgs)
{
	invalidate();
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &bufferId);
	glDeleteTextures(1, &colorAttachment);
	glDeleteTextures(1, &depthAttachment);
}

auto Framebuffer::bind() -> void 
{
	glBindFramebuffer(GL_FRAMEBUFFER, bufferId);
	glViewport(0, 0, args.width, args.height);
}

auto Framebuffer::unbind() -> void 
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

auto Framebuffer::resize(uint32_t width, uint32_t height) -> void 
{
	if (width == 0 || height == 0 || width > MAX_FRAME_BUFFER_SIZE || height > MAX_FRAME_BUFFER_SIZE)
	{
		return;
	}
	args.width = width;
	args.height = height;

	invalidate();
}

auto Framebuffer::invalidate() -> void
{
	if (bufferId)
	{
		glDeleteFramebuffers(1, &bufferId);
		glDeleteTextures(1, &colorAttachment);
		glDeleteTextures(1, &depthAttachment);
	}

	glGenFramebuffers(1, &bufferId);
	//glCreateFramebuffers(1, &m_RendererID);
	glBindFramebuffer(GL_FRAMEBUFFER, bufferId);

	glGenTextures(1, &colorAttachment);
	glBindTexture(GL_TEXTURE_2D, colorAttachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, args.width, args.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment, 0);

	glGenTextures(1, &depthAttachment);
	glBindTexture(GL_TEXTURE_2D, depthAttachment);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, args.width, args.height);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthAttachment, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
	{
		throw std::runtime_error("Framebuffer is incomplete!");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

