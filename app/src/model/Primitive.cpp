#include "Primitive.h"
#include "opengl/OGL.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

auto Primitive::init() -> void
{
	vertexArray = std::make_unique<VertexArray>();
}


auto Primitive::draw(opengl::Shader* shader, const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) -> void
{

	shader->useProgram();
	shader->setUniformMatrix4f("model", glm::value_ptr(m));
	shader->setUniformMatrix4f("view", glm::value_ptr(v));
	shader->setUniformMatrix4f("projection", glm::value_ptr(p));
	vertexArray->bind();

	uint32_t count = vertexArray->getVertexBuffers().size();
	if (count > 0) 
	{
		auto& vb = vertexArray->getVertexBuffers()[0];
		
		glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vb->getSize() ));
	}

	vertexArray->unbind();
	CHECK_GL_ERROR();
}
