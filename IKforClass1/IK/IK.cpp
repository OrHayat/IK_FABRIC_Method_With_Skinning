#include "IK.h"
#include <iostream>
#include <math.h>
using namespace std;
using namespace glm;


void printArr(vector<int> arr)
{

	std::cout << "array" << std::endl;
	for (int i = 0; i < arr.size(); i++)
	{
		std::cout << i << "  ";
	}
	std::cout << std::endl;
	for (int p : arr)
	{

		std::cout << p << "  ";
	}
	cout << std::endl;
}

IK::IK(void)
{
	cameraMode = false;
	isIKactive = false;
	delta = 1e-1;
	distPosition = vec3(1, 0, 0);
	tipPosition = vec3(0, 0, linksNum*scaleFactor);
	maxDistance = linksNum * scaleFactor;
}

IK::IK(vec3 position, float angle, float hwRelation, float near, float far) : Scene(position, angle, hwRelation, near, far)
{
	cameraMode = false;
	isIKactive = false;
	delta = 1e-1;
	distPosition = vec3(1, 0, 0);
	tipPosition = vec3(0, 0, linksNum*scaleFactor);
	maxDistance = linksNum * scaleFactor;
}

IK::~IK(void)
{
}

void IK::init(Vertex *vertices, unsigned int *indices, int verticesSize, int indicesSize)
{
	myRotate(-90.0f, vec3(1, 0, 0), -1);
	//addShape(vertices, verticesSize, indices, indicesSize,"./res/textures/plane.png",-1);
	addShape(0, 2, "./res/textures/plane.png", -1);
	pickedShape = 0;
	shapeTransformation(zScale, scaleFactor);
	for (int i = 1; i < linksNum - 1; i++)
	{
		pickedShape = i;
		addShape(1, 1, "./res/textures/plane.png", -1);
		shapeTransformation(zScale, scaleFactor);
		shapeTransformation(zGlobalTranslate, 1.0);
		setParent(i, i - 1);
	}
	pickedShape = linksNum - 1;
	addShape(0, 3, "./res/textures/plane.png", -1);
	shapeTransformation(zScale, scaleFactor);

	shapeTransformation(zGlobalTranslate, 1.0);
	setParent(linksNum - 1, linksNum - 2);

	pickedShape = 0;
	// distination point
	pickedShape = linksNum;

	//addShape(0,"./res/textures/box0.bmp",-1);
	addShape(vertices, verticesSize, indices, indicesSize, "./res/textures/box0.bmp", -1);
	shapeTransformation(xScale, 1.0);
	shapeTransformation(yScale, 1.0);
	shapeTransformation(zScale, 1.0);
	shapeTransformation(xGlobalTranslate, 10.0);
	shapeTransformation(yGlobalTranslate, 0.0);
	shapeTransformation(zGlobalTranslate, 3.0);
	pickedShape = 0;

	distPosition = getDistination(linksNum);
	tipPosition = getTipPosition(linksNum - 1);
}

void IK::addShape(int CylParts, int linkPosition, int parent)
{

	__super::addShape(CylParts, linkPosition, parent);
}

void IK::addShape(int CylParts, int linkPosition, const std::string& fileName, int parent)
{
	__super::addShape(CylParts, linkPosition, fileName, parent);
}

void IK::addShape(int type, int parent)
{

	__super::addShape(type, parent);
}

void IK::addShape(const std::string& fileName, int parent)
{

	__super::addShape(fileName, parent);
}

void IK::addShape(const std::string& fileName, const std::string& textureFileName, int parent)
{

	__super::addShape(fileName, textureFileName, parent);
}

void IK::addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices, int parent)
{

	__super::addShape(vertices, numVertices, indices, numIndices, parent);
}

void IK::addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices, const std::string &textureFlieName, int parent)
{

	__super::addShape(vertices, numVertices, indices, numIndices, textureFlieName, parent);
}

void IK::calculateStep(std::vector<glm::vec3> before, std::vector<glm::vec3> points)
{
	glm::vec3 Zaxis;
	glm::vec3 Xaxis;
	glm::vec3 Vp;
	glm::vec3 V;

	glm::vec3 a, b, newZ;
	float theta, phi, cosPhi, cosTheta, sinPhi, sinTheta;


	for (int i = 0; i < linksNum; i++) {
		float signX = 1;
		float signZ = 1;
		if (before.at(linksNum).x - points.at(0).x > 0)
			signX = -1;
		float r, gamma;;
		glm::vec3 Zaxis;
		glm::vec3 Xaxis;

		mat4 axiesMat = mat4(1);
		for (int j = 0; j < i; j++)
		{
			axiesMat = axiesMat * maketranslatetoshape(j);
		}
		Xaxis = glm::vec3(axiesMat[0]);
		Zaxis = glm::vec3(axiesMat[2]);


		glm::vec3 Vp = points[i + 1] - points[i];
		Zaxis = normalize(Zaxis);
		Xaxis = normalize(Xaxis);
		if (Vp != glm::vec3(0, 0, 0))
			Vp = normalize(Vp);
		glm::vec3 V = glm::vec3(glm::cross(Zaxis, Vp));
		if (V != glm::vec3(0, 0, 0))
			V = normalize(V);

		float theta = glm::acos(glm::clamp(glm::dot(Zaxis, Vp), -1.0f, 1.0f));
		float phi = -glm::acos(glm::clamp(glm::dot(Xaxis, V), -1.0f, 1.0f));;
		build_new_rotate(i, zAxis1, glm::cos(phi), glm::sin(signX*phi));
		build_new_rotate(i, xAxis1, glm::cos(theta), glm::sin(theta));
	}
}


void IK::makeChange()
{
	glm::vec3 b;
	vector<glm::vec3> before;
	vector<glm::vec3> new_points;
	glm::vec3 target = getDistination(linksNum);
	float ri;
	float lambda;

	for (int i = 0; i < linksNum; i++) {
		before.push_back(getDistination(i));
	}
	before.push_back(getTipPosition(linksNum - 1));
	if (maxDistance < distToTarget(-1)) {
		new_points.push_back(before.at(0));
		for (int i = 0; i <= linksNum - 1; i++) {
			ri = distToTarget(i - 1);
			lambda = scaleFactor / ri;
			new_points.push_back((1 - lambda)*new_points.at(i) + lambda * target);
		}
		calculateStep(before, new_points);

	}
	else {
		for (int i = 0; i < linksNum; i++)
		{
			new_points.push_back(getDistination(i));
		}
		new_points.push_back(getTipPosition(linksNum - 1));
		b = new_points.at(0);
		float difa = glm::distance(new_points.at(new_points.size() - 1), target);

		while (delta < difa) {
			new_points.at(new_points.size() - 1) = target;
			for (int i = linksNum - 1; i >= 0; i--)
			{
				ri = glm::distance(new_points.at(i + 1), new_points.at(i));
				lambda = scaleFactor / ri;
				new_points.at(i) = (1 - lambda) * new_points.at(i + 1) + lambda * new_points.at(i);
			}

			new_points.at(0) = b;
			new_points.push_back(target);
			for (int i = 0; i <= linksNum - 1; i++)
			{
				ri = glm::distance(new_points.at(i + 1), new_points.at(i));
				lambda = scaleFactor / ri;
				new_points.at(i + 1) = (1 - lambda) * new_points.at(i) + lambda * new_points.at(i + 1);
			}
			difa = glm::distance(new_points.at(new_points.size() - 1), target);
			calculateStep(before, new_points);
		}
	}
}


void IK::printTipPositions() {
	for (int i = 0; i < linksNum; i++)
	{
		std::cout << "i is: " << i << ", and tip i is: " << getTipPosition(i).x << "," << getTipPosition(i).y << "," << getTipPosition(i).z << std::endl;
	}
}

float IK::distToTarget(int linkInd) {
	float dist;
	glm::vec3 linkTip;
	// getting the relevant link tip position coordinates
	if (linkInd >= 0)
		linkTip = getTipPosition(linkInd);
	else
		linkTip = glm::vec3(0, 0, 0);
	glm::vec3 cubeCenter = getDistination(linksNum);
	//	printVec(linkTip, "linkTip");
	//	printVec(cubeCenter, "cubeCenter");
	dist = glm::distance(linkTip, cubeCenter);
	//	std::cout << "dist: " << dist << std::endl;

	return dist;
}
void IK::printRotationCalc(glm::vec3 pi, glm::vec3 pi_plus1, glm::vec3 vp, glm::vec3 v, float zAngle, float xAngle) {
	std::cout << "  pi is : " << pi.x << " , " << pi.y << " , " << pi.z << std::endl;
	std::cout << "  pi_plus1 : " << pi_plus1.x << " , " << pi_plus1.y << " , " << pi_plus1.z << std::endl;
	std::cout << "  vp : " << vp.x << " , " << vp.y << " , " << vp.z << std::endl;
	std::cout << "  v : " << v.x << " , " << v.y << " , " << v.z << std::endl;
	std::cout << "  zAngle: " << zAngle << std::endl;
	std::cout << "  xAngle: " << xAngle << std::endl;
}