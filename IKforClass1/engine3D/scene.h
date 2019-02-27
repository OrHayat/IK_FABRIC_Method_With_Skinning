#pragma once
#include "shader.h"
#include "shape.h"
#include "camera.h"
#include <vector>
#include "mesh.h"


class Scene : public MovableGLM
{
	std::vector<Shader*> shaders;
	
	Shape *axisMesh;
protected:
	std::vector<int> chainParents;
	int pickedShape;
	static const int scaleFactor = 3;
public:
	int camera_index = 0;
	bool collidesnake = false;
	std::vector<Camera*> cameras; //light will have the properties of camera
	glm::vec3 Scene::checkDirection(int indx, glm::vec3 dir, int upperZ, int lowerZ);
	std::vector<Shape*> shapes;

	float *speedx = new float(-0.005f);
	float *speedy = new float(0.0f);

	std::vector<Shape*> Scene::get_shapes();
	glm::mat4 maketranslatetoshape(int indx);
	void Scene::init_angle(int linksNum);
	enum axis{xAxis,yAxis,zAxis};
	enum transformations{xLocalTranslate,yLocalTranslate,zLocalTranslate,xGlobalTranslate,yGlobalTranslate,zGlobalTranslate,
		xLocalRotate,yLocalRotate,zLocalRotate,xGlobalRotate,yGlobalRotate,zGlobalRotate,xScale,yScale,zScale,xCameraTranslate,yCameraTranslate,zCameraTranslate};
	Scene();
	int get_pickedShape() {
		return this->pickedShape;
	}
	void set_pickedShape(int pi) {
		this->pickedShape = pi;
	}
	std::vector<int> get_chainParents() {
		return this->chainParents;
	}
	Scene(glm::vec3 position,float angle,float hwRelation,float near, float far);
	void addShape(int type,int parent);
	void addShape(const Shape& shape, int parent);
	void addShape(const std::string& fileName,int parent);
	void addShape(const std::string& fileName,const std::string& textureFileName,int parent);
	void addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices,int parent);
	void addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices,const std::string& textureFileName,int parent);
	void addShape(int Cyparts,int linkPosition,int parent);
	void addShape(int CylParts,int linkPosition,const std::string& textureFileName,int parent);
	glm::vec3 getCenterPosition(int indx, glm::vec3 p);
	void shape_rotate(int index, float ang, glm::vec3 vec);
	void shape_rotate(int index,glm::mat4 mat);
	bool Scene::collide(Node* A, Node* B, glm::mat4 trt1, glm::mat4 trt2);
	bool Scene::equ(Node* A, Node * B, glm::vec3 L, glm::mat4 trt1, glm::mat4 trt2);
	bool Scene::find_collision(int index1, int index2);
	glm::vec3 getVectorInSystem(int i, glm::mat4 &prevTransformations, glm::vec3 point);
	void addShader(const std::string& fileName);
	void Scene::build_new_rotate(int index, int axis, float sin, float cos);
	glm::mat4 GetViewProjection(int indx) const;
	glm::mat4 GetShapeTransformation() const;
	void draw(int shaderIndx,int cameraIndx,bool drawAxis);
	void shapeTransformation(int type,float amt);
	void shapeRotation(glm::vec3 v, float ang,int indx);
	//void inline setPicked(int pickID){pickedShape = pickID;}
	float picking(double x,double y);
	void inline setPicked(int pickID) { pickedShape = pickID; } //@Uncommented
	glm::mat4 GetShapeTrans(int j);
	void resize(int width,int hight,int near,int far);
	//void updateTipPosition(int indx);
	glm::vec3 getTipPosition(int indx);
	glm::vec3 getDistination(int indx);
	glm::vec3 getAxisDirection(int indx,int axis);
	inline void setParent(int indx,int newValue) {chainParents[indx]=newValue;}
	virtual ~Scene(void);
	Mesh* unitcube;
	std::queue<std::pair<int, Node*>>* to_draw;

};

