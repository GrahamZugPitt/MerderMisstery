#include "collisionTree.hpp"

CollisionTree::CollisionTree(){
	root = nullptr;
};

CollisionTree::CollisionTree(Collidable *value){
	root = new Node(value);
};

CollisionTree::~CollisionTree(){
	destroyTree();
};

void CollisionTree::removeNode(Node *node){
	if( node == nullptr) return;
	for (auto child: (*node).children){
		removeNode(child);
	}
	delete node->data;
	delete node;
};

void CollisionTree::destroyTree(){
	if( root == nullptr) return;
	removeNode(root);
};

Node *CollisionTree::getNode(Collidable *c){
	Node *n = root;
	n = getNodeHelper(n, c);
	return n;
}

Node *CollisionTree::getNodeHelper(Node *n, Collidable *c){
	if ( n == nullptr) return n;
	if( n->data == c){ 
		std::cout << n->data << "  |  " << c << std::endl;
		return n;
	} else {
		if (n->children.empty()) return nullptr;
	}
	for (auto child: n->children){
		std::cout << "in loop" << std::endl;
		n = getNodeHelper(child, c);
		if (n != nullptr) break;
	}
	return n;
}

int main(){
	Collidable *c = new Collidable(500, 500, 100, 100);
	Collidable *n = new Collidable(100, 100, 10, 10);
	CollisionTree ct(c);
	Node node;
	if( (ct.getNode(c)) != nullptr )
		std::cout << ct.getNode(c)->toString() << std::endl << n << std::endl;
	if( (ct.getNode(n)) != nullptr )
		std::cout << ct.getNode(n)->toString() << std::endl;
	else
		std::cout << "Not found" << std::endl;
}