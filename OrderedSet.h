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
	Node* root;

	int getHeight(Node* node);
	int getBalance(Node* node);
	Node* rotateLeft(Node* node);
	Node* rotateRight(Node* node);
	Node* rotateLeftRight(Node* node);
	Node* rotateRightLeft(Node* node);
	Node* insertAfter(Node* node, Food* f);
	void searchAfter(Node* node, int ID);
	void traversePreorder(Node* node, string ingredients_, vector<int>& ids);
public:
	OrderedSet();
	void insert(Food* f);
	void searchID(int ID);
	vector<int> traverse(string ingredients_);
};

OrderedSet::OrderedSet()
{
	root = nullptr;
}

int OrderedSet::getHeight(Node* node)
{
	if (node == nullptr)
		return 0;
	return node->height;
}

int OrderedSet::getBalance(Node* node)
{
	if (node == nullptr)
		return 0;
	return getHeight(node->left) - getHeight(node->right);
}

Node* OrderedSet::insertAfter(Node* node, Food* f) // insert after root
{
	if (node == nullptr)
	{
		node = new Node(f);
		return node;
	}
	if (f->id == node->food->id)
		return node;
	else if (f->id < node->food->id)
		node->left = insertAfter(node->left, f);
	else
		node->right = insertAfter(node->right, f);

	node->height = max(getHeight(node->left), getHeight(node->right)) + 1;

	// Get balance factor and check if unbalanced
	int balance = getBalance(node);

	if (balance > 1 && f->id < node->left->food->id) // left left case
		return rotateRight(node);

	else if (balance < -1 && f->id > node->right->food->id) // right right case
		return rotateLeft(node);

	else if (balance > 1 && f->id > node->left->food->id) // left right case
		return rotateLeftRight(node);

	else if (balance < -1 && f->id < node->right->food->id) // right left case
		return rotateRightLeft(node);

	return node;
}

void OrderedSet::insert(Food* f)
{
	if (root == nullptr) // first insertion call
	{
		root = new Node(f);
		return;
	}
	else
	{
		if (f->id == root->food->id)
			return;
		else if (f->id < root->food->id)
			root->left = insertAfter(root->left, f);
		else
			root->right = insertAfter(root->right, f);
	}
}

void OrderedSet::searchAfter(Node* node, int ID) // search after root
{
	if (node == nullptr)
		cout << endl;
	else if (ID == node->food->id)
	{
		cout << "Food ID: " << node->food->id << endl;
		cout << "Ingredients: " << node->food->ingredients << endl;
		cout << "Category: " << node->food->category << endl;
		cout << "Brand: " << node->food->brand << endl;
	}
	else if (ID < node->food->id)
		searchAfter(node->left, ID);
	else
		searchAfter(node->right, ID);
}

void OrderedSet::searchID(int ID)
{
	if (root == nullptr)
		cout << endl;
	else if (ID == root->food->id)
	{
		cout << "Food ID: " << root->food->id << endl;
		cout << "Ingredients: " << root->food->ingredients << endl;
		cout << "Category: " << root->food->category << endl;
		cout << "Brand: " << root->food->brand << endl;
	}
	else if (ID < root->food->id)
		searchAfter(root->left, ID);
	else
		searchAfter(root->right, ID);
}

// Traverse tree after root
void OrderedSet::traversePreorder(Node* node, string ingredients_, vector<int>& ids)
{
	if (node == nullptr)
		return;
	else
	{
		size_t found = node->food->ingredients.find(ingredients_);
		if (found != string::npos)
			ids.push_back(node->food->id);
			
		traversePreorder(node->left, ingredients_, ids);
		traversePreorder(node->right, ingredients_, ids);
	}
}

vector<int> OrderedSet::traverse(string ingredients_) // start at root
{
	vector<int> ids;
	if (root != nullptr)
	{
		size_t found = root->food->ingredients.find(ingredients_);
		if (found != string::npos)
			ids.push_back(root->food->id);

		traversePreorder(root->left, ingredients_, ids);
		traversePreorder(root->right, ingredients_, ids);
	}
	return ids;
}

Node* OrderedSet::rotateLeft(Node* node)
{
	Node* grandchild = node->right->left;
	Node* newParent = node->right;
	newParent->left = node;
	node->right = grandchild;

	node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
	newParent->height = max(getHeight(newParent->left), getHeight(newParent->right)) + 1;

	return newParent;
}

Node* OrderedSet::rotateRight(Node* node)
{
	Node* grandchild = node->left->right;
	Node* newParent = node->left;
	newParent->right = node;
	node->left = grandchild;

	node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
	newParent->height = max(getHeight(newParent->left), getHeight(newParent->right)) + 1;

	return newParent;
}

Node* OrderedSet::rotateLeftRight(Node* node)
{
	Node* child = node->left;
	node->left = rotateLeft(child);

	return rotateRight(node);
}

Node* OrderedSet::rotateRightLeft(Node* node)
{
	Node* child = node->right;
	node->right = rotateRight(child);

	return rotateLeft(node);
}