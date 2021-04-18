#pragma once
#include <string>
using namespace std;

class Food {
public:
	int id;
	string brand;
	string ingredients;
	string category;

	Food() {
		id = 0;
		brand = "";
		category = "";
	}
	Food(int id_, string brand_, string ingredients_, string category_) {
		id = id_;
		brand = brand_;
		ingredients = ingredients_;
		category = category_;
	}
};