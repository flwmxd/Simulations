#include "RenderObject.h"
#include "Application.h"




auto RenderObject::draw(opengl::Shader* shader, const glm::mat4& p) -> void
{

}

auto RenderObject::convertWorldToNormalized(const glm::mat4& modelView, const glm::mat4& projection, const glm::vec3& point) ->glm::vec3
{
	auto translatedPosition = glm::vec4{ point,1 } *modelView;

	return glm::project(point, modelView, projection, glm::vec4{ 0,0,app->getWindow().getWidth() ,app->getWindow().getHeight() });

}

auto RenderObject::convertNormalizedToWorld(const glm::mat4& modelView, const glm::mat4& projection, const glm::vec3& normalizedPosition) ->glm::vec4
{
	auto result = glm::vec4{
		(normalizedPosition.x / app->getWindow().getWidth() - 0.5F) * 2.0F,
						((1.0F - normalizedPosition.y) / app->getWindow().getHeight() - 0.5F) * 2.0F,
						0.0F,
						1
	};
	auto invert = glm::inverse(projection * modelView);
	return result * invert;
}