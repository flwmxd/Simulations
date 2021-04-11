#include "Grid.h"
#include <iostream>


auto Grid::updateCells(int32_t x, int32_t y) -> void
{
	numberCellsX = x;
	numberCellsY = y;
}


auto Grid::update(const std::vector<std::shared_ptr<FluidParticle>>& particles, float range) -> void
{

	cells = { size_t(numberCellsX) ,std::vector<std::vector<int32_t>>{ size_t(numberCellsY),std::vector<int32_t>{}} };

	for (auto i = 0; i < particles.size(); i++)
	{
		auto pos = particles[i]->getPosition();
		int32_t xCell =pos.x / range;
		int32_t yCell =pos.y / range;

		cells[xCell][yCell].emplace_back(i);
	}
}

auto Grid::getNeighboringCells(const std::shared_ptr<FluidParticle>& particle, float range) ->std::vector<std::vector<int32_t>>
{
	std::vector<std::vector<int32_t>> resultCells;
	auto pos = particle->getPosition();// -leftTop;

	int32_t xCell = pos.x / range;
	int32_t yCell = pos.y / range;

	resultCells.emplace_back(cells[xCell][yCell]);
	if (xCell > 0) resultCells.emplace_back(cells[xCell - 1][yCell]);
	if (xCell < numberCellsX - 1) resultCells.emplace_back(cells[xCell + 1][yCell]);

	if (yCell > 0) resultCells.emplace_back(cells[xCell][yCell - 1]);
	if (yCell < numberCellsY - 1) resultCells.emplace_back(cells[xCell][yCell + 1]);

	if (xCell > 0 && yCell > 0) resultCells.emplace_back(cells[xCell - 1][yCell - 1]);
	if (xCell > 0 && yCell < numberCellsY - 1) resultCells.emplace_back(cells[xCell - 1][yCell + 1]);
	if (xCell < numberCellsX - 1 && yCell > 0) resultCells.emplace_back(cells[xCell + 1][yCell - 1]);
	if (xCell < numberCellsX - 1 && yCell < numberCellsY - 1) resultCells.emplace_back(cells[xCell + 1][yCell + 1]);

	return resultCells;
}

