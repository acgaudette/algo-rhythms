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

size_t RBTree::Node::ValidatePostorder() const
{
	size_t size_l = child[0] ? child[0]->ValidatePostorder() : 1;
	size_t size_r = child[1] ? child[1]->ValidatePostorder() : 1;

	if (size_l != size_r)
		printf("Invalid: Black height does not match\n");

	if (!black && !is_black(child[0]))
		printf("Invalid: Left child of red parent is red\n");

	if (!black && !is_black(child[1]))
		printf("Invalid: Right child of red parent is red\n");

	if (child[0] && child[0]->key >= key)
		printf("Invalid: Left child has key out of order\n");

	if (child[1] && child[1]->key <= key)
		printf("Invalid: Right child has key out of order\n");

	return size_l + black;
}

RBTree::Node *RBTree::Copy(Node *node)
{
	if (node == nullptr)
		return nullptr;

	Node *copy = new Node(node->key, node->black);

	copy->child[0] = Copy(node->child[0]);
	copy->child[1] = Copy(node->child[1]);

	return copy;
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

RBTree::RBTree(const RBTree &other)
{
	root = Copy(other.root);
}

RBTree::RBTree(RBTree &&other): root(other.root)
{
	other.root = nullptr;
}

RBTree::~RBTree()
{
	Cleanup(root);
}

void RBTree::Insert(const int key)
{
#ifdef DEBUG
	printf("Inserting %d...\n", key);
	size_t prev_count = Count();
#endif

	Node *current = root;

	if (current == nullptr) {
		root = new Node(key, true);
#ifdef DEBUG
		Print();
		printf("\n");
#endif
		return;
	}

	// Create dummy head for root rotations
	Node head(0);
	head.child[1] = root;

	Node *parent[3] = {
		nullptr,
		nullptr,
		&head,
	};

	bool right[2] = { false, false };

	while (true) {
		if (current == nullptr) {
			// Insert
			current = new Node(key);
			parent[0]->child[right[0]] = current;
		}

		// Look for 4-nodes on the way down.
		// These are valid, but we break them up to make
		// insertion easier.
		else {
			Node *child_l = current->child[0];
			Node *child_r = current->child[1];

			// Black parent, two red child
			if (!is_black(child_l) && !is_black(child_r)) {
#ifdef DEBUG
				printf("Violation: Case 1\n");
#endif
				// Flip colors
				current->black = false;
				child_l->black = true;
				child_r->black = true;
			}
		}

		// Cases 2 and 3: can either occur after a 4-node breakup
		// or after a successful insertion.
		if (!is_black(current) && !is_black(parent[0])) {
			// Reuse right[0]
			right[0] = parent[2]->child[1] == parent[1];

			// We know this cannot be a case 1 violation,
			// because we have already broken any 4-nodes.
			// Here we check if the child and parent are inline.
			if (current == parent[0]->child[right[1]]) {
#ifdef DEBUG
				printf("Violation: Case 3\n");
#endif
				Node *new_grandparent
					= parent[1]->rotate(!right[1]);
				parent[2]->child[right[0]] = new_grandparent;

				// Recolor
				parent[1]->black = false;
				new_grandparent->black = true;
			}

			// If they are not inline, we need to do a rotation
			// to make them inline first, followed by another
			// rotation.
			else {
#ifdef DEBUG
				printf("Violation: Case 2 -> Case 3\n");
#endif
				Node *new_grandparent
					= parent[1]->rotate2(!right[1]);
				parent[2]->child[right[0]] = new_grandparent;

				// Recolor
				parent[1]->black = false;
				new_grandparent->black = true;
			}
		}

		// Only unique keys are allowed
		if (key == current->key)
			break;

		if (parent[1]) // Don't mangle the head
			parent[2] = parent[1];

		parent[1] = parent[0];
		parent[0] = current;

		right[1] = right[0];
		right[0] = key > current->key;
		current = current->child[right[0]];
	}

	// Fix root
	root = head.child[1];
	root->black = true;

#ifdef DEBUG
	Print();

	if (Find(key) == nullptr)
		printf("Invalid: Insert not successful\n");

	size_t count = Count();
	if (count != prev_count + 1 && count != prev_count)
		printf("Invalid: Insert count is incorrect\n");

	Validate();
	printf("\n");
#endif
}

RBTree::Node *RBTree::Find(const int key) const
{
	Node *current = root;

	while (current && key != current->key)
		current = current->child[key > current->key];

	return current;
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

void RBTree::Validate() const
{
	if (root == nullptr) {
		return;
	}

	if (!root->black) {
		printf("Invalid: Root is red\n");
		return;
	}

	root->ValidatePostorder();
}
