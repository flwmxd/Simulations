

#pragma once

#include <glm/glm.hpp>
#include <bvh11.hpp>
class IkEffector;

class InverseKinematics 
{
public:
	friend class ImLayer;
	InverseKinematics(bvh11::BvhObject* object);
	enum class Jacobian
	{
		PseudoInverse,
		Damped
	};
	enum class Axis
	{
		X,
		Y,
		Z
	};

	auto bindJoint(std::shared_ptr<bvh11::Joint> j,bool withTarget = true,int32_t frame = 0) -> void;
	auto update (uint32_t frame) -> void;
	auto compute(const glm::vec3& delta, uint32_t frame) -> bool;
	auto getJacobianMatrix(int32_t frame,const glm::vec3 & endPos, const glm::vec3& targetPosition)->Eigen::MatrixXd;
	inline auto getEffector() { return effctor; }
	auto beginDrag(const glm::vec3& delta, const std::shared_ptr<bvh11::Joint>& joint, int32_t frame) -> void;
	auto getTranslation(std::shared_ptr<bvh11::Joint> joint, int32_t frame)->glm::vec3;
	auto getWorldRotation(std::shared_ptr<bvh11::Joint> joint, int32_t frame)->glm::mat3;
	auto getLocalRotation(std::shared_ptr<bvh11::Joint> joint, int32_t frame)->glm::mat3;
	auto getLocalEulerAngle(std::shared_ptr<bvh11::Joint> joint,int32_t frame)->glm::vec3;
private:
	auto toQuaternion(float yaw, float pitch, float roll)->glm::quat;// yaw (Z), pitch (Y), roll (X)
	auto getJacobianPseudoInverse(const  Eigen::MatrixXd& jac)->Eigen::MatrixXd;
	auto getJacobianPseudoInverseWithDamped(const  Eigen::MatrixXd& jac)->Eigen::MatrixXd;
	auto solveIK(int32_t frame, const glm::vec3 & position, const glm::vec3& delta)->Eigen::MatrixXd;
	auto apply(const Eigen::VectorXd & vector,uint32_t frame) -> void;

	bool lockRoot = false;
	Jacobian method = Jacobian::PseudoInverse;
	std::vector<std::shared_ptr<bvh11::Joint>> joints;
	IkEffector * effctor = nullptr;
	bvh11::BvhObject* object = nullptr;

	std::vector<float> angles;
	float tol = 0.05;
	float delta = 0.01;
	float damping = 0.1;
	bool withTarget = true;
	int32_t maxIter = 1000;
	int32_t currentIter = 0;
	glm::vec3 desiredEndPosition = {};
private:
	std::shared_ptr<bvh11::Joint>  joint;
};