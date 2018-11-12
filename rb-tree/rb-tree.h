#ifndef RB_TREE_H
#define RB_TREE_H

struct RBTree {
	struct Node {
		Node *child[2];
		bool black;
		int key;

		Node(const int key, const bool black = false);

		void PrintInorder(bool root = false) const;

		// Returns new root
		inline Node *rotate(bool right)
		{
			Node *swap = child[!right];

			// Switch sides of grandchild
			child[!right] = swap->child[right];
			swap->child[right] = this; // Move new root up

			return swap;
		}

		// Rotate child opposite input direction to inline,
		// then rotate self the other way.
		inline Node *rotate2(bool right)
		{
			child[!right] = child[!right]->rotate(!right);
			return rotate(right);
		}
	};

	Node *root;

	static inline bool is_black(Node *node)
	{
		return node == nullptr || node->black;
	}

	RBTree();

	void Print() const;
};

#endif
