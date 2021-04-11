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
#include <cassert>
#include "OGL.h"
#include "Shader.h"
#include <iostream>



namespace opengl
{
	Shader::Shader(const std::string & vN, const std::string & fN)
		:vertexName(vN),fragmentName(fN)
	{
		vertexFile = std::make_unique<filesystem::File>(vertexName);
		fragmentFile = std::make_unique<filesystem::File>(fragmentName);
	}

	auto Shader::useProgram() -> void
	{
		glUseProgram(programId);
		CHECK_GL_ERROR();
	}

	auto Shader::createProgram() -> void
	{
		programId = glCreateProgram();
		glAttachShader(programId, vs);
		glAttachShader(programId, fs);
		glLinkProgram(programId);
		GLint result = 0;
		glGetProgramiv(programId, GL_LINK_STATUS, &result);
		char infoLog[512];
		if (!result)
		{
			glGetProgramInfoLog(programId, 512, NULL, infoLog);
			throw new std::logic_error(infoLog);
		}
	}

	auto Shader::compile() -> void
	{
		auto vBuffer = vertexFile->getBuffer();
		auto fBuffer = fragmentFile->getBuffer();
		const uint32_t size1 = vertexFile->getFileSize();
		const uint32_t size2 = fragmentFile->getFileSize();
		createShader(vBuffer.get(), Type::VERTEX,&size1);
		createShader(fBuffer.get(), Type::FRAGMENT,&size2);
		createProgram();
		CHECK_GL_ERROR();
	}

	auto Shader::compile(const char * vb,const uint32_t * vbLen,const char * fb,const uint32_t * fbLen) -> void
	{
		createShader((const int8_t *)vb, Type::VERTEX,vbLen);
		createShader((const int8_t *)fb, Type::FRAGMENT,fbLen);
		createProgram();
		CHECK_GL_ERROR();
	}

	auto Shader::setUniformValue(const std::string & name, bool value) const -> void
	{
		glUniform1i(glGetUniformLocation(programId, name.c_str()), (int)value);
	}

	auto Shader::setUniformValue(const std::string & name, int32_t value) const -> void
	{
		glUniform1i(glGetUniformLocation(programId, name.c_str()), value);
	}

	auto Shader::setUniformValue(const std::string & name, float value) const -> void
	{
		glUniform1f(glGetUniformLocation(programId, name.c_str()), value);
	}

	auto Shader::setUniformValue(const std::string & name, int32_t value, int32_t value2) const -> void
	{
		glUniform2f(glGetUniformLocation(programId, name.c_str()), (GLfloat)value, (GLfloat)value2);
	}

	auto Shader::setUniform4f(const std::string& name, const float* value) const -> void
	{
		glUniform4fv(glGetUniformLocation(programId, name.c_str()), 1, value);
	}

	auto Shader::setUniform2f(const std::string& name, float value, float value2) const -> void
	{
		glUniform2f(glGetUniformLocation(programId, name.c_str()), (GLfloat)value, (GLfloat)value2);
	}

	auto Shader::setUniform2f(uint32_t index, float value, float value2) const -> void
	{
		glUniform2f(index, (GLfloat)value, (GLfloat)value2);
	}

	auto Shader::setUniform3f(const std::string & name, float value, float value2, float value3) const -> void
	{
		glUniform3f(glGetUniformLocation(programId, name.c_str()), (GLfloat)value, (GLfloat)value2, (GLfloat)value3);
	}

	auto Shader::setUniform3f(const std::string& name, const float* value) const -> void
	{
		glUniform3f(glGetUniformLocation(programId, name.c_str()),value[0],value[1],value[2]);
	}

	auto Shader::setUniform4f(const std::string& name, float value, float value2, float value3, float value4) const -> void
	{
		glUniform4f(glGetUniformLocation(programId, name.c_str()), (GLfloat)value, (GLfloat)value2, (GLfloat)value3, (GLfloat)value4);
	}

	auto Shader::getUniformPosition(const std::string & name) const -> int32_t
	{
		return glGetUniformLocation(programId, name.c_str());
	}

	auto Shader::getAttribLocation(const std::string & name) const->int32_t
	{
		return glGetAttribLocation(programId, name.c_str());
	}

	auto Shader::setUniformMatrix4f(const std::string& name, const float* data) const -> void
	{
		glUniformMatrix4fv(getUniformPosition(name), 1, false, data);
	}

	auto Shader::setUniformMatrix3f(const std::string& name, const float* data) const -> void
	{
		glUniformMatrix3fv(getUniformPosition(name), 1, false, data);
	}

/*
	auto Shader::enableVertexAttribArray() -> void
	{
		for (auto & attr : attributes)
		{
			glEnableVertexAttribArray(attr.pos);

			auto type = GL_FLOAT;
			auto inc = 4;
			switch (attr.type)
			{
			case AttributeType::TYPE_INT8:
				type = GL_BYTE;
				inc = sizeof(GLbyte);
				break;
			case AttributeType::TYPE_INT16:
				type = GL_SHORT;
				inc = sizeof(GLshort);
				break;
			case AttributeType::TYPE_INT32:
				type = GL_INT;
				inc = sizeof(GLint);
				break;
			case AttributeType::TYPE_UINT8:
				type = GL_UNSIGNED_BYTE;
				inc = sizeof(GLubyte);
				break;
			case AttributeType::TYPE_UINT16:
				type = GL_UNSIGNED_SHORT;
				inc = sizeof(GLushort);
				break;
			case AttributeType::TYPE_UINT32:
				type = GL_UNSIGNED_INT;
				inc = sizeof(GLuint);
				break;
			case AttributeType::TYPE_FLOAT:
				type = GL_FLOAT;
				inc = sizeof(GLfloat);
				break;

			}
			glVertexAttribPointer(attr.pos, attr.size, type, GL_FALSE, sizeof(Vertex), (void*)attr.offset);
		}
	}

	auto Shader::disableVertexAttribArray() -> void
	{
		for (auto& attr : attributes)
		{
			glDisableVertexAttribArray(attr.pos);
		}
	}

	auto Shader::setAttributes(const std::vector<Attribute> & attrs) -> void
	{
		auto offset = 0;
		this->attributes = attrs;
		for (auto & a : attributes)
		{
			auto pos = getAttribLocation(a.name);
			a.pos = pos;
			glEnableVertexAttribArray(pos);

			auto type = GL_FLOAT;
			auto inc = 4;
			switch (a.type)
			{
				case AttributeType::TYPE_INT8 : 
					type = GL_BYTE;
					inc = sizeof(GLbyte);
				break;
				case AttributeType::TYPE_INT16: 
					type = GL_SHORT;
					inc = sizeof(GLshort);
				break;
				case AttributeType::TYPE_INT32: 
					type = GL_INT;
					inc = sizeof(GLint);
				break;
				case AttributeType::TYPE_UINT8: 
					type = GL_UNSIGNED_BYTE;
					inc = sizeof(GLubyte);
				break;
				case AttributeType::TYPE_UINT16:
					type = GL_UNSIGNED_SHORT;
					inc = sizeof(GLushort);
				break;
				case AttributeType::TYPE_UINT32: 
					type = GL_UNSIGNED_INT;
					inc = sizeof(GLuint);
				break;
				case AttributeType::TYPE_FLOAT:
					type = GL_FLOAT;
					inc = sizeof(GLfloat);
				break;

			}	
			a.offset = offset;
			glVertexAttribPointer(pos, a.size, type, GL_FALSE, sizeof(Vertex), (void*) offset);
			offset += a.size * inc;
		}
		CHECK_GL_ERROR();
	}	
*/


	auto Shader::createShader(const int8_t * buffer, Type type, const uint32_t * length) -> void
	{
		uint32_t shaderId = glCreateShader(type == Type::VERTEX ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
		glShaderSource(shaderId, 1, (const GLchar**)&buffer, (const GLint *)length);
		glCompileShader(shaderId);
		GLint result;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
		if (!result)
		{
			CHECK_GL_ERROR();

			GLint logLen;
			glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLen);
			if (logLen > 0)
			{
				const char * typeChar = type == Type::VERTEX ? "Vertex" : "Fragment";
				char *log = new char[logLen];
				GLsizei written;
				glGetShaderInfoLog(shaderId, logLen, &written, log);
				char buf[512] = {};
				sprintf(buf, "shader log: %s\n", log);
				delete[] log;
				std::cout << buf<<std::endl;
				throw std::logic_error(buf);
			}
		}
		if (type == Type::VERTEX) 
		{
			vs = shaderId;
		}
		else 
		{
			fs = shaderId;
		}
	}
};

