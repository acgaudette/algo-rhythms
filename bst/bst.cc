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

	while (current) {
		// Only unique keys are allowed
		if (key == current->key)
			return;

		parent = current;
		right = key > current->key;
		current = current->child[right];
	}

	current = new Node(key);

	if (parent)
		parent->child[right] = current;
}

void BST::Remove(const int key)
{
	Node *entry = root;
	Node *parent = nullptr;
	bool left = false;

	while (entry != nullptr) {
		if (key == entry->key)
			break;

		parent = entry;
		left = key < entry->key;
		entry = left ? entry->left : entry->right;
	}

	if (entry == nullptr)
		return;

	// Case 1: no children
	if (entry->left == nullptr && entry->right == nullptr) {
		if (parent != nullptr) {
			if (left)
				parent->left = nullptr;
			else
				parent->right = nullptr;
		}

		delete entry;
		return;
	}

	// Case 2: Two children
	if (entry->left != nullptr && entry->right != nullptr) {
		// Find inorder predecessor
		Node *pred = entry->left;
		parent = entry;

		while (pred->right != nullptr) {
			parent = pred;
			pred = pred->right;
		}

		entry->key = pred->key; // Replace entry
		parent->left = pred->left; // Potential left child

		delete pred;
		return;
	}

	// Case 3: One child
	Node *next = entry->left == nullptr ?
		entry->right : entry->left;

	entry->key = next->key;
	entry->left = next->left;
	entry->right = next->right;

	delete next;
}

BST::Node *BST::Find(const int key) const
{
	Node *current = root;

	while (current != nullptr) {
		if (key == current->key)
			return current;

		current = key < current->key ?
			current->left : current->right;
	}

	return nullptr;
}

void BST::Print() const
{
	if (root == nullptr)
		return;

	root->PrintInorder();
	printf("\n");
}
