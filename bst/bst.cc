#include <stdio.h>
#include "bst.h"

BST::Node::Node(const int key):
	left(nullptr), right(nullptr), key(key) {}

void BST::Node::PrintInorder() const
{
	if (left != nullptr) {
		left->PrintInorder();
	}

	printf("%d ", key);

	if (right != nullptr) {
		right->PrintInorder();
	}
}

void BST::Cleanup(Node *node)
{
	if (node == nullptr)
		return;

	Cleanup(node->left);
	Cleanup(node->right);

	delete node;
}

BST::Node *BST::Copy(Node *node)
{
	if (node == nullptr)
		return nullptr;

	Node *copy = new Node(node->key);

	copy->left = Copy(node->left);
	copy->right = Copy(node->right);

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
	bool left = false;

	while (current != nullptr) {
		// Only unique keys are allowed
		if (key == current->key)
			return;

		parent = current;
		left = key < current->key;
		current = left ? current->left : current->right;
	}

	current = new Node(key);

	if (parent != nullptr) {
		if (left)
			parent->left = current;
		else
			parent->right = current;
	}
}

void BST::Print() const
{
	if (root == nullptr)
		return;

	root->PrintInorder();
	printf("\n");
}
