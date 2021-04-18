
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <unordered_map>
#include <queue>
#include "OrderedSet.h"
#include "unordered_set.h"
using namespace std;

unordered_map<int, Food*> readData(string path) { //return a root node for an AVL Tree that contains all food
	unordered_map<int, Food*> foodList;

	ifstream inFile(path); //use file stream to read a file 
	string lineFromFile;
	getline(inFile, lineFromFile); //ignore the first line
	int count = 0;
	while (getline(inFile, lineFromFile)) {
		istringstream streamFromString(lineFromFile);

		string id_;
		int id;
		getline(streamFromString, id_, ',');
		id = stoi(id_);

		string brand_;
		getline(streamFromString, brand_, ',');

		string c;
		getline(streamFromString, c, ',');

		string ingredients_;
		getline(streamFromString, ingredients_, ',');

		string e;
		getline(streamFromString, e, ',');

		string f;
		getline(streamFromString, f, ',');

		string g;
		getline(streamFromString, g, ',');

		string category_;
		getline(streamFromString, category_, ',');

		Food* curFood = new Food(id, brand_, ingredients_, category_);

		foodList[id] = curFood;
		count++;
		if (count == 1000)
			break;
	}

	return foodList;
}

unordered_map<string, pair<OrderedSet*, Node*>> createOrderedSets (unordered_map<int, Food*> foodList){
	unordered_map<string, pair<OrderedSet*, Node*>> orderedSetList;
	for (auto member : foodList) {
		Food* curFood = member.second;
		string category_ = curFood->category;

		auto itr = orderedSetList.find(category_);

		if (itr != orderedSetList.end()) {  //ordered set already exists 
			Node* curRoot = itr->second.second;
			OrderedSet* curSet = itr->second.first;
		    curRoot = curSet->insert(curRoot, curFood);
			orderedSetList[category_] = make_pair(curSet, curRoot);
		}
		else {
			OrderedSet* newOS = new OrderedSet();
			Node* newRoot = newOS->insert(NULL, curFood);
			orderedSetList[category_] = make_pair(newOS, newRoot);
		}
	}

	return orderedSetList;
}

unordered_map<string, unorderedSet*> createUnorderedSets(unordered_map<int, Food*> foodList) {
	unordered_map<string, unorderedSet*> unorderedSetList;
	for (auto member : foodList) {
		Food* curFood = member.second;
		string category_ = curFood->category;

		auto itr = unorderedSetList.find(category_);

		if (itr != unorderedSetList.end()) {  //ordered set already exists 
			unorderedSet* curSet = itr->second;
			curSet->insert(*curFood);
			unorderedSetList[category_] = curSet;
		}
		else {
			unorderedSet* newSet = new unorderedSet();
			newSet->insert(*curFood);
			unorderedSetList[category_] = newSet;
		}
	}
	return unorderedSetList;
}

int main() {
	std::cout << "Loading All Your Food (Data Set)..." << endl;
	unordered_map<int, Food*> foodList = readData("branded_food.csv");
	std::cout << "All Your Food Is Loaded!" << endl;

	//--------------Debug Loading Data Set------------//
	/*string targetID = "810268";
	auto itr = foodList.find(targetID);
	if (itr != foodList.end()) {
		Food* target = itr->second;
		std::cout << "brand: " << target->brand << endl;
		std::cout << "ingredients: " << target->ingredients << endl;
		std::cout << "category: " << target->category;
	}
	*/

	while (true) {
		std::cout << "Input Food Product ID: ";
		string inputID;
		std::cin >> inputID;

		auto itr = foodList.find(stoi(inputID));

		if (itr != foodList.end()) {
			std::cout << "Food Is Found!" << endl;
			std::cout << "Input Ingredients You Want To Avoid: ";
			string inputIngredients;
			std::cin >> inputIngredients;
			for (int i = 0; i < inputIngredients.length(); i++) {
				if (!isupper(inputIngredients[i]))
					inputIngredients[i] = toupper(inputIngredients[i]); //change input ingredients to upper case to match with data set 
			}

			Food* target = itr->second;
			int targetID = itr->first;
			string curIngredients = target->ingredients;

			std::size_t found = curIngredients.find(inputIngredients);

			if (found != string::npos) {
				std::cout << "This Food Is NOT YOURS Because It Contains " << inputIngredients << endl;
				std::cout << "Here Are Some Recommendations Of Similar Food :" << endl;
				
				
				unordered_map<string, pair<OrderedSet*, Node*>> ordered_Set_List = createOrderedSets(foodList);
				unordered_map<string, unorderedSet*> unordered_Set_List = createUnorderedSets(foodList);

				/*for (auto member : ordered_Set_List) {
					OrderedSet* targetSet = member.second.first;
					Node* targetRoot = member.second.second;
					targetSet->inorder(targetRoot);
				}*/

				/*cout << "Using unordered set: " << endl;
				for (auto member : unordered_Set_List) {
					if (member.first == target->category) {
						unorderedSet* targetSet = member.second;
						targetSet->traversal(inputIngredients);
					}

				}*/

				cout << "Using ordered set: " << endl;
				for (auto member : ordered_Set_List) {
					if (member.first == target->category) {
						OrderedSet* targetSet = member.second.first;
						Node* targetRoot = member.second.second;
						vector<int> targetID = targetSet->levelOrder(targetRoot, inputIngredients);
						for (auto itr : targetID) {
							cout << itr << " ";
						}
						cout << endl;
					}
				}

			}
			else {
				std::cout << "This Food Is YOURS. Please Enjoy." << endl;
			}

		}
		else {
			std::cout << "Food Is Not Found!" << endl;
		}
	}


	return 0;
}
