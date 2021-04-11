

#pragma once
#include "glm/glm.hpp"

struct Light
{
	glm::vec3 lightDirection;
	glm::vec3 lightAmbient;
	glm::vec3 lightDiffuse;
	glm::vec3 lightSpecular;
};