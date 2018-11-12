#ifndef RB_TREE_H
#define RB_TREE_H

struct RBTree {
	struct Node {
		Node *child[2];
		bool black;
		int key;

		Node(const int key, const bool black = false);

		size_t Count() const;
		void PrintInorder(bool root = false) const;
		size_t ValidatePostorder() const;

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

	static void Cleanup(Node *node);
	static Node *Copy(Node *node);

	RBTree();
	RBTree(const RBTree &other); // Copy constructor
	RBTree(RBTree &&other); // Move constructor
	~RBTree();

	void Insert(const int key);
	Node *Find(const int key) const;
	size_t Count() const;
	void Print() const;
	void Validate() const;

private:
	RBTree &operator=(RBTree &other); // Copy assignment operator
	RBTree &operator=(RBTree &&other); // Move assignment operator
};

#endif
