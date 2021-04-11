
#pragma once

#include "Config.h"
#include <memory>

class Particle;

class Spring
{
public:
	Spring();
	Spring(const std::shared_ptr<Particle> & x1, const std::shared_ptr<Particle> & x2, double spring = DEFAULT_K);
	auto addForce() -> void;

private:
	double restLength = 0;
	double springConstant = DEFAULT_K;
	std::shared_ptr<Particle> p1 ;
	std::shared_ptr<Particle> p2 ;
};

