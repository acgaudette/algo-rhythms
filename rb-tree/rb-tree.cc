#include <stdio.h>
#include "rb-tree.h"

RBTree::Node::Node(const int key, const bool black):
	child { nullptr, nullptr }, black(black), key(key) {}

size_t RBTree::Node::Count() const
{
	size_t size_l = child[0] ? child[0]->Count() : 0;
	size_t size_r = child[1] ? child[1]->Count() : 0;

	return size_l + size_r + 1;
}

void RBTree::Node::PrintInorder(bool root) const
{
	if (child[0])
		child[0]->PrintInorder();

	char color = black ? 'B' : 'R';
	printf(" %c%d%c ", root ? '*' : ' ', key, color);

	if (child[1])
		child[1]->PrintInorder();
}

void RBTree::Cleanup(Node *node)
{
	if (node == nullptr)
		return;

	Cleanup(node->child[0]);
	Cleanup(node->child[1]);

	delete node;
}

RBTree::RBTree(): root(nullptr) {}

RBTree::~RBTree()
{
	Cleanup(root);
}

size_t RBTree::Count() const
{
	return root ? root->Count() : 0;
}

void RBTree::Print() const
{
	if (root == nullptr)
		return;

	root->PrintInorder(true);
	printf("\n");
}
