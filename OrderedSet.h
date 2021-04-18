#pragma once
#include "Food.h"

struct Node
{
	Food* food;
	int height;
	Node* left;
	Node* right;

	Node() : height(1), left(nullptr), right(nullptr) {}
	Node(Food* f, int height_ = 1) : food(f), height(height_), left(nullptr), right(nullptr) {}
};

class OrderedSet
{
public:
	int getHeight(Node* node);
	int getBalance(Node* root);
	Node* insert(Node* root, Food* f);
	Node* searchID(Node* root, int ID);
	void traversePreorder(Node* root);
	Node* rotateLeft(Node* root);
	Node* rotateRight(Node* root);
	Node* rotateLeftRight(Node* root);
	Node* rotateRightLeft(Node* root);
};

int OrderedSet::getHeight(Node* node)
{
	if (node == nullptr)
		return 0;
	return node->height;
}

int OrderedSet::getBalance(Node* root)
{
	if (root == nullptr)
		return 0;
	return getHeight(root->left) - getHeight(root->right);
}

Node* OrderedSet::insert(Node* root, Food* f)
{
	if (root == nullptr)
	{
		Node* node = new Node(f);
		return node;
	}
	if (f->id == root->food->id)
		return root;
	else if (f->id < root->food->id)
		root->left = insert(root->left, f);
	else
		root->right = insert(root->right, f);

	root->height = max(getHeight(root->left), getHeight(root->right)) + 1;

	// Get balance factor and check if unbalanced
	int balance = getBalance(root);

	if (balance > 1 && f->id < root->left->food->id) // left left case
		return rotateRight(root);

	else if (balance < -1 && f->id > root->right->food->id) // right right case
		return rotateLeft(root);

	else if (balance > 1 && f->id > root->left->food->id) // left right case
		return rotateLeftRight(root);

	else if (balance < -1 && f->id < root->right->food->id) // right left case
		return rotateRightLeft(root);

	return root;
}

Node* OrderedSet::searchID(Node* root, int ID)
{
	if (root == nullptr)
		return nullptr;
	else if (ID == root->food->id)
		return root;
	else if (ID < root->food->id)
		return searchID(root->left, ID);
	else
		return searchID(root->right, ID);
}

void OrderedSet::traversePreorder(Node* root) // Traverse whole tree
{
	if (root == nullptr)
		return;
	else
	{
		traversePreorder(root->left);
		traversePreorder(root->right);
	}
}

Node* OrderedSet::rotateLeft(Node* root)
{
	Node* grandchild = root->right->left;
	Node* newParent = root->right;
	newParent->left = root;
	root->right = grandchild;

	root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
	newParent->height = max(getHeight(newParent->left), getHeight(newParent->right)) + 1;

	return newParent;
}

Node* OrderedSet::rotateRight(Node* root)
{
	Node* grandchild = root->left->right;
	Node* newParent = root->left;
	newParent->right = root;
	root->left = grandchild;

	root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
	newParent->height = max(getHeight(newParent->left), getHeight(newParent->right)) + 1;

	return newParent;
}

Node* OrderedSet::rotateLeftRight(Node* root)
{
	Node* child = root->left;
	root->left = rotateLeft(child);

	return rotateRight(root);
}

Node* OrderedSet::rotateRightLeft(Node* root)
{
	Node* child = root->right;
	root->right = rotateRight(child);

	return rotateLeft(root);
}