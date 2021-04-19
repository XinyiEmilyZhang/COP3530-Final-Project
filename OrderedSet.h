#pragma once
#include "Food.h"

struct Node // Tree node for AVL tree
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
	void searchID(Node* root, int ID);
	vector<int> traversePreorder(Node* root, string ingredients_, vector<int>& ids);
	vector<int> levelOrder(Node* root, string ingredients_);
	Node* rotateLeft(Node* root);
	Node* rotateRight(Node* root);
	Node* rotateLeftRight(Node* root);
	Node* rotateRightLeft(Node* root);
	void inorder(Node* root);
};

int OrderedSet::getHeight(Node* node) // get height of current node
{
	if (node == nullptr)
		return 0;
	return node->height;
}

int OrderedSet::getBalance(Node* root) // get balance factor
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

void OrderedSet::searchID(Node* root, int ID) // search node given food id
{
	string oldStr = "";
	if (root == nullptr)
		cout << endl;
	else if (ID == root->food->id)
	{
		oldStr = root->food->ingredients;
		//Change all '!' to ','
		stringstream ss(oldStr);
		string newStr = "";
		string token;
		while (getline(ss, token, '!'))
		{
			newStr = newStr + token + ", ";
		}
		newStr = newStr.substr(0, newStr.length() - 2);
		
		// Print out all food info
		cout << "Food ID: " << root->food->id << endl;
		cout << "Ingredients: " << newStr << endl;
		cout << "Category: " << root->food->category << endl;
		cout << "Brand: " << root->food->brand << endl;
	}
	else if (ID < root->food->id)
		searchID(root->left, ID);
	else
		searchID(root->right, ID);
}

 // Traverse entire tree and return vector of food ids that don't have the ingredients passed in
vector<int> OrderedSet::levelOrder(Node* root, string ingredients_) {
	vector<int> recommendation;
	queue<Node*> q;
	q.push(root);
	while (!q.empty()) {
		Node* curNode = q.front();
		if (curNode->left != NULL)
			q.push(curNode->left);
		if (curNode->right != NULL)
			q.push(curNode->right);
		q.pop();
		//cout << curNode->food->id << " ";
		std::size_t found = curNode->food->ingredients.find(ingredients_);
		if (found == string::npos) {
			recommendation.push_back(curNode->food->id);
		}

	}
	sort(recommendation.begin(), recommendation.end());
	return recommendation;
}

void OrderedSet::inorder(Node* root) {
	if (root == NULL)
		cout << "";
	else {
		inorder(root->left);
		cout << root->food->id << " ";
		inorder(root->right);
	}
}

// Rotate functions used from Project 1 code
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
