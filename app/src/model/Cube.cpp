

#include "Cube.h"
#include "opengl/Buffer.h"
#include "opengl/OGL.h"
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265f
#endif

auto Cube::init() -> void
{
	Primitive::init();

	
	auto buffer = std::make_shared<VertexBuffer>(108 * 2);

	buffer->setLayout({
			{ ShaderDataType::Float3, "aPos" },
			{ ShaderDataType::Float3, "aNormal"},
	});

	constexpr float v[] =
	{
		+0.5, +0.5, +0.5,
		-0.5, +0.5, +0.5,
		-0.5, -0.5, +0.5,
		+0.5, +0.5, +0.5,
		-0.5, -0.5, +0.5,
		+0.5, -0.5, +0.5,

		+0.5, +0.5, -0.5,
		+0.5, +0.5, +0.5,
		+0.5, -0.5, +0.5,
		+0.5, +0.5, -0.5,
		+0.5, -0.5, +0.5,
		+0.5, -0.5, -0.5,

		-0.5, +0.5, -0.5,
		+0.5, +0.5, -0.5,
		+0.5, -0.5, -0.5,
		-0.5, +0.5, -0.5,
		+0.5, -0.5, -0.5,
		-0.5, -0.5, -0.5,

		-0.5, +0.5, +0.5,
		-0.5, +0.5, -0.5,
		-0.5, -0.5, -0.5,
		-0.5, +0.5, +0.5,
		-0.5, -0.5, -0.5,
		-0.5, -0.5, +0.5,

		+0.5, +0.5, -0.5,
		-0.5, +0.5, -0.5,
		-0.5, +0.5, +0.5,
		+0.5, +0.5, -0.5,
		-0.5, +0.5, +0.5,
		+0.5, +0.5, +0.5,

		-0.5, -0.5, +0.5,
		-0.5, -0.5, -0.5,
		+0.5, -0.5, -0.5,
		-0.5, -0.5, +0.5,
		+0.5, -0.5, -0.5,
		+0.5, -0.5, +0.5
	};

	constexpr float normals[] =
	{
		0.0, 0.0, +1.0,
		0.0, 0.0, +1.0,
		0.0, 0.0, +1.0,
		0.0, 0.0, +1.0,
		0.0, 0.0, +1.0,
		0.0, 0.0, +1.0,

		+1.0, 0.0, 0.0,
		+1.0, 0.0, 0.0,
		+1.0, 0.0, 0.0,
		+1.0, 0.0, 0.0,
		+1.0, 0.0, 0.0,
		+1.0, 0.0, 0.0,

		0.0, 0.0, -1.0,
		0.0, 0.0, -1.0,
		0.0, 0.0, -1.0,
		0.0, 0.0, -1.0,
		0.0, 0.0, -1.0,
		0.0, 0.0, -1.0,

		-1.0, 0.0, 0.0,
		-1.0, 0.0, 0.0,
		-1.0, 0.0, 0.0,
		-1.0, 0.0, 0.0,
		-1.0, 0.0, 0.0,
		-1.0, 0.0, 0.0,

		0.0, +1.0, 0.0,
		0.0, +1.0, 0.0,
		0.0, +1.0, 0.0,
		0.0, +1.0, 0.0,
		0.0, +1.0, 0.0,
		0.0, +1.0, 0.0,

		0.0, -1.0, 0.0,
		0.0, -1.0, 0.0,
		0.0, -1.0, 0.0,
		0.0, -1.0, 0.0,
		0.0, -1.0, 0.0,
		0.0, -1.0, 0.0
	};

	std::vector<float> vertices;


	for (auto i = 0;i<108;i+=3)
	{
		auto x = v[i];
		auto y = v[i + 1];
		auto z = v[i + 2];


		auto nx = v[i];
		auto ny = v[i + 1];
		auto nz = v[i + 2];

		vertices.emplace_back(x);
		vertices.emplace_back(y);
		vertices.emplace_back(z);

	

		vertices.emplace_back(nx);
		vertices.emplace_back(ny);
		vertices.emplace_back(nz);
	
	}

	buffer->setData(vertices.data(), vertices.size() * sizeof(float));

	vertexArray->addVertexBuffer(buffer);
	
}

