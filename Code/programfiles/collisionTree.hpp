#ifndef COLLISION_TREE_H__
#define COLLISION_TREE_H__

#include <iostream>
#include <string>
#include <cstddef>
#include "collision.hpp"

class Node{
public:
	int flags;
	Collidable *data;
	std::vector<Node *> children;

	Node(){
		flags = -1;
		data = nullptr;
	};
	Node(Collidable *c, int flags){
		flags = flags;
		data = c;
	};
	Node(Collidable *c){
		data = c;
		flags = 0;
	}

	std::string toString(){
		std::string str;
		str.append(std::to_string(flags));
		str.append((*data).toString());
		for (auto child: children){
			str.append((*child).toString());
		}
		return str;
	}
};

class CollisionTree{

	private:
	Node *root;
	Node *getNodeHelper(Node *n, Collidable *value);

	public:
	CollisionTree();
	~CollisionTree();
	CollisionTree(Collidable *value);

	void removeNode(Node *node);
	void destroyTree();
	void addNode(Collidable *parent);
	Node *getNode(Collidable *value);

};

#endif