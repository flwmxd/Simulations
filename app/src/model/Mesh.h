
#pragma  once
#include "engine/RenderObject.h"
#include "opengl/VertexArray.h"
#include "opengl/OGLTexture.h"
#include "opengl/Vertex.h"

#include "halfedge/Face.h"
#include "halfedge/HalfEdge.h"
#include "halfedge/HalfEdgeVertex.h"
#include "halfedge/Edge.h"

#include "tools/BoundingBox.h"

namespace std
{
	template<> struct hash<std::pair<int32_t, int32_t>>
	{
		typedef Vertex argument_type;
		typedef std::size_t result_type;
		size_t operator()(const std::pair<int32_t, int32_t>& s) const
		{
			size_t h1 = std::hash<float>()(s.first);
			size_t h2 = std::hash<float>()(s.first);
			return h1 ^ (h2 << 1);
		}
	};

}

class Mesh : public RenderObject 
{
public:
	Mesh();
	Mesh(const std::string& name,bool load = false);
	inline auto& getVertexArray() { return vertexArray; }
	virtual auto draw(opengl::Shader* shader, const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) -> void override;
	auto loadTexture(const std::string& name) -> void;
	static auto generateNormals(uint32_t numVertices, glm::vec3 * vertices, uint32_t * indices, uint32_t numIndices) ->glm::vec3*;
	virtual auto onImGui() -> void override;
	inline auto& getVertices() { return vertices; }
	inline auto& getIndices() { return indices; }
	inline auto& getBoundingBox() { return box; }
	inline auto& getName() const { return name; }
	

protected:
	std::string name;
	std::unique_ptr<VertexArray> vertexArray;
	std::shared_ptr<OGLTexture> texture;
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;



	BoundingBox box;

};