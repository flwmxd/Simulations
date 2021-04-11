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
#include <string>
#include <map>

#include "Attribute.h"
#include "FileSystem/File.h"

namespace opengl 
{
	class Shader
	{

	public:
		enum class Type
		{
			VERTEX,
			FRAGMENT,
		};


		Shader() = default;
		Shader(const std::string & vertexName, const std::string & fragmentName);
		auto useProgram() -> void;
	
		auto compile() -> void;
		auto compile(const char * vb,const uint32_t * vbLen,const char * fb,const uint32_t * fbLen) -> void;
		//setUniformValue
		auto setUniformValue(const std::string & name, bool value) const -> void;
		auto setUniformValue(const std::string & name, int32_t value) const -> void;
		auto setUniformValue(const std::string & name, int32_t value,int32_t value2) const -> void;
		auto setUniformValue(const std::string & name, float value) const -> void;

		auto setUniform2f(const std::string & name, float, float) const -> void;
		auto setUniform3f(const std::string & name, float, float, float) const -> void;
		auto setUniform3f(const std::string& name, const float * value) const -> void;

		auto setUniform4f(const std::string & name, float, float, float, float) const -> void;
		auto setUniform4f(const std::string& name, const float * value) const -> void;

		auto setUniform2f(uint32_t index, float, float) const -> void;
		auto getUniformPosition(const std::string & name) const->int32_t;
		auto setUniformMatrix4f(const std::string& name, const float* data)const -> void;
		auto setUniformMatrix3f(const std::string& name, const float* data)const -> void;
		auto setAttributes(const std::vector<Attribute> & attributes) -> void;
		auto getAttribLocation(const std::string & name) const->int32_t;
		auto enableVertexAttribArray() -> void;
		auto disableVertexAttribArray() -> void;
		inline auto getProgramId() const  { return programId; };

	private:
		
		auto createShader(const int8_t * buffer, Type type,const uint32_t * length = nullptr) -> void;//1.����shader;
		auto createProgram() -> void;//2. ��������;

		uint32_t programId = 0;
		std::string vertexName;
		std::string fragmentName;
		//
		uint32_t vs = 0;
		uint32_t fs = 0;

		std::unique_ptr<filesystem::File> vertexFile;
		std::unique_ptr<filesystem::File> fragmentFile;
		std::vector<Attribute> attributes;

	};
};
