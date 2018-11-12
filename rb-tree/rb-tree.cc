#include <stdio.h>
#include "rb-tree.h"

RBTree::Node::Node(const int key, const bool black):
	child { nullptr, nullptr }, black(black), key(key) {}

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

void RBTree::Print() const
{
	if (root == nullptr)
		return;

	root->PrintInorder(true);
	printf("\n");
}
