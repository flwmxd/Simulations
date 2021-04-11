

#include "ModelLayerFluid.h"
#include "Application.h"
#include <limits>
#include <cmath>


auto ModelLayerFluid::init() -> void
{
	Layer::init();
	shader = std::make_unique<opengl::Shader>("shader/vertClothShader.glsl","shader/fragClothShader.glsl");
	shader->compile();

	projection = glm::ortho(-1.0, 1.0, -1.0, 1.0, -100.0, 100.0);
	model = glm::mat4(1.f);
	view = glm::mat4(1.f);

	linesCommand = std::make_unique<BatchCommand>(BatchCommand::Type::LINES);
	pointCommand = std::make_unique<BatchCommand>(BatchCommand::Type::POINTS);


	fluidConfig.box[0] = glm::vec2{ -0.8,0.8 };
	fluidConfig.box[1] = glm::vec2{ 0.8,-0.8 };

	handler.mouseScrollHandler = [&](auto event) {
		if(isActive() && camera)
			camera->onMouseScrolled(event->getYOffset());
		return isActive();
	};

	handler.mouseClickHandler = [&](auto event) {
		mouseClicked = true;
		lastPosition = event->position;
		return false;
	};

	handler.mouseRelaseHandler = [&](auto event) {
		mouseClicked = false;
		return false;
	};

	handler.mouseMoveHandler = [&](auto event) {
		if (isActive() && mouseClicked && camera) {
			auto delta = event->position - lastPosition;
			camera->onMouseMove(delta.x, delta.y,false);
			lastPosition = event->position;
		}
		return isActive() && mouseClicked;
	};

	
	handler.keyPressedHandler = [&](auto event) {
		if (isActive() && camera) {
			camera->onKeyDown(event->getKeyCode());
		}
		return false;
	};
	
	app->getEventDispatcher().addEventHandler(&handler);

	start();
	glEnable(GL_POINT_SMOOTH);
}
auto ModelLayerFluid::start() -> void
{
	updateConfig();
	updateGrid();
	initParticles();
}


auto ModelLayerFluid::draw(const Timestep& timestep) -> void
{
	update(timestep);
	drawBox();
	flatShader->useProgram();
	flatShader->useProgram();
	flatShader->setUniformMatrix4f("model", glm::value_ptr(model));
	flatShader->setUniformMatrix4f("view", glm::value_ptr(view));
	flatShader->setUniformMatrix4f("projection", glm::value_ptr(projection));
	linesCommand->flush(*flatShader);
	drawParticles();
	pointCommand->flush(*flatShader);
	glPointSize(10);
	
}

auto ModelLayerFluid::onImGui() -> void
{
	ImGui::Text("Config");
	
	if (ImGui::DragFloat2("Box Left Top", &fluidConfig.box[0][0], 0.001)) 
	{
		start();
	
	}
	if (ImGui::DragFloat2("Box Right Bottom", &fluidConfig.box[1][0], 0.001)) 
	{
		start();
	}

	ImGui::DragInt("Number of Particles", &fluidConfig.numberParticles);

	ImGui::DragFloat("Stiffness", &fluidConfig.stiffness,0.1); 
	ImGui::DragFloat("Viscosity", &fluidConfig.viscosity, 0.1);
	ImGui::DragFloat("Tension", &fluidConfig.tension, 0.1);
	ImGui::DragFloat("Gravity", &fluidConfig.gravity, 0.1);



	if (ImGui::Checkbox("Simulation 3 With Spiky kernel (Default is Poly6)", &fluidConfig.spikyWithPressure)) 
	{
		start();
	}
	if (ImGui::Checkbox("Simulation 4 With Viscosity", &fluidConfig.calViscosity))
	{
		start();
	}
	if(ImGui::Checkbox("Simulation 5 With Surface Tension", &fluidConfig.calSurface))
	{
		start();
	}

	if (ImGui::Button("Restart")) 
	{
		start();
	}

}



auto ModelLayerFluid::onImGuiEnd() -> void
{
	
}

auto ModelLayerFluid::update(const Timestep& dt) -> void
{
	for (auto& p : particles)
	{
		p->force = {0,0};
	}

	findNeighborhoods();
	calculateDensity();
	calculatePressure();
	calculateForceDensity();
	integrationStep(dt);
	collisionHandling();
	grid.update(particles, fluidConfig.kernelRange);
}

auto ModelLayerFluid::initParticles() -> void
{
	int32_t particlesX = fluidConfig.numberParticles / 2.0f;
	int32_t particlesY = fluidConfig.numberParticles;
	totalParticles = particlesY * particlesX;
	particles.clear();
	auto w = 0.2;
		//std::abs(fluidConfig.box[0].x - fluidConfig.box[1].x);
	auto h = 0.7;//std::abs(fluidConfig.box[0].y - fluidConfig.box[1].y);
	float dx = w / particlesX;
	float dy = h / particlesY;

	auto start = glm::vec2{ 0.4,0 };
	for (int32_t i = 0; i < particlesX; i++)
	{
		for (int32_t j = 0; j < particlesY; j++)
		{
			auto pos = start + glm::vec2(i * dx, j * dy);
			particles.emplace_back(std::make_shared<FluidParticle>(pos,fluidConfig.mass));
		}
	}
	grid.update(particles,fluidConfig.kernelRange);
}

auto ModelLayerFluid::updateGrid() -> void
{
	auto w = std::abs(fluidConfig.box[0].x - fluidConfig.box[1].x) / fluidConfig.kernelRange + 1;
	auto h = std::abs(fluidConfig.box[0].y - fluidConfig.box[1].y) / fluidConfig.kernelRange + 1;
	grid.updateCells(w, h);
}

auto ModelLayerFluid::drawBox() -> void
{
	linesCommand->addVertex({ fluidConfig.box[0] , 0 }, { 1,1,1,1.f });
	linesCommand->addVertex({ fluidConfig.box[0].x, fluidConfig.box[1].y , 0 }, { 1,1,1,1.f });
	linesCommand->addVertex({ fluidConfig.box[1], 0 }, { 1,1,1,1.f });
	linesCommand->addVertex({ fluidConfig.box[1].x, fluidConfig.box[0].y , 0 }, { 1,1,1,1.f });
	linesCommand->addVertex({ fluidConfig.box[0].x, fluidConfig.box[1].y , 0 }, { 1,1,1,1.f });
	linesCommand->addVertex({ fluidConfig.box[1] , 0 }, { 1,1,1,1.f });
}

auto ModelLayerFluid::drawParticles() -> void
{

	auto w = std::abs(fluidConfig.box[0].x - fluidConfig.box[1].x);
	auto h = std::abs(fluidConfig.box[0].y - fluidConfig.box[1].y);
	for (auto & p : particles)
	{
		auto pos = p->getPosition();
		pos.y = h - pos.y;
		pos.x += fluidConfig.box[0].x;
		pos.y -= fluidConfig.box[0].y;
		pointCommand->addVertex({ pos,0 }, p->getRenderColor());
	}
}

auto ModelLayerFluid::findNeighborhoods() -> void
{
	auto maxDistance = fluidConfig.kernelRange * fluidConfig.kernelRange;
	neighborhoods.clear();

	for (const auto & p : particles)
	{
		std::vector<int32_t> neighbors;
		auto neighboringCells = grid.getNeighboringCells(p, fluidConfig.kernelRange);

		for (const auto & cell : neighboringCells)
		{
			for (auto index : cell)
			{
				auto x = p->getPosition() - particles[index]->getPosition();
				float dist2 = x.x * x.x + x.y * x.y;
				if (dist2 <= maxDistance) {
					neighbors.emplace_back(index);
				}
			}
		}
		neighborhoods.emplace_back(neighbors);
	}
}

auto ModelLayerFluid::calculateDensity() -> void
{
#pragma omp parallel for
	for (auto i = 0; i < totalParticles; i++)
	{
		auto & neighbors = neighborhoods[i];
		float densitySum = 0.0f;

		for (auto n = 0; n < neighbors.size(); n++)
		{
			auto j = neighbors[n];
			auto distance = particles[i]->getPosition() - particles[j]->getPosition();
			densitySum += particles[j]->getMass() * Poly6::calculate(distance, fluidConfig.kernelRange);
		}
		particles[i]->setDensity(densitySum);
	}
}

auto ModelLayerFluid::calculatePressure() -> void
{
#pragma omp parallel for
	for (auto i = 0; i < particles.size(); i++)
	{
		//P=k(p-p0)
		
		auto& particle = particles[i];
		auto pressure = fluidConfig.stiffness * 1000 * (particle->getDensity() - fluidConfig.restDensity);
		particle->setPressure(std::max<float>(pressure, 0.0f));
	}

}

auto ModelLayerFluid::calculateForceDensity() -> void
{
#pragma omp parallel for
	for (auto i = 0; i < particles.size(); i++)
	{
		auto fPressure = glm::vec2(0.0f, 0.0f);
		auto fViscosity = glm::vec2(0.0f, 0.0f);
		auto fGravity = glm::vec2(0.0f, 0.0f);
		auto fSurface = glm::vec2(0.0f, 0.0f);
		auto colorFiled = glm::vec2(0.0f, 0.0f);
		auto colorFieldNormal = glm::vec2(0.0f,0.0f);
		float colorFieldLaplacian = 1.f;
		auto & neighbors = neighborhoods[i];

		//particles[i].color = 0;

		for (auto n = 0; n < neighbors.size(); n++)
		{
			int j = neighbors[n];
			if (i == j) {
				continue;
			}
			auto distance = particles[i]->getPosition() - particles[j]->getPosition();

	
/**
*f(pressure)i =−∑j(mj * (pi+pj)/(2ρj) * ∇W(ri−rj,h)).
* Mass * PressureSum * Smoothing Kernel Gradient
*/
			if (fluidConfig.spikyWithPressure) 
			{
				fPressure += particles[j]->getMass() * (particles[i]->getPressure() + particles[j]->getPressure()) / (2.0f * particles[j]->getDensity()) * Spiky::calculateGradient(distance, fluidConfig.kernelRange);
			}
			else
			{
				fPressure += particles[j]->getMass() * (particles[i]->getPressure() + particles[j]->getPressure()) / (2.0f * particles[j]->getDensity()) * Poly6::calculateGradient(distance, fluidConfig.kernelRange);
			}
			
			if (fluidConfig.calViscosity)
			{
/** 
* f(viscosity)i =µ∑j(mj * (vj−vi)/ρj * ∇2 W(ri−rj,h)
* Mass * Velocity/Density * Smoothing Kernel Laplacian
*/
				fViscosity += particles[j]->getMass() * (particles[j]->getVelocity() - particles[i]->getVelocity()) / particles[j]->getDensity() * Viscosity::calculate(distance, fluidConfig.kernelRange);
			}
		
			// Color field

/**
*  cs(r) =µ∑j(mj/ρj * ∇W(ri−rj,h)
*/
			colorFiled += particles[j]->getMass() / particles[j]->density * Poly6::calculate(distance, fluidConfig.kernelRange);
/*
* n = ∇cs
*/
			colorFieldNormal += Poly6::calculateGradient(colorFiled, fluidConfig.kernelRange) / particles[j]->getDensity();
/**
* (∇cs)^2
*/
			colorFieldLaplacian += Poly6::calculateLaplacian(colorFiled, fluidConfig.kernelRange) / particles[j]->getDensity();
		}

		colorFieldNormal *= fluidConfig.mass;
		particles[i]->normal = -1.f * colorFieldNormal;
		colorFieldLaplacian *= fluidConfig.mass;
		
/**
* |n|
*/
		auto colorFieldNormalMagnitude = colorFieldNormal.x * colorFieldNormal.x + colorFieldNormal.y * colorFieldNormal.y;

		if (colorFieldNormalMagnitude > fluidConfig.surfaceThreshold && fluidConfig.calSurface)
		{
		
			fSurface = -fluidConfig.tension * colorFieldLaplacian * colorFieldNormal / colorFieldNormalMagnitude;
		}
		
		fGravity = particles[i]->getDensity() * glm::vec2(0, fluidConfig.gravity * 1000);
		fPressure *= -1.0f;
		fViscosity *= fluidConfig.viscosity * 1000;
		particles[i]->force += fPressure + fViscosity + fGravity + fSurface;// +fSurface;

	}
}

auto ModelLayerFluid::integrationStep(const Timestep& dt) -> void
{
	auto w = std::abs(fluidConfig.box[0].x - fluidConfig.box[1].x);
	auto h = std::abs(fluidConfig.box[0].y - fluidConfig.box[1].y);
#pragma omp parallel for
	for (int32_t i = 0;i<particles.size();i++) 
	{
		constexpr float dt = 0.00005f;
	
		auto& particle = particles[i];
		
		auto acc2 = particle->force / particle->density;

		//particle->velocity += dt * acc2;
		//particle->position += dt * particle->velocity;


		particle->velocity += dt * 0.5f * acc2;
		particle->position += dt * particle->velocity;
		particle->velocity += dt * 0.5f * acc2;

	}
}


auto ModelLayerFluid::collisionHandling() -> void
{

	auto w = std::abs(fluidConfig.box[0].x - fluidConfig.box[1].x);
	auto h = std::abs(fluidConfig.box[0].y - fluidConfig.box[1].y);
#pragma omp parallel for
	for (int32_t i = 0; i < particles.size(); i++)
	{
		auto& particle = particles[i];
		//
		if (particle->position.x < 0)
		{
			particle->position.x = 0;
			particle->velocity.x = -0.5f * particle->velocity.x;
		}
		else if (particle->position.x > w)
		{
			particle->position.x = w;
			particle->velocity.x = -0.5f * particle->velocity.x;
		}

		if (particle->position.y < 0)
		{
			particle->position.y = 0;
			particle->velocity.y = -0.5f * particle->velocity.y;
		}
		else if (particle->position.y > h)
		{
			particle->position.y = h;
			particle->velocity.y = -0.5f * particle->velocity.y;
		}

	}
}



auto ModelLayerFluid::updateConfig() -> void
{
	fluidConfig.spacing = 1.f / fluidConfig.numberParticles;
	fluidConfig.kernelRange = 2 * fluidConfig.spacing;
}
