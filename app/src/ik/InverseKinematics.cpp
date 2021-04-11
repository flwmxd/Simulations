
#include "InverseKinematics.h"
#include "IkEffector.h"


#ifndef M_PI
#define M_PI 3.14159265f
#endif
#include <glm/gtx/transform.hpp>
#include <glm/gtx/vector_angle.hpp>

InverseKinematics::InverseKinematics(bvh11::BvhObject * object)
	:object(object)
{


}

auto InverseKinematics::bindJoint(std::shared_ptr<bvh11::Joint> j, bool withTarget, int32_t frame) -> void
{
	joints.clear();
	joint = j;
	this->withTarget = withTarget;
	auto parent = joint->parent();
	//if there is a target (end effector), the current joint will be changed.

	joints.emplace_back(j);
	
	while (parent != nullptr) 
	{
		if (parent->name() == "Hips" && lockRoot) {
			break;;
		}
		joints.emplace(joints.begin(),parent);
		parent = parent->parent();
	}
	if (effctor != nullptr)
	{
		delete effctor;
		effctor = nullptr;
	}
	if (withTarget) 
	{
		effctor = new IkEffector(getTranslation(joint, frame));
	}

}

auto InverseKinematics::update(uint32_t frame) -> void
{
	if (joint != nullptr && withTarget) 
	{
		glm::vec3 start = getTranslation(joint, frame);
		glm::vec3 delta = effctor->getPosition()  - start;
		auto distance = glm::distance(start, effctor->getPosition());
		if (distance > tol)
		{
			auto result = solveIK(frame,start, delta);
			apply(result, frame);
		}
	}
}

auto InverseKinematics::compute(const glm::vec3& delta, uint32_t frame) -> bool
{
	if (joint != nullptr && !withTarget)
	{
		glm::vec3 end = getTranslation(joint, frame);
		auto pseudo = solveIK(frame,end, delta);
		auto distance = glm::distance(end, desiredEndPosition);
		if (distance > tol) 
		{
			apply(pseudo, frame);
		}
	}
	return true;
}

auto InverseKinematics::getJacobianMatrix(int32_t frame, const glm::vec3& endPos, const glm::vec3& targetPosition) ->Eigen::MatrixXd
{
	 Eigen::MatrixXd jaco(3, joints.size() * 3);
	 jaco.setZero();
	 for (int32_t col = 0; col < joints.size(); col++) {
		 auto current = joints[col];
		 auto currPos = getTranslation(current, frame);
		 auto rotation = getWorldRotation(current, frame);
		 auto delta = endPos - currPos;//dp
		 // Get the x, y, and z axis relative to the base of the current joint
		 auto rx = glm::cross(rotation * (glm::vec3(-1.f, 0.f, 0.f)), delta);
		 auto ry = glm::cross(rotation * (glm::vec3(0.f, -1.f, 0.f)), delta);
		 auto rz = glm::cross(rotation * (glm::vec3(0.f, 0.f, -1.f)), delta);

		 jaco(0, 3 * col) = rx.x;
		 jaco(0, 3 * col + 1) = rx.y;
		 jaco(0, 3 * col + 2) = rx.z;

		 jaco(1, 3 * col) = ry.x;
		 jaco(1, 3 * col + 1) = ry.y;
		 jaco(1, 3 * col + 2) = ry.z;

		 jaco(2, 3 * col) = rz.x;
		 jaco(2, 3 * col + 1) = rz.y;
		 jaco(2, 3 * col + 2) = rz.z;

	 }

	return jaco;
}


auto InverseKinematics::beginDrag(const glm::vec3& delta, const std::shared_ptr<bvh11::Joint>& joint, int32_t frame) -> void
{
	desiredEndPosition = delta + getTranslation(joint, frame);
}

auto InverseKinematics::getTranslation(std::shared_ptr<bvh11::Joint> joint, int32_t frame) ->glm::vec3
{
	auto transform = object->GetTransformation(joint, frame);
	auto currentPosition = transform.translation();
	return glm::vec3{ currentPosition.x(),currentPosition.y(),currentPosition.z() };
}


auto InverseKinematics::getJacobianPseudoInverse(const Eigen::MatrixXd& jaco) -> Eigen::MatrixXd
{
	auto jTrans = jaco.transpose();
	Eigen::MatrixXd transpoe = (jaco * jTrans);
	auto inverse = transpoe.inverse();
	return jTrans * inverse;
}

//θ`= JT(JJT+λ2I)^−1 * V
auto InverseKinematics::getJacobianPseudoInverseWithDamped(const Eigen::MatrixXd& jaco) ->Eigen::MatrixXd
{
	auto jTrans = jaco.transpose();
	Eigen::MatrixXd transpoe = (jaco * jTrans);
	Eigen::MatrixXd matrix(jaco.rows(), jaco.rows());
	matrix.setIdentity();
	matrix *= std::pow(damping, 2);
	transpoe += matrix;
	auto inverse = transpoe.inverse();
	auto md  = jTrans * inverse;
	return md;
}

auto InverseKinematics::solveIK(int32_t frame,const glm::vec3& position, const glm::vec3& delta) ->Eigen::MatrixXd
{
	//V = J * θ
	Eigen::VectorXd deltaV = Eigen::VectorXd::Zero(3, 1);
	deltaV(0) = delta.x;
	deltaV(1) = delta.y;
	deltaV(2) = delta.z;

	auto jaco = getJacobianMatrix(0, position,position);

	Eigen::MatrixXd pseudo = (method == Jacobian::Damped ?getJacobianPseudoInverseWithDamped(jaco) :getJacobianPseudoInverse(jaco))* deltaV;

	return pseudo;
}

auto InverseKinematics::apply(const Eigen::VectorXd & vector,uint32_t frame) -> void
{

	for (int32_t i = 0; i < joints.size(); i++)
	{
		auto x = i * 3;
		auto y = i * 3 + 1;
		auto z = i * 3 + 2;

		for (int32_t j = 0;j<3;j++)
		{
			if (std::isnan(vector(i * 3 + j)) || std::isinf(vector(i * 3 + j))) 
			{
				std::cerr << "solution is nan or inf" << std::endl;
			}
		}

		auto curJoin = joints[i];

		if (curJoin->name() == "Hips" && lockRoot) {
			continue;
		}

		// Apply time-varying transformations
		for (auto channelIndex : curJoin->associated_channels_indices())
		{
			const bvh11::Channel& channel = object->channels()[channelIndex];
			float& value = object->motion()(frame, channelIndex);

			switch (channel.type)
			{
			case bvh11::Channel::Type::x_rotation:
			{
				if (!std::isnan(vector(x)) && !std::isinf(vector(x)))
					value += vector(x);
			}
			break;
			case bvh11::Channel::Type::y_rotation:
			{
				if (!std::isnan(vector(y)) && !std::isinf(vector(y)))
					value += vector(y);//angles[i];
			}
			break;
			case bvh11::Channel::Type::z_rotation:
			{
				if (!std::isnan(vector(z)) && !std::isinf(vector(z)))
					value += vector(z);//angles[i];
			}
			break;
			}
		}
	}
}

//yzx
auto InverseKinematics::toQuaternion(float yaw, float pitch, float roll) -> glm::quat
{
	double cy = std::cos(yaw * 0.5);
	double sy = std::sin(yaw * 0.5);
	double cp = std::cos(pitch * 0.5);
	double sp = std::sin(pitch * 0.5);
	double cr = std::cos(roll * 0.5);
	double sr = std::sin(roll * 0.5);

	glm::quat q;
	q.w = cy * cp * cr + sy * sp * sr;
	q.x = cy * cp * sr - sy * sp * cr;
	q.y = sy * cp * sr + cy * sp * cr;
	q.z = sy * cp * cr - cy * sp * sr;
	return q;
}

auto InverseKinematics::getWorldRotation(std::shared_ptr<bvh11::Joint> joint,int32_t frame) -> glm::mat3
{

	auto transform = object->GetTransformation(joint, frame);
	Eigen::Matrix3f currentRotation = transform.rotation();
	//
	glm::mat3 mat;
	for (auto i = 0;i<3;i++)
	{
		for (auto j = 0;j<3;j++)
		{
			mat[i][j] = currentRotation(i,j);
		}
	}
	return mat;

}

auto InverseKinematics::getLocalRotation(std::shared_ptr<bvh11::Joint> joint, int32_t frame) ->glm::mat3
{
	glm::vec3 angle = {};

	for (auto channelIndex : joint->associated_channels_indices())
	{
		const bvh11::Channel& channel = object->channels()[channelIndex];
		float& value = object->motion()(frame, channelIndex);

		switch (channel.type)
		{
		case bvh11::Channel::Type::x_rotation:
			angle.x = glm::radians(value);
			break;
		case bvh11::Channel::Type::y_rotation:
			angle.y = glm::radians(value);
			break;
		case bvh11::Channel::Type::z_rotation:
			angle.z = glm::radians(value);
			break;
		}
	}

	return glm::rotate(angle.x, glm::vec3{ 1,0,0 })* glm::rotate(angle.y, glm::vec3{ 0,1,0 })*
		glm::rotate(angle.z, glm::vec3{ 0,0,1 });
}

auto InverseKinematics::getLocalEulerAngle(std::shared_ptr<bvh11::Joint> joint,int32_t frame) ->glm::vec3
{
	glm::vec3 angle = {};

	for (auto channelIndex : joint->associated_channels_indices())
	{
		const bvh11::Channel& channel = object->channels()[channelIndex];
		float& value = object->motion()(frame, channelIndex);

		switch (channel.type)
		{
		case bvh11::Channel::Type::x_rotation:
			angle.x = value;
			break;
		case bvh11::Channel::Type::y_rotation:
			angle.y =value;
			break;
		case bvh11::Channel::Type::z_rotation:
			angle.z = value;
			break;
		}
	}
	return angle;
}

