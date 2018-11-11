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

RBTree::RBTree(): root(nullptr) {}

void RBTree::Print() const
{
	if (root == nullptr)
		return;

	root->PrintInorder(true);
	printf("\n");
}
