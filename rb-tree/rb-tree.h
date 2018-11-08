#ifndef RB_TREE_H
#define RB_TREE_H

struct RBTree {
	struct Node {
		Node *child[2];
		bool black;
		int key;

		Node(const int key, const bool black = false);
	};

	Node *root;

	RBTree();
};

#endif
