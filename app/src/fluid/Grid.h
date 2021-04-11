#pragma once
#include <vector>
#include "FluidParticle.h"
#include <memory>


class Grid 
{
public:
	auto updateCells(int32_t numberCellsX, int32_t  numberCellsY) -> void;
	auto update(const std::vector<std::shared_ptr<FluidParticle>>& particles, float range) -> void;
	auto getNeighboringCells(const std::shared_ptr<FluidParticle>& particle, float range)->std::vector<std::vector<int32_t>>;
private:
	int32_t numberCellsX = 0;
	int32_t numberCellsY = 0;
	std::vector<std::vector<std::vector<int32_t>>> cells;
};