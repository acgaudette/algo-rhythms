#ifndef BST_H
#define BST_H

struct BST {
	struct Node {
		Node *child[2];
		int key;

		Node(const int key);
		void PrintInorder() const;
	};

	Node *root;

	static void Cleanup(Node *node);
	static Node *Copy(Node *node);

	BST();
	BST(const BST &other); // Copy constructor
	BST(BST &&other); // Move constructor
	~BST();

	void Insert(const int key);
	void Remove(const int key);
	Node *Find(const int key) const;
	void Print() const;

private:
	BST &operator=(BST &other); // Copy assignment operator
	BST &operator=(BST &&other); // Move assignment operator
};

#endif
