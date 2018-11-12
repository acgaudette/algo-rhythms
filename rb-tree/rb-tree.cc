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

void RBTree::Remove(const int key)
{
#ifdef DEBUG
	printf("Removing %d...\n", key);
	size_t prev_count = Count();
#endif

	Node *current = root;

	if (current == nullptr)
		return;

	// Create dummy head for root rotations
	Node head(0);
	head.child[1] = root;

	Node *parent[2] = { &head, nullptr };
	Node *target = nullptr;

	bool right[2] = { true, false };

	do {
		right[1] = right[0];

		// Defaults to true after key is found;
		// leads to inorder predecessor.
		right[0] = key > current->key;

		/* Attempt to push red nodes down from the root */

		bool inline_black = is_black(current)
			&& is_black(current->child[right[0]]);

		if (!inline_black)
			goto CONTINUE;

		// All three are black
		if (is_black(current->child[!right[0]])) {
			Node *sibling = parent[0]->child[!right[1]];

			// If there is no sibling, do nothing.
			if (sibling == nullptr)
				goto CONTINUE;

			// Get the children of the sibling
			Node *child_0 = sibling->child[ right[1]];
			Node *child_1 = sibling->child[!right[1]];

			// If all children are black, do a color flip
			// (the reverse of insert case 1).
			if (is_black(child_0) && is_black(child_1)) {
#ifdef DEBUG
				printf("Fixup: Case 1\n");
#endif
				// "Push" the red node from the parent
				// into the current node and its sibling.
				parent[0]->black = true;
				sibling->black = false;
				current->black = false;
			} else {
				bool side = parent[1]->child[1] == parent[0];
				Node *new_parent;

				// Sibling child is inline;
				// rotate it towards the current node.
				if (!is_black(child_1)) {
#ifdef DEBUG
					printf("Fixup: Case 4\n");
#endif
					new_parent // Rotate parent
						= parent[0]->rotate(right[1]);

					// Update grandparent's child pointer
					parent[1]->child[side]
						= new_parent;
				}

				// Sibling child is not inline;
				// make it inline and then rotate around.
				else { // child_0 must be red
#ifdef DEBUG
					printf("Fixup: Case 3 -> Case 4\n");
#endif
					new_parent
						= parent[0]->rotate2(right[1]);
					parent[1]->child[side]
						= new_parent;
				}

				// Recolor
				new_parent->black = false;
				current->black = false; // Push red down
				// One of these is parent[0]; both are black
				new_parent->child[0]->black = true;
				new_parent->child[1]->black = true;
			}
		} else { // The non-inline child is red
#ifdef DEBUG
			printf("Fixup: Case 2\n");
#endif
			// Rotate the red child inline with the other child
			// along our direction of movement.
			Node *new_current = current->rotate(right[0]);
			parent[0]->child[right[1]] = new_current;

			// Update parent pointer
			parent[0] = new_current;

			// Recolor
			current->black = false;
			new_current->black = true;
		}

CONTINUE:
		// We found the node to delete
		if (current->key == key)
			target = current;

		parent[1] = parent[0];
		parent[0] = current;

		current = current->child[right[0]];
	} while (current);

	/* At this point, parent[0] is the last valid current (a leaf).
	 * parent[1] is the parent of the leaf.
	 */

	// Remove or replace target
	if (target) {
		// Copy data
		target->key = parent[0]->key;

		bool right_leaf = parent[0] == parent[1]->child[1];

		/* Works for all cases.
		 * 1. No children:
		 *	parent[0] == target (a leaf node; no children)
		 *	parent[1]'s child pointer to target is cleared
		 * 2. Two children:
		 *	parent[0] is target's inorder predecessor (a leaf node)
		 *	parent[1]'s child pointer to parent[0] is cleared
		 * 3. One child:
		 *	parent[0] is either equal to target or its predecessor
		 *	a. parent[0] == target:
		 *		The right tree is moved up
		 *	b. parent[0] is target's inorder predecessor
		 *		The left tree is moved up
		 */

		bool right_child = parent[0]->child[0] == nullptr;
		parent[1]->child[right_leaf] = parent[0]->child[right_child];
		delete parent[0];
	}

	// Fix root
	root = head.child[1];
	if (root)
		root->black = true;

#ifdef DEBUG
	Print();

	if (Find(key))
		printf("Invalid: Remove not successful\n");

	size_t count = Count();
	if (count != prev_count - 1 && count != prev_count)
		printf("Invalid: Remove count is incorrect\n");

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
