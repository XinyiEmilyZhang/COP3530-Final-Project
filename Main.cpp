#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <unordered_map>
#include "OrderedSet.h"
using namespace std;

unordered_map<string, Food*> readData(string path) { //return a root node for an AVL Tree that contains all food
	unordered_map<string, Food*> foodList;
	ifstream inFile(path); //use file stream to read a file 
	string lineFromFile;
	getline(inFile, lineFromFile); //ignore the first line

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

		foodList[id_] = curFood;
	}

	return foodList;
}


int main() {
	std::cout << "Loading All Your Food (Data Set)..." << endl;
	unordered_map<string, Food*> foodList = readData("branded_food.csv");
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

		auto itr = foodList.find(inputID);
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
			string curIngredients = target->ingredients;
			std::size_t found = curIngredients.find(inputIngredients);
			if (found != string::npos) {
				std::cout << "This Food Is NOT YOURS Because It Contains " << inputIngredients << endl;
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