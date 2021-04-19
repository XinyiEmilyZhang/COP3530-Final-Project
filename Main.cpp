#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <unordered_map>
#include <queue>
#include <chrono>
#include "OrderedSet.h"
#include "unordered_set.h"
using namespace std;
using namespace std::chrono;

unordered_map<int, Food*> readData(string path) {
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

	}

	return foodList;
}

unordered_map<string, pair<OrderedSet*, Node*>> createOrderedSets(unordered_map<int, Food*> foodList) {
	unordered_map<string, pair<OrderedSet*, Node*>> orderedSetList; //we pair the set to its root 

	for (auto member : foodList) {
		Food* curFood = member.second;
		string category_ = curFood->category;

		auto itr = orderedSetList.find(category_);

		if (itr != orderedSetList.end()) {  //ordered set for this category already exists 
			Node* curRoot = itr->second.second;
			OrderedSet* curSet = itr->second.first;
			curRoot = curSet->insert(curRoot, curFood);
			orderedSetList[category_] = make_pair(curSet, curRoot);
		}
		else { //create new ordered set
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

		if (itr != unorderedSetList.end()) {  //unordered set for this category already exists 
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

	auto start = high_resolution_clock::now();
	unordered_map<int, Food*> foodList = readData("branded_food.csv"); //load data set
	std::cout << "All Your Food Is Loaded!" << endl;
	auto stop = high_resolution_clock::now();

	auto duration = duration_cast<seconds>(stop - start);
	cout << "*-----------------------------------------------------*" << endl;
	cout << "Time to Load Data: " << duration.count() << " seconds" << endl;
	cout << "*-----------------------------------------------------*" << endl;
	cout << endl;

	cout << "************************************************" << endl;
	cout << "Group Food Based On Categories To Unordered Set" << endl;
	cout << "************************************************" << endl;

	auto build_Unordered_Set_Start = high_resolution_clock::now();
	unordered_map<string, unorderedSet*> unordered_Set_List = createUnorderedSets(foodList); //create a list of ordered sets
	auto build_Unordered_Set_End = high_resolution_clock::now();
	auto build_Unordered_Set_Duration = duration_cast<microseconds>(build_Unordered_Set_End - build_Unordered_Set_Start);
	cout << "Time To Build All Unordered Sets: " << build_Unordered_Set_Duration.count() << " microseconds" << endl;
	cout << endl;

	cout << "************************************************" << endl;
	cout << "Group Food Based On Categories To Ordered Set" << endl;
	cout << "************************************************" << endl;

	auto build_Ordered_Set_Start = high_resolution_clock::now();
	unordered_map<string, pair<OrderedSet*, Node*>> ordered_Set_List = createOrderedSets(foodList); //create a list of unordered sets
	auto build_Ordered_Set_End = high_resolution_clock::now();
	auto build_Ordered_Set_Duration = duration_cast<microseconds>(build_Ordered_Set_End - build_Ordered_Set_Start);
	cout << "Time To Build All Ordered Sets: " << build_Ordered_Set_Duration.count() << " microseconds" << endl;
	cout << endl;


	while (true) {
		std::cout << "Input Food Product ID: ";
		string inputID;
		std::cin >> inputID;

		auto itr = foodList.find(stoi(inputID));

		if (itr != foodList.end()) { //find whether the input id is valid
			std::cout << "Food Is Found!" << endl;
			cout << endl;
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
			std::cout << endl;

			std::size_t found = curIngredients.find(inputIngredients);

			if (found != string::npos) { //if the input food contains the ingredient 
				std::cout << "This Food Is NOT YOURS Because It Contains " << inputIngredients << endl;

				cout << endl;
				//----------------Use Unordered Set--------------------//

				for (auto member : unordered_Set_List) { //locate the set that represents user input food's category
					if (member.first == target->category) {
						unorderedSet* targetSet = member.second;

						auto traverse_Unordered_Set_Start = high_resolution_clock::now();
						vector<int> recList = targetSet->traversal(inputIngredients); //traverse in the same category to find subsitutes 
						cout << "Use Unordered Set --- Here Are Total Of " << recList.size() << " Recommendations from " << target->category << " That Does Not Contain: " << inputIngredients << endl;
						if (recList.size() > 5) {
							cout << "Here Are Five Examples: ";
							cout << recList[0] << " ";
							cout << recList[recList.size() / 4] << " ";
							cout << recList[recList.size() / 2] << " ";
							cout << recList[recList.size() * 3 / 4] << " ";
							cout << recList[recList.size() - 1] << " ";
						}
						else {
							std::cout << "Here Are Your Suggestions: ";
							for (unsigned i = 0; i < recList.size(); i++) {
								cout << recList[i] << " ";
							}
						}

						cout << endl;
						auto traverse_Unordered_Set_End = high_resolution_clock::now();

						auto traverse_Unordered_Set_Duration = duration_cast<microseconds>(traverse_Unordered_Set_End - traverse_Unordered_Set_Start);
						cout << endl;
						cout << "Insert One Item To Look UP: " << endl;
						string lookUpID;
						cin >> lookUpID;

						auto search_Unordered_Set_Start = high_resolution_clock::now();
						cout << endl;
						cout << "-------------------Product Details----------------------" << endl;
						targetSet->search(stoi(lookUpID));    // Look up an item in the set 
						auto search_Unordered_Set_End = high_resolution_clock::now();
						auto search_Unordered_Set_Duration = duration_cast<microseconds>(search_Unordered_Set_End - search_Unordered_Set_Start);

						cout << endl;
						cout << "*-----------------------------------------------------*" << endl;
						cout << "Time To Build All Unordered Sets: " << build_Unordered_Set_Duration.count() << " microseconds" << endl;
						cout << "Time to Traverse An Unordered Set: " << traverse_Unordered_Set_Duration.count() << " microseconds" << endl;
						cout << "Time to Search In An Unordered Set: " << search_Unordered_Set_Duration.count() << " microseconds" << endl;
						cout << "*-----------------------------------------------------*" << endl;
						cout << endl;
					}
				}

				cout << endl;

				//-----------------------Use Ordered Set--------------------------//
				for (auto member : ordered_Set_List) {    // locate the set that represents user input food's category
					if (member.first == target->category) {
						OrderedSet* targetSet = member.second.first;
						Node* targetRoot = member.second.second;

						auto traverse_Ordered_Set_Start = high_resolution_clock::now();
						vector<int> recList = targetSet->levelOrder(targetRoot, inputIngredients); //traverse the set to find subsitutues
						auto traverse_Ordered_Set_End = high_resolution_clock::now();

						cout << "Use Ordered Set --- Here Are Total of " << recList.size() << " Recommendations from " << target->category << " That Does Not Contain: " << inputIngredients << endl;
						if (recList.size() > 5) {
							cout << "Here Are Five Examples: ";
							cout << recList[0] << " ";
							cout << recList[recList.size() / 4] << " ";
							cout << recList[recList.size() / 2] << " ";
							cout << recList[recList.size() * 3 / 4] << " ";
							cout << recList[recList.size() - 1] << " ";
						}
						else {
							std::cout << "Here Are Your Suggestions: ";
							for (unsigned i = 0; i < recList.size(); i++) {
								cout << recList[i] << " ";
							}
						}
						cout << endl;

						auto traverse_Ordered_Set_Duration = duration_cast<microseconds>(traverse_Ordered_Set_End - traverse_Ordered_Set_Start);

						cout << endl;
						cout << "Insert One Item To Look UP: " << endl;
						string lookUpID;
						cin >> lookUpID;

						auto search_Ordered_Set_Start = high_resolution_clock::now();
						cout << endl;
						cout << "-------------------Product Details----------------------" << endl;
						targetSet->searchID(targetRoot, stoi(lookUpID));                        //search an item in the set 
						auto search_Ordered_Set_End = high_resolution_clock::now();
						auto search_Ordered_Set_Duration = duration_cast<microseconds>(search_Ordered_Set_End - search_Ordered_Set_Start);

						cout << endl;
						cout << "*-----------------------------------------------------*" << endl;
						cout << "Time to Build All Ordered Sets: " << build_Ordered_Set_Duration.count() << " microseconds" << endl;
						cout << "Time to Traverse An Ordered Set: " << traverse_Ordered_Set_Duration.count() << " microseconds" << endl;
						cout << "Time to Search In An Ordered Set: " << search_Ordered_Set_Duration.count() << " microseconds" << endl;
						cout << "*-----------------------------------------------------*" << endl;
						cout << endl;

					}
				}

			}
			else { //if the input food does not contain the ingredient 
				cout << "This Food Is YOURS. Please Enjoy." << endl;
				cout << endl;
				Food* yourFood = foodList[stoi(inputID)];
				string oldStr = "";
				oldStr = yourFood->ingredients;
				stringstream ss(oldStr);  //Change all '!' to ',' http://www.cplusplus.com/forum/general/116829/
				string newStr = "";
				string token; 
				while (getline(ss, token, '!')) 
				{
					newStr = newStr + token + ", ";
				}
				newStr = newStr.substr(0, newStr.length() - 2);

				cout << "Food ID: " << yourFood->id << endl;
				cout << "Ingredients: " << newStr << endl;
				cout << "Category: " << yourFood->category << endl;
				cout << "Brand: " << yourFood->brand << endl;
				std::cout << endl;
			}

		}
		else { //input ID not valid 
			std::cout << "Food Is Not Found!" << endl;
			std::cout << endl;

		}
	}


	return 0;
}
