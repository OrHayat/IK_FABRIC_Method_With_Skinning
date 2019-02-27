#define GLEW_STATIC
#include "mesh.h"
#include <GL/glew.h>
//#include "util.h"
#include "debugTimer.h"
#include "curves.h"
#include <map>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdlib.h>

Mesh::Mesh(int CylParts,int linkPosition, bool iskdtree)
{
	model = Surface(CylParts, 8, 8, linkPosition).ToIndexedModel();
	InitMesh(model,false);
	buildKDTree(model);

}

Mesh::Mesh(const std::string& fileName, bool withKdtree, int maxfaces)
{
	IndexedModel model;
	OBJModel(fileName, maxfaces).ToIndexedModel(model);
	InitMesh(model, false);
	if (withKdtree)
		buildKDTree(model);
}

void Mesh::buildKDTree(const IndexedModel& model) {
	std::list<Node::vecType> treelist;
	for (unsigned int i = 0; i < model.positions.size(); i++)
	{
		treelist.push_back(Node::vecType(model.positions[i], 1));
	}
	kdtree.makeTree(treelist);
	//	kdtree.printTree(kdtree.getRoot());
}
void Mesh::InitMesh(const IndexedModel& model,bool isBox)
{
    m_numIndices = model.indices.size();

    glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.positions[0]) * model.positions.size(), &model.positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(model.texCoords[0]) * model.texCoords.size(), &model.texCoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(model.normals[0]) * model.normals.size(), &model.normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[COLOR_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(model.colors[0]) * model.colors.size(), &model.colors[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(model.indices[0]) * model.indices.size(), &model.indices[0], GL_STATIC_DRAW);
	if (!isBox) 
	{
		if (model.weights.size() > 0)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[WEIGHT_VB]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(model.weights[0]) * model.weights.size(), &model.weights[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}
	}
	glBindVertexArray(0);
}

Mesh::Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices, bool iskdtree)
{

	for(unsigned int i = 0; i < numVertices; i++)
	{
		model.positions.push_back(*vertices[i].GetPos());
		model.texCoords.push_back(*vertices[i].GetTexCoord());
		model.normals.push_back(*vertices[i].GetNormal());
		model.colors.push_back(*vertices[i].GetColor());
	}

	for(unsigned int i = 0; i < numIndices; i++)
        model.indices.push_back(indices[i]);
	buildKDTree(model);
    InitMesh(model,true);
}

Mesh::~Mesh()
{
	glDeleteBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}

void Mesh::Draw(int mode)
{
	glBindVertexArray(m_vertexArrayObject);

	//glDrawElements(GL_LINES, m_numIndices, GL_UNSIGNED_INT, 0);

	glDrawElementsBaseVertex(mode, m_numIndices, GL_UNSIGNED_INT, 0, 0);
	glBindVertexArray(0);
}
