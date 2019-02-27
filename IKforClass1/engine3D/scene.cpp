#define GLEW_STATIC
#include <GL\glew.h>
#include "scene.h"
#include <iostream>
#include "MovableGLM.h"
using namespace std;
using namespace glm;

	LineVertex axisVertices[] = 
	{
		LineVertex(glm::vec3(1,0,0),glm::vec3(1,0,0)),
		LineVertex(glm::vec3(-1,0,0),glm::vec3(1,0,0)),
		LineVertex(glm::vec3(0,1,0),glm::vec3(0,1,0)),
		LineVertex(glm::vec3(0,-1,0),glm::vec3(0,1,0)),
		LineVertex(glm::vec3(0,0,1),glm::vec3(0,0,1)),
		LineVertex(glm::vec3(0,0,-1),glm::vec3(0,0,1)),
	};

	//Vertex axisVertices[] = 
	//	{
	//		Vertex(glm::vec3(1,0,0),glm::vec2(1, 0), glm::vec3(0, 0, -1),glm::vec3(1,0,0)),
	//		Vertex(glm::vec3(-1,0,0),glm::vec2(1, 0), glm::vec3(0, 0, -1),glm::vec3(1,0,0)),
	//		Vertex(glm::vec3(0,1,0),glm::vec2(1, 0), glm::vec3(0, 0, -1),glm::vec3(0,1,0)),
	//		Vertex(glm::vec3(0,-1,0),glm::vec2(1, 0), glm::vec3(0, 0, -1),glm::vec3(0,1,0)),
	//		Vertex(glm::vec3(0,0,1),glm::vec2(1, 0), glm::vec3(0, 0, -1),glm::vec3(0,0,1)),
	//		Vertex(glm::vec3(0,0,-1),glm::vec2(1, 0), glm::vec3(0, 0, -1),glm::vec3(0,0,1)),
	//	};

	unsigned int axisIndices[] = 
	{
		0,1,
		2,3,
		4,5
	};

	Scene::Scene()
	{
		glLineWidth(3);		
		cameras.push_back(new Camera(vec3(0,0,-20.0f),60.0f,1.0f,0.1f,100.0f));
		pickedShape = -1;

	}
	
	Scene::Scene(vec3 position,float angle,float hwRelation,float near, float far)
	{
		glLineWidth(3);
		cameras.push_back(new Camera(position,angle,hwRelation,near,far));
		cameras.push_back(new Camera(glm::vec3(0,0,-16), angle, hwRelation, near, far));
		cameras.at(1)->Pitch(-50.0f);
		//cameras.at(1)->Pitch(-20.0f);
		//	axisMesh = new Shape(axisVertices,sizeof(axisVertices)/sizeof(axisVertices[0]),axisIndices, sizeof(axisIndices)/sizeof(axisIndices[0]));
		pickedShape = -1;
	}

	void Scene::addShape(int Cylparts,int linkPosition,int parent)
	{
		chainParents.push_back(parent);
		shapes.push_back(new Shape(Cylparts,linkPosition, glm::vec4(0.1f, 0.1f, 0.1f, 1)));
	}
	void Scene::addShape(int CylParts,int linkPosition,const std::string& textureFileName,int parent)
	{
		chainParents.push_back(parent);
		shapes.push_back(new Shape(CylParts,linkPosition,textureFileName, glm::vec4(0.1f, 0.1f, 0.1f, 1)));
	}

	void Scene::addShape(int type,int parent)
	{
		chainParents.push_back(parent);
		shapes.push_back(new Shape(*shapes[type]));
	}

	void Scene::addShape(const Shape& shape, int parent)
	{
		chainParents.push_back(parent);
		shapes.push_back(new Shape(shape));
	}

	void Scene::addShape(const std::string& fileName,int parent)
	{
		chainParents.push_back(parent);
		shapes.push_back(new Shape(fileName, glm::vec4(0.1f, 0.1f, 0.1f, 1)));
	}

	void Scene::addShape(const std::string& fileName,const std::string& textureFileName,int parent)
	{
		chainParents.push_back(parent);
		shapes.push_back(new Shape(fileName,textureFileName, glm::vec4(0.1f, 0.1f, 0.1f, 1)));
	}

	void Scene::addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices,int parent)
	{
		chainParents.push_back(parent);
		shapes.push_back(new Shape(vertices,numVertices,indices,numIndices, glm::vec4(0.1f, 0.1f, 0.1f, 1)));
	}
	
	void Scene::addShape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices, const std::string &textureFlieName,int parent)
	{
		chainParents.push_back(parent);
		shapes.push_back(new Shape(vertices,numVertices,indices,numIndices,textureFlieName, glm::vec4(0.1f, 0.1f, 0.1f, 1)));
	}

	mat4 Scene::GetShapeTrans(int j) {
		return shapes[j]->makeTrans();
	}
	void Scene::addShader(const std::string& fileName)
	{
		shaders.push_back(new Shader(fileName));
		if(!axisMesh)
		{	
			axisMesh = new Shape(axisVertices,sizeof(axisVertices)/sizeof(axisVertices[0]),axisIndices, sizeof(axisIndices)/sizeof(axisIndices[0]), glm::vec4(0.1f, 0.1f, 0.1f, 1));
			axisMesh->myScale(vec3(2.0*scaleFactor,2.0*scaleFactor,2.0*scaleFactor));
			axisMesh->myTranslate(vec3(0,0,0.5),1);
		}
	}

	mat4 Scene::GetViewProjection(int indx) const
	{
		return cameras[indx]->GetViewProjection();
	}

	mat4 Scene::GetShapeTransformation() const
	{
		return shapes[pickedShape]->makeTrans();
	}

	void Scene::draw(int shaderIndx,int cameraIndx,bool drawAxis)
	{
		std::vector<glm::mat4> rotates_snake;

		glm::mat4 Normal = makeTrans();
		glm::mat4 MVP = cameras[cameraIndx]->GetViewProjection() * Normal;
		shaders[shaderIndx]->Bind();
	
		for(int i = 0; i < shapes.size() - 1; i++)
		{
			mat4 Normal1 = mat4(1);
			for (int j = i; chainParents[j] > -1; j = chainParents[j])
			{
				Normal1 = shapes[chainParents[j]]->makeTrans() * Normal1;
			}
			mat4 MVP1 = MVP * Normal1;
		}



		for (int i = 0; i < shapes.size(); i++)
		{
			//int j = i;
//			int counter = 0;
			mat4 Normal1 = mat4(1);
			for (int j = i; chainParents[j] > -1; j = chainParents[j])
			{
				Normal1 = shapes[chainParents[j]]->makeTrans() * Normal1;
			}


			mat4 MVP1 = MVP * Normal1;
			Normal1 = Normal * Normal1;

			if (shaderIndx == 0 && drawAxis && chainParents[i] >= 0)
			{
				shaders[shaderIndx]->Update(axisMesh->makeTransScale(MVP1), axisMesh->makeTransScale(Normal1), 0, rotates_snake, &shapes[0]->ambient, &cameras[0]->getPos());
				axisMesh->draw(GL_LINES);
			}

			MVP1 = MVP1 * shapes[i]->makeTransScale(mat4(1));
			rotates_snake.push_back(MVP1);

			Normal1 = Normal1 * shapes[i]->makeTrans();

			shaders[shaderIndx]->Update(MVP1, Normal1, i, rotates_snake, &shapes[0]->ambient, &cameras[0]->getPos());
			

			if (shapes[i]->toDraw) {
				if (shaderIndx == 1)
					shapes[i]->draw(GL_TRIANGLES);
				else
					shapes[i]->draw(GL_TRIANGLES);
			}
			if (shaderIndx == 0)
			{
				shaders[shaderIndx]->Bind();
				shaders[shaderIndx]->Update(cameras[0]->GetViewProjection()*scale(vec3(10, 10, 10)), Normal*scale(vec3(10, 10, 10)), 0, rotates_snake, &shapes[0]->ambient, &cameras[0]->getPos());
				axisMesh->draw(GL_LINES);
			}
		}
	}

	glm::mat4 Scene::maketranslatetoshape(int indx)
	{
		return shapes[indx]->makeTrans();

	}

		void Scene::shapeRotation(vec3 v, float ang,int indx)
		{
			if(v.x >0.999)
				shapes[indx]->myRotate(ang,v,xAxis1);
			else 
				if(v.z >0.999)
					shapes[indx]->myRotate(ang,v,zAxis12);
			else
				shapes[indx]->myRotate(ang,v,-1);
		}

	void Scene::shapeTransformation(int type,float amt)
	{
		    vec3 newAxis;
			switch (type)
			{
			case xLocalTranslate:
				if(pickedShape ==-1)
					myTranslate(vec3(amt,0,0),1);
				else
				{
					int i = pickedShape;
					for (; chainParents[i] > -1; i = chainParents[i]);
					shapes[i]->myTranslate(vec3(amt,0,0),1);
				}
				break;
			case yLocalTranslate:
				if(pickedShape ==-1)
					myTranslate(vec3(0,amt,0),1);
				else
								{
						int i = pickedShape;
						for (; chainParents[i] > -1; i = chainParents[i]);
						shapes[i]->myTranslate(vec3(0,amt,0),1);
					}
				break;
			case zLocalTranslate:
				if(pickedShape ==-1)
					myTranslate(vec3(0,0,amt),1);
				else
				{
					int i = pickedShape;
					for (; chainParents[i] > -1; i = chainParents[i]);
					shapes[i]->myTranslate(vec3(0,0,amt),1);
				}
				break;
			case xGlobalTranslate:
				if(pickedShape ==-1)
					myTranslate(vec3(amt/5.0,0,0),0);
				else
				{
					int i = pickedShape;
					for (; chainParents[i] > -1; i = chainParents[i]);
					shapes[i]->myTranslate(vec3(amt,0,0),0);
				}
				break;
			case yGlobalTranslate:
				if(pickedShape ==-1)
					myTranslate(vec3(0,amt/5.0,0),0);
				else
				{
					int i = pickedShape;
					for (; chainParents[i] > -1; i = chainParents[i]);
					shapes[i]->myTranslate(vec3(0,amt,0),0);
				}
				break;
			case zGlobalTranslate:
				if(pickedShape ==-1)
					myTranslate(vec3(0,0,amt/5.0),0);
								else
				{
					int i = pickedShape;
					for (; chainParents[i] > -1; i = chainParents[i]);
					shapes[i]->myTranslate(vec3(0,0,amt),0);
				}
				break;
			case xLocalRotate:
				if(pickedShape ==-1)
					myRotate(amt,vec3(1,0,0),xAxis1);
				else
					shapes[pickedShape]->myRotate(amt,vec3(1,0,0),xAxis1);
				break;
			case yLocalRotate:
				if(pickedShape ==-1)
					myRotate(amt,vec3(0,1,0),-1);
				else
					shapes[pickedShape]->myRotate(amt,vec3(0,1,0),-1);
				break;
			case zLocalRotate:
				if(pickedShape ==-1)
					myRotate(amt,vec3(0,0,1),zAxis12);
				else
					shapes[pickedShape]->myRotate(amt,vec3(0,0,1),zAxis12);
			break;
			case xGlobalRotate:
				if(pickedShape ==-1)
					globalSystemRot(amt,vec3(1,0,0),xAxis1);
				else
					shapes[pickedShape]->globalSystemRot(amt,vec3(1,0,0),xAxis1);
				break;
			case yGlobalRotate:
				if(pickedShape ==-1)
					globalSystemRot(amt,vec3(0,1,0),-1);
				else
					shapes[pickedShape]->globalSystemRot(amt,vec3(0,1,0),-1);
				break;
			case zGlobalRotate:
				if(pickedShape ==-1)
					globalSystemRot(amt,vec3(0,0,1),zAxis12);
				else
					shapes[pickedShape]->globalSystemRot(amt,vec3(0,0,1),zAxis12);
			break;
			case xScale:
				if(pickedShape ==-1)
					myScale(vec3(amt,1,1));
				else
					shapes[pickedShape]->myScale(vec3(amt,1,1));
			break;
			case yScale:
				if(pickedShape ==-1)
					myScale(vec3(1,amt,1));
				else
					shapes[pickedShape]->myScale(vec3(1,amt,1));
			break;
			case zScale:
				if(pickedShape ==-1)
					myScale(vec3(1,1,amt));
				else
					shapes[pickedShape]->myScale(vec3(1,1,amt));
			break;
			case xCameraTranslate: //camera plane translate
				if(pickedShape == -1)
					myTranslate(vec3(amt/5.0,0,0),0);
				else
				{
					//newAxis = findAxis(vec3(1,0,0));					
						int i = pickedShape;
					for (; chainParents[i] > -1; i = chainParents[i]);
				
					shapes[i]->translateInSystem(*this,vec3(amt,0,0),0,false);
				}
				break;
			case yCameraTranslate:
				if(pickedShape ==-1)
					myTranslate(vec3(0,amt/5.0,0),0);
				else
					{
						//newAxis = findAxis(vec3(0,1,0));
							int i = pickedShape;
						for (; chainParents[i] > -1; i = chainParents[i]);
						
						shapes[i]->translateInSystem(*this,vec3(0,amt,0),0,false);
					}
			break;
			case zCameraTranslate:
				if(pickedShape ==-1)
					myTranslate(vec3(0,0,amt/5.0),0);
				else
					{
					//	newAxis = findAxis(vec3(0,0,1));
							int i = pickedShape;
						for (; chainParents[i] > -1; i = chainParents[i]);
			
						shapes[i]->translateInSystem(*this,vec3(0,0,amt),0,false);
					}
			break;
			default:
				break;
			}

		
	}
	



	bool Scene::equ(Node* A, Node * B, glm::vec3 L, glm::mat4 trt1, glm::mat4 trt2) {

		float res = 0;
		if (A == nullptr || B == nullptr || A == NULL || B == NULL)
		{
			return false;
		}
		glm::vec4 center1 = trt1 * A->center;
		glm::vec4 center2 = trt2 * B->center;

		glm::vec3 T = glm::vec3(center1.x - center2.x, center1.y - center2.y, center1.z - center2.z);
		float dotres = glm::dot(T, L);
		float absdot = glm::abs(dotres);

		glm::vec3 AxisAx = A->halfwidthX*glm::vec3(trt1*glm::vec4(A->axisX, 0));
		float fi = glm::dot(AxisAx, L);
		float fiabsdot = glm::abs(fi);

		glm::vec3 AxisAy = A->halfhightY*glm::vec3(trt1*glm::vec4(A->axisY, 0));
		float se = glm::dot(AxisAy, L);
		float seabsdot = glm::abs(se);

		glm::vec3 AxisAz = A->halfdepthZ*glm::vec3(trt1*glm::vec4(A->axisZ, 0));
		float th = glm::dot(AxisAz, L);
		float thabsdot = glm::abs(th);

		glm::vec3 AxisBx = B->halfwidthX*glm::vec3(trt1*glm::vec4(B->axisX, 0));
		float fiB = glm::dot(AxisBx, L);
		float fiBabsdot = glm::abs(fiB);

		glm::vec3 AxisBy = B->halfhightY*glm::vec3(trt1*glm::vec4(B->axisY, 0));
		float seB = glm::dot(AxisBy, L);
		float seBabsdot = glm::abs(seB);

		glm::vec3 AxisBz = B->halfdepthZ*glm::vec3(trt1*glm::vec4(B->axisZ, 0));
		float thB = glm::dot(AxisBz, L);
		float thBabsdot = glm::abs(thB);

		res = fiabsdot + seabsdot + thabsdot + fiBabsdot + seBabsdot + thBabsdot;


		return absdot > res;

	}




	bool Scene::collide(Node* A, Node* B, glm::mat4 trt1, glm::mat4 trt2)
	{
		if (A == nullptr || B == nullptr || A == NULL || B == NULL)
		{
			return false;
		}
		glm::vec3 L;
		L = glm::vec3(trt1*glm::vec4(A->axisX, 0));

		if (equ(A, B, L, trt1, trt2)) {
			return false;
		}
		L = glm::vec3(trt1*glm::vec4(A->axisY, 0));

		if (equ(A, B, L, trt1, trt2)) {

			return false;
		}
		L = glm::vec3(trt1*glm::vec4(A->axisZ, 0));;

		if (equ(A, B, L, trt1, trt2)) {

			return false;
		}
		L = glm::vec3(trt1*glm::vec4(B->axisX, 0));;

		if (equ(A, B, L, trt1, trt2)) {

			return false;
		}
		L = glm::vec3(trt1*glm::vec4(A->axisY, 0));
		if (equ(A, B, L, trt1, trt2)) {

			return false;
		}
		L = glm::vec3(trt1*glm::vec4(A->axisZ, 0));

		if (equ(A, B, L, trt1, trt2)) {
			return false;
		}
		L = glm::vec3(trt1*glm::vec4(A->axisX, 0))*glm::vec3(trt1*glm::vec4(B->axisX, 0));
		if (equ(A, B, L, trt1, trt2)) {
			return false;
		}
		L = glm::vec3(trt1*glm::vec4(A->axisX, 0))*glm::vec3(trt1*glm::vec4(B->axisY, 0));
		if (equ(A, B, L, trt1, trt2)) {
			return false;
		}
		L = glm::vec3(trt1*glm::vec4(A->axisX, 0))*glm::vec3(trt1*glm::vec4(A->axisZ, 0));
		if (equ(A, B, L, trt1, trt2)) {
			return false;
		}
		L = glm::vec3(trt1*glm::vec4(A->axisY, 0))*glm::vec3(trt1*glm::vec4(B->axisX, 0));

		if (equ(A, B, L, trt1, trt2)) {
			return false;
		}
		L = glm::vec3(trt1*glm::vec4(A->axisY, 0))*glm::vec3(trt1*glm::vec4(B->axisY, 0));
		if (equ(A, B, L, trt1, trt2)) {
			return false;
		}
		L = glm::vec3(trt1*glm::vec4(A->axisY, 0))*glm::vec3(trt1*glm::vec4(B->axisZ, 0));
		if (equ(A, B, L, trt1, trt2)) {
			return false;
		}
		L = glm::vec3(trt1*glm::vec4(A->axisZ, 0))*glm::vec3(trt1*glm::vec4(B->axisX, 0));
		if (equ(A, B, L, trt1, trt2)) {
			return false;
		}
		L = glm::vec3(trt1*glm::vec4(A->axisZ, 0))*glm::vec3(trt1*glm::vec4(B->axisY, 0));
		if (equ(A, B, L, trt1, trt2)) {
			return false;
		}
		L = glm::vec3(trt1*glm::vec4(A->axisZ, 0))*glm::vec3(trt1*glm::vec4(B->axisZ, 0));
		if (equ(A, B, L, trt1, trt2)) {
			return false;
		}
		return true;
	}


	bool Scene::find_collision(int index1, int index2) {
		std::queue<std::pair<Node*, Node*>> queue1;
		queue1.push(std::pair<Node*, Node*>(this->shapes[index1]->mesh->get_kdtree().getRoot(), this->shapes[index2]->mesh->get_kdtree().getRoot()));

		glm::mat4 Normal1 = glm::mat4(1);

		for (int j = index1; this->get_chainParents()[j] > -1; j = this->get_chainParents()[j])
		{
			Normal1 = this->shapes[this->get_chainParents()[j]]->makeTrans() * Normal1;
		}
		Normal1 = Normal1 * this->shapes[index1]->makeTransScale(glm::mat4(1));

		glm::mat4 Normal2 = glm::mat4(1);

		for (int j = index2; this->get_chainParents()[j] > -1; j = this->get_chainParents()[j])
		{
			Normal2 = this->shapes[this->get_chainParents()[j]]->makeTrans() * Normal1;
		}
		Normal2 = Normal2 * this->shapes[index2]->makeTransScale(glm::mat4(1));



		while (!queue1.empty()) {
			std::pair<Node*, Node*> front = queue1.front();
			queue1.pop();


			if (collide(front.first, front.second, Normal1, Normal2)) {
				//std::cout << "First loop " << front.first->leaf_father() << std::endl;
				if (index2>=9 && index2<20) {
					collidesnake = true;

				}
				if (front.first->leaf_father() && front.second->leaf_father()) {
					//		scn.to_draw->push(std::pair<int, Node*>(index1, front.first));
					//	scn.to_draw->push(std::pair<int, Node*>(index2, front.second));
					//std::cout << "collision" << std::endl;
					return true;
				}

				else if (front.second->leaf_father() && front.first != nullptr) {
					queue1.push(std::pair<Node*, Node*>(front.first->left, front.second));
					queue1.push(std::pair<Node*, Node*>(front.first->right, front.second));
				}
				else if (front.first->leaf_father() && front.second != nullptr) {
					queue1.push(std::pair<Node*, Node*>(front.first, front.second->left));
					queue1.push(std::pair<Node*, Node*>(front.first, front.second->right));
				}
				else {
					queue1.push(std::pair<Node*, Node*>(front.first->left, front.second->left));
					queue1.push(std::pair<Node*, Node*>(front.first->right, front.second->left));
					queue1.push(std::pair<Node*, Node*>(front.first->left, front.second->right));
					queue1.push(std::pair<Node*, Node*>(front.first->right, front.second->right));
				}
			}
		}
		return false;
	}


	void Scene::resize(int width,int height,int near,int far)
	{
		glViewport(0,0,width,height);
		cameras[0]->setProjection((float)width/(float)height,near,far);
	}

	float Scene::picking(double x,double y)
	{
		float depth;
		glClearColor(0.0,0.0,0.0,0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		draw(1,0,false); 
						
		GLint viewport[4];  
		unsigned char data[4];
		glGetIntegerv(GL_VIEWPORT, viewport); //reading viewport parameters
		glReadPixels(x,viewport[3] - y,1,1, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glReadPixels(x,viewport[3] - y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&depth);
		cout<<"depth "<<depth<<endl;
		int pickedID = data[0] + data[1]*256 + data[2]*256*256 - 1;
		if(data[0] == 0 && data[1] == 0 && data[2] == 0)
		{
			pickedShape = -1;
			//std::cout<<"not picked"<<std::endl;
		}
		else
		{
			pickedShape = pickedID;
			std::cout<<"picked "<<pickedID<<std::endl;
		}
		return depth;
	}

	vec3 Scene::getTipPosition(int indx)
	{
		mat4 Normal1 = mat4(1);
		if(indx>-1)
		{
			for (int j = indx;  chainParents[j] > -1; j = chainParents[j])
			{
				Normal1 =  shapes[chainParents[j]]->makeTrans() * Normal1;
			}
			return shapes[indx]->getPointInSystem(Normal1,vec3(0,0,1));
			//return shapes[indx]->getTipPos(Normal1);
		}
		else
		{
			return shapes[0]->getPointInSystem(mat4(1),vec3(0,0,-1)); 
				//shapes[0]->getRootPos(mat4(1));
		}
	}

	vec3 Scene::getCenterPosition(int indx,glm::vec3 p)
	{
		mat4 Normal1 = mat4(1);
		if (indx>-1)
		{
			for (int j = indx; chainParents[j] > -1; j = chainParents[j])
			{
				Normal1 = shapes[chainParents[j]]->makeTrans() * Normal1;
			}
			return shapes[indx]->getPointInSystem(Normal1, p);
			//return shapes[indx]->getTipPos(Normal1);
		}
		else
		{
			return shapes[0]->getPointInSystem(mat4(1), vec3(0, 0, 0));
			//shapes[0]->getRootPos(mat4(1));
		}
	}


	std::vector<Shape*> Scene::get_shapes() {
		return this->shapes;
	}

	vec3 Scene::getDistination(int indx)
	{
		mat4 Normal1 = mat4(1);
		if( indx>-1)
		{
			for (int j = indx; chainParents[j] > -1; j = chainParents[j])
			{
				Normal1 =  shapes[chainParents[j]]->makeTrans() * Normal1;
			}
			return shapes[indx]->getPointInSystem(Normal1,vec3(0,0,0));
			//return shapes[indx]->getCenterOfRotation(Normal1);
		}
		else
		{
			return vec3(0,0,0);
		}
	}
	void Scene::init_angle(int linksNum) {
		for (int i = 1; i < linksNum; i++) {
			//shapes[i]->phi = glm::mat4(1);
		//	shapes[i]->theta = glm::mat4(1);
			//shapes[i]->psi = glm::mat4(1);
			shapes[i]->reset_euler();
		}
	}

	glm::vec3 Scene::getVectorInSystem(int i,glm::mat4 &prevTransformations, glm::vec3 point)
	{
		return shapes[i]->getVectorInSystem(prevTransformations, point);
	}

	void Scene::build_new_rotate(int index, int axis, float cos, float sin) {		
		shapes[index]->rot(axis,cos,sin);
	}

	void Scene::shape_rotate(int index, float ang, glm::vec3 vec) {
		shapes[index]->myRotate(ang,vec,-1);
	}

	vec3 Scene::getAxisDirection(int indx,int axis)
	{
		if(axis == xAxis)
		{
			mat4 Normal1 = mat4(1);
			for (int j = indx; chainParents[j] > -1; j = chainParents[j])
			{
				Normal1 =  shapes[chainParents[j]]->makeTrans() * Normal1;
			}
			return shapes[indx]->getPointInSystem(Normal1,vec3(1,0,0)); 
				//shapes[indx]->getXdirection(Normal1);
		}
		else
		{
			mat4 Normal1 = mat4(1);
			for (int j = indx; chainParents[j] > -1; j = chainParents[j])
			{
				Normal1 =  shapes[chainParents[j]]->makeTrans() * Normal1;
			}
			return shapes[indx]->getVectorInSystem(Normal1,vec3(0,0,1)); 
				//shapes[indx]->getZdirection(Normal1);
		}

	}
	Scene::~Scene(void)
{
	for (Shape* shp : shapes)
		{
			delete shp;
		}
	for (Camera* cam : cameras)
		{
			delete cam;
		}
	for(Shader* sdr: shaders)
		{
			delete sdr;
		}
	delete axisMesh;
}
