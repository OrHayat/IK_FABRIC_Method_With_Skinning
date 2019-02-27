#pragma once
#include "MovableGLM.h"
#include "mesh.h"
#include "lineMesh.h"
#include "texture.h"
class Shape : public MovableGLM
{

	LineMesh *lineMesh;
	Texture *tex;
	bool isCopy;
public:
	bool collide = true;
	Mesh * mesh;
	bool toDraw = true;
	glm::vec4 ambient;
	enum{triangles,lines};

	Mesh * get_mesh() {
		return this->mesh;
	};
	Mesh* mycube = 0;
	unsigned int indices[36] = { 0, 1, 2,
		0, 2, 3,

		6, 5, 4,
		7, 6, 4,

		10, 9, 8,
		11, 10, 8,

		12, 13, 14,
		12, 14, 15,

		16, 17, 18,
		16, 18, 19,

		22, 21, 20,
		23, 22, 20
	};
	Vertex* Shape::initcube(double centerx, double centery, double centerz, double widthx, double highty, double depthz);
	Shape(const Shape& shape);
	Shape(const std::string& fileName, glm::vec4 amb);
	Shape(const std::string& fileName,const std::string& textureFileName, glm::vec4 amb);
	Shape(int CylParts, int linkPosition, glm::vec4 amb);
	Shape(int CylParts, int linkPosition,const std::string& textureFileName, glm::vec4 amb);
	Shape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices, glm::vec4 amb);
	Shape(LineVertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices, glm::vec4 amb);
	Shape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices, const std::string& textureFileName, glm::vec4 amb);
	void addMesh(Vertex* vertices, unsigned int numVertices,unsigned int* indices, unsigned int numIndices, const std::string& textureFileName);
	void addMesh(Vertex* vertices, unsigned int numVertices,unsigned int* indices, unsigned int numIndices);
	void addLineMesh(LineVertex* vertices, unsigned int numVertices,unsigned int* indices, unsigned int numIndices);
	void draw(int mode);
	virtual ~Shape(void);
};