#pragma once
#include "scene.h"
#include <iostream>
const int linksNum = 5;
const int maximumIterations = 1;

class IK : public Scene
{
	bool cameraMode;
	glm::vec3 tipPosition;
	glm::vec3 distPosition;
	bool isIKactive;
	double delta;
	float maxDistance;

public:

	IK(void);
	void IK::getDistances();
	virtual ~IK(void);

	void calculateDQS();
	IK(glm::vec3 position, float angle, float hwRelation, float near, float far);
	float IK::Distance(int indx1, int indx2);
	void init(Vertex *vertices, unsigned int *indices, int verticesSize, int indicesSize);
	float distToTarget(int linkInd);
	void addShape(int type, int parent);
	void addShape(const std::string& fileName, int parent);
	void addShape(const std::string& fileName, const std::string& textureFileName, int parent);
	void addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices, int parent);
	void addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices, const std::string& textureFileName, int parent);
	void addShape(int CylParts, int linkPosition, int parent);
	void addShape(int CylParts, int linkPosition, const std::string& textureFileName, int parent);
	void IK::printTipPositions();
	void IK::printRotationCalc(glm::vec3 pi, glm::vec3 pi_plus1, glm::vec3 vp, glm::vec3 v, float zAngle, float xAngle);
	void inline changeMode() { cameraMode = !cameraMode; }
	void makeChange();
	void IK::calculateStep(std::vector<glm::vec3> prevPos, std::vector<glm::vec3> newPos/*bool EulerVersion*/);
	bool inline isActive() const { return isIKactive; }
	void inline dActivate() { isIKactive = !isIKactive; }
};

