/*
*
* k-d tree declaration file
*
* author: Arvind Rao
* license: GNU
*
*
*/

#ifndef KDTREE_
#define KDTREE_
#define _USE_MATH_DEFINES

#include <memory>
#include <vector>
#include <list>
#include <cmath>
#include <queue>
#include <utility>
#include <glm\glm.hpp>

const int N = 3;
class Node
{
public:
	typedef glm::vec4 vecType;
	Node* left;// = std::unique_ptr<Node>( new Node(3) );
	Node* right; //(new Node(3));
	vecType data;
	glm::vec4 center;
	glm::vec3 axisX = glm::vec3(1, 0, 0);
	glm::vec3 axisY = glm::vec3(0, 1, 0);
	glm::vec3 axisZ = glm::vec3(0, 0, 1);
	float halfwidthX;
	float halfhightY;
	float halfdepthZ;



	//default constructor
	Node(int _n);

	//copy constructor
	Node(int _n, vecType _data);

	//default deconstructor
	~Node();

	bool is_leaf();
	bool leaf_father();

};

class Kdtree
{

public:
	//typedef float                numType;
	typedef Node::vecType vecType;
	void Kdtree::findCenter(Node* head, std::list<Kdtree::vecType>& plist);
	//default constructor
	Kdtree();

	//default deconstructor
	~Kdtree();

	/*
	*   Return the tree root node
	*/
	Node* getRoot() const { return root; };
	/*
	* support function for printTree
	*/
	void print_data(vecType pt);

	/*  prints the tree
	*   and really works best for small trees
	*   as a test of tree construction.
	*/
	void printTree(Node* head);

	//function for finding the median of a list of points
	vecType findMedian(int axis, std::list<vecType> &plist, std::list<vecType> &left,
		std::list<vecType> &right);
	//function for making the tree
	void makeTree(std::list<vecType> &plist);


private:
	//helper for makeTree 
	void _makeTree(Node* head, std::list<vecType> &plist, int depth);

	Node* root;
};


#endif