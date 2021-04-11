
#include "Cloth.h"
#include "opengl/Vertex.h"
#include <imgui.h>
#include "glm/ext/matrix_transform.hpp"
#include "glm/glm.hpp"
#include "obj.h"
#include "tools/ObjLoader.h"
#include <tiny_obj_loader.h>
#include <algorithm>

Cloth::Cloth(const std::string& path)
	:Mesh(path,true)
{
	for (auto & v : vertices)
	{
		auto par = std::make_shared<Particle>(v.pos,this);
		par->setNormal(v.normal);
		par->setTextCoord(v.texCoord);
		particles.emplace_back(par);
	}


	if (indices.size() >= 3)
	{
		auto& v1 = vertices[indices[0]];
		auto& v2 = vertices[indices[1]];
		auto& v3 = vertices[indices[2]];
		auto size = box.size();
		float maxX = std::max(std::max(v1.pos.x, v2.pos.x), v3.pos.x);
		float minX = std::min(std::min(v1.pos.x, v2.pos.x), v3.pos.x);
		//we are in Y plane
		float maxZ = std::max(std::max(v1.pos.z, v2.pos.z), v3.pos.z);
		float minZ = std::min(std::min(v1.pos.z, v2.pos.z), v3.pos.z);

		float dx = maxX - minX;
		float dz = maxZ - minZ;

		int32_t xCount = std::round(size.x / dx) + 1;
		int32_t zCount = std::round(size.z / dz) + 1;

		auto tempParticles = particles;
		std::sort(tempParticles.begin(), tempParticles.end(), [](auto & a ,auto & b) {
			return a->getPosition().x > b->getPosition().x;
		});


		for (auto i = 0;i<zCount;i++)
		{
			twoDimParticles.emplace_back(std::vector<std::shared_ptr<Particle>>{});
			for (auto  j = 0;j< xCount;j++)
			{
				twoDimParticles[i].push_back(tempParticles[i * xCount + j]);
			}
		}


		for (auto & v : twoDimParticles)
		{
			std::sort(v.begin(), v.end(), [](auto& a, auto& b) {
				return a->getPosition().z > b->getPosition().z;
			});
		}
			
		/*for (auto i = 0;i< twoDimParticles.size();i++)
		{
			for (auto j = 0; j < twoDimParticles[i].size(); j++)
			{
				auto particle = twoDimParticles[i][j];
				particle->setTextCoord(glm::vec2((float)i / (size.x - 1), (float)j / (size.z - 1)));
			}
		}*/


		for (auto i = 0; i < twoDimParticles.size(); i++)
		{
			auto& v = twoDimParticles[i];
			for (auto j = 0; j < v.size(); j++)
			{
				if (i < twoDimParticles.size() - 1)
					springs.emplace_back(v[j], twoDimParticles[i + 1][j], DEFAULT_K);
				if (j < v.size() - 1)
					springs.emplace_back(v[j], twoDimParticles[i][j + 1], DEFAULT_K);
				if (i < twoDimParticles.size() - 1 && j < v.size() - 1)
					springs.emplace_back(v[j], twoDimParticles[i + 1][j + 1], DEFAULT_K);
				if (i < twoDimParticles.size() - 1 && j  > 0)
					springs.emplace_back(v[j], twoDimParticles[i + 1][j - 1], DEFAULT_K);
				if (i < twoDimParticles.size() - 2)
					springs.emplace_back(v[j], twoDimParticles[i + 2][j], DEFAULT_K);
				if (j < v.size() - 2)
					springs.emplace_back(v[j], twoDimParticles[i][j + 2], DEFAULT_K);
			}
		}
	} 

	//computeNormals();
	generateBuffer();
}

auto Cloth::computeNormals() -> void
{
	for (size_t i = 0; i < twoDimParticles.size() - 1; i++)
		for (size_t j = 0; j < twoDimParticles[i].size() - 1; j++)
		{
			glm::vec3 faceNormal = glm::cross(
				twoDimParticles[i + 1][j]->getPosition() - twoDimParticles[i][j]->getPosition(),
				twoDimParticles[i + 1][j + 1]->getPosition() - twoDimParticles[i + 1][j]->getPosition()
			);

			twoDimParticles[i][j]->normal += faceNormal;
			twoDimParticles[i + 1][j]->normal += faceNormal;
			twoDimParticles[i + 1][j + 1]->normal += faceNormal;

			faceNormal = glm::cross(twoDimParticles[i][j + 1]->position - twoDimParticles[i + 1][j + 1]->position,
				twoDimParticles[i][j]->position - twoDimParticles[i][j + 1]->position);
			twoDimParticles[i][j]->normal += faceNormal;
			twoDimParticles[i][j + 1]->normal += faceNormal;
			twoDimParticles[i + 1][j + 1]->normal += faceNormal;
		}

	for (size_t i = 0; i < twoDimParticles.size(); i++)
		for (size_t j = 0; j < twoDimParticles[i].size(); j++)
			twoDimParticles[i][j]->normal = glm::normalize(twoDimParticles[i][j]->normal);
}

auto Cloth::onImGui() -> void 
{
	static const char* DIALIG_NAME = "Input your obj file name";
	
	ImGui::Separator();

	if (ImGui::Button("Save To Obj")) 
	{
		
		ImGui::OpenPopup(DIALIG_NAME);
		// Always center this window when appearing
		ImVec2 center(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	}

	if (ImGui::BeginPopupModal(DIALIG_NAME, NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		static char buff[128] = {};
		ImGui::InputText("filename", buff, 128);
		if (ImGui::Button("OK", ImVec2(120, 0))) {
			saveToObj(buff);
			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}


	ImGui::Checkbox("Play", &play); 
	ImGui::DragFloat3("Cloth Position", &position[0], 0.1);

	if (ImGui::Checkbox("Lock 1", &lock1)) 
	{
		twoDimParticles[0][0]->setMoveable(!lock1);
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("Lock 2", &lock2)) 
	{
		twoDimParticles[0].back()->setMoveable(!lock2);
	}
	if (ImGui::Checkbox("Lock 3", &lock3)) 
	{
		twoDimParticles.back()[0]->setMoveable(!lock3);
	} 
	ImGui::SameLine();
	if (ImGui::Checkbox("Lock 4", &lock4)) 
	{
		twoDimParticles.back().back()->setMoveable(!lock4);
	}

	ImGui::Checkbox("Wind Simulation", &windSimulation);
	if (windSimulation)
	{
		ImGui::DragFloat3("Wind Force", &windForce[0], 0.01);
	}

	ImGui::Checkbox("Collision With Floor", &collisionWithFloor);

	ImGui::DragFloat("Gravity", &gravity, 0.01);
	ImGui::DragFloat("Mass", &mass, 0.01);
	ImGui::DragFloat("Friction", &friction, 0.01,0,1);




}

auto Cloth::clearForces() -> void
{
	for (size_t i = 0; i < twoDimParticles.size(); i++)
		for (size_t j = 0; j < twoDimParticles[i].size(); j++)
			twoDimParticles[i][j]->force = { 0.0f, 0.0f, 0.0f };
}

auto Cloth::addForces() -> void
{
	glm::vec3 gravity(0.0f, -mass * gravity, 0.0f);

	for (size_t i = 0; i < twoDimParticles.size(); i++)
		for (size_t j = 0; j < twoDimParticles[i].size(); j++)
			twoDimParticles[i][j]->addForce(gravity + (windSimulation ? (float)mass * windForce : glm::vec3{0,0,0}));

	for (auto& spring : springs) {
		spring.addForce();
	}
}

auto Cloth::groundCollision(float floorY) -> void
{
	if (collisionWithFloor) 
	{
		float l;
		for (size_t i = 0; i < twoDimParticles.size(); i++)
			for (size_t j = 0; j < twoDimParticles[i].size(); j++)
			{
				l = (twoDimParticles[i][j]->position.y + position.y - floorY);
				if (l < 0)
					twoDimParticles[i][j]->movable = false;
			}
	}
}

auto Cloth::calculateNextPosition() -> void
{
	for (size_t i = 0; i < twoDimParticles.size(); i++)
		for (size_t j = 0; j < twoDimParticles[i].size(); j++)
			twoDimParticles[i][j]->calculateNextPosition();
}

auto Cloth::generateBuffer() -> void
{
	for (auto i = 0;i<particles.size();i++)
	{
		auto & ver = vertices[i];
		auto particle = particles[i];
		ver.normal = particle->getNormal();
		ver.pos = particle->getPosition();
		ver.texCoord = particle->getTextCoord();
	}

	if (getVertexArray()->getVertexBuffers().size() > 0) 
	{
		getVertexArray()->getVertexBuffers()[0]->updateData(vertices.data(), sizeof(Vertex) * vertices.size());
	}
}

auto Cloth::updatePosition(const glm::vec3& clothPosition) -> void
{
	position = clothPosition;
}

auto Cloth::collisionWithMesh(const Sphere * s) -> void
{
	glm::vec3 repulsiveForce;
	for (size_t i = 0; i < twoDimParticles.size(); i++)
	{
		for (size_t j = 0; j < twoDimParticles[i].size(); j++)
		{

			if (glm::distance(s->getPosition(), twoDimParticles[i][j]->position + position)  < s->getRadius() )
			{
				repulsiveForce = twoDimParticles[i][j]->position + position - s->getPosition();
				/*glm::vec3 g(0, -mass * gravity, 0);
				auto cosTheta = glm::dot(g, repulsiveForce * -1.f) / (glm::length(repulsiveForce) * glm::length(g));
*/				
				glm::vec3 g(0, -mass * gravity, 0);	
				auto frictionDir = glm::normalize(g - repulsiveForce) * friction * glm::length(repulsiveForce);
				twoDimParticles[i][j]->addForce(repulsiveForce + frictionDir );
			}
		}
	}
}

auto Cloth::draw(opengl::Shader* shader, const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) -> void 
{
	Mesh::draw(shader, glm::translate(m,position), v, p);
}

auto Cloth::saveToObj(const std::string& name) -> void
{
	
	wow::Obj obj;
	auto k = 0;

	obj.enableTextureCoordinates(true);
	for (auto & par : particles)
	{
		wow::Vertex v(par->position.x, par->position.y, par->position.z);
		v.setTexCoord(par->getTextCoord().x, par->getTextCoord().y);
		obj.appendVertex(v);
	}

	for (int32_t i = 0; i<indices.size(); i+=3)
	{
		obj.appendFace({ (int32_t)indices[i], (int32_t)indices[i + 1], (int32_t)indices[i + 2] });
	}


	obj.output(name);
}

