#include "MathTools.h"

auto MathTools::pseudoInverse(const glm::mat4& jacobian) -> glm::mat4
{
	auto jjtInv = (jacobian * glm::transpose(jacobian));
	jjtInv = glm::inverse(jjtInv);
	return (glm::transpose(jacobian) * jjtInv);
}
