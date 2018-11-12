#include <stdio.h>
#include "bst.h"

BST::Node::Node(const int key):
	child { nullptr, nullptr }, key(key) {}

void BST::Node::PrintInorder() const
{
	if (child[0])
		child[0]->PrintInorder();

	printf("%d ", key);

	if (child[1])
		child[1]->PrintInorder();
}

void BST::Cleanup(Node *node)
{
	if (node == nullptr)
		return;

	Cleanup(node->child[0]);
	Cleanup(node->child[1]);

	delete node;
}

BST::Node *BST::Copy(Node *node)
{
	if (node == nullptr)
		return nullptr;

	Node *copy = new Node(node->key);

	copy->child[0] = Copy(node->child[0]);
	copy->child[1] = Copy(node->child[1]);

	return copy;
}

BST::BST(): root(nullptr) {}

BST::BST(const BST &other)
{
	root = Copy(other.root);
}

BST::BST(BST &&other): root(other.root)
{
	other.root = nullptr;
}

BST::~BST()
{
	Cleanup(root);
}

void BST::Insert(const int key)
{
	Node *current = root;

	if (current == nullptr) {
		root = new Node(key);
		return;
	}

	Node *parent = nullptr;
	bool right = false;

	do {
		// Only unique keys are allowed
		if (key == current->key)
			return;

		parent = current;
		right = key > current->key;
		current = current->child[right];
	} while (current);

	current = new Node(key);

	if (parent)
		parent->child[right] = current;
}

void BST::Remove(const int key)
{
	Node *current = root;
	Node *parent = nullptr;
	bool right = false;

	while (current) {
		if (key == current->key)
			break;

		parent = current;
		right = key > current->key;
		current = current->child[right];
	}

	if (current == nullptr)
		return;

	// Case 1: no children
	if (!current->child[0] && !current->child[1]) {
		if (parent)
			parent->child[right] = nullptr;

		return delete current;
	}

	// Case 2: Two children
	if (current->child[0] && current->child[1]) {
		// Find inorder predecessor
		Node *pred = current->child[0];
		parent = current;

		while (pred->child[1]) {
			parent = pred;
			pred = pred->child[1];
		}

		current->key = pred->key; // Replace current
		parent->child[0] = pred->child[0]; // Potential left child

		return delete pred;
	}

	// Case 3: One child
	right = current->child[1];
	Node *single = current->child[right];
	*current = *single;
	delete single;
}

BST::Node *BST::Find(const int key) const
{
	Node *current = root;

	while (current && key != current->key)
		current = current->child[key > current->key];

	return current;
}

void BST::Print() const
{
	if (root == nullptr)
		return;

	root->PrintInorder();
	printf("\n");
}
