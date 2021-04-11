//////////////////////////////////////////////////////////////////////////////
// This file is part of the PharaohStroy MMORPG client                      // 
// Copyright ?2020-2022 Prime Zeng                                          // 
//                                                                          // 
// This program is free software: you can redistribute it and/or modify     // 
// it under the terms of the GNU Affero General Public License as           // 
// published by the Free Software Foundation, either version 3 of the       // 
// License, or (at your option) any later version.                          // 
//                                                                          // 
// This program is distributed in the hope that it will be useful,          // 
// but WITHOUT ANY WARRANTY; without even the implied warranty of           // 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            // 
// GNU Affero General Public License for more details.                      // 
//                                                                          // 
// You should have received a copy of the GNU Affero General Public License // 
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    // 
////////////////////////////////////////////////////////////////////////////// 

#pragma once
#include <cstdint>
#include <chrono>

// Small class for measuring elapsed time between game loops.
class Timer
{
public:
	Timer()
	{
		start();
	}

	~Timer()
	{
	}

	// Start the timer by setting the last measurement to now.
	auto start() -> void
	{
		point = clock::now();
		prev = current();
	}

	// Return time elapsed since the last measurement.
	auto stop() -> int64_t
	{
		clock::time_point last = point;
		point = clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(point - last);
		return duration.count();
	}

	auto currentTimestamp()
	{
		auto tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
		auto tmp = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
		return tmp.count();
	}

	auto current() -> std::chrono::high_resolution_clock::time_point
	{
		return std::chrono::high_resolution_clock::now();
	}

	auto elapsed(std::chrono::high_resolution_clock::time_point begin,
					std::chrono::high_resolution_clock::time_point end)
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
	}

	auto step()-> float
	{
		auto currTime = current();
		auto ela =  elapsed(prev, currTime);
		prev = currTime;
		return ela / 1000000.f;
	}


private:
	using clock = std::chrono::high_resolution_clock;
	clock::time_point point;
	std::chrono::high_resolution_clock::time_point prev;
};

class Timestep
{
public:
	Timestep(float initTime = 0.0f)
		: time(initTime)
	{
	}

	operator float() const { return time; }

	inline auto getSeconds() const { return time; }
	inline auto getMilliseconds() const { return time * 1000.0f; }
private:
	float time;
};