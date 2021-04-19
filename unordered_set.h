#pragma once

#include <iostream>
#include <vector>
#include "Food.h"
using namespace std;

// cite: stepik 10.2.1 and Aman's lecture on 10.2.1
class unorderedSet {
private:
    int bucketSize;
    int currSize;
    int loadFactor;

    // Pointer to an array of vector (the container/bucket of the unordered set object)
    // The indices of the array represent the hash code
    vector<Food>* table;
    // Hash function
    int hashFunction(int x) {
        return (x % bucketSize);
    }

public:
    // Constructor
    unorderedSet() {
        this->bucketSize = 100;
        this->currSize = 0;
        this->loadFactor = currSize / bucketSize;
        table = new vector<Food>[bucketSize];
    }

    // Helper function that updates the current load factor of the set
    void updateLoadFactor() {
        this->loadFactor = currSize / bucketSize;
    }

    // Helper function that rehashes all the previously inserted values into a bigger bucket
    vector<Food>* hashingForBiggerBucket() {
        int oldBucketSize = bucketSize;
        this->bucketSize *= 2;
        vector<Food>* newTable = new vector<Food>[bucketSize];
        for (int i = 0; i < oldBucketSize; i++) {
            auto iter = table[i].begin();
            for (; iter != table[i].end(); iter++) {
                int index = hashFunction((*iter).id);
                newTable[index].push_back(*iter);
            }
        }
        // Delete the old array
        delete[] table;
        // Return the pointer to a new array container
        return newTable;

    }

    // Inserting food object into the unordered set bucket (array of vector)
    void insert(Food food) {

        int index = hashFunction(food.id);
        table[index].push_back(food);
        currSize++;
        updateLoadFactor();

        //If the bucket load factor is 0.75, double the size of the array and re-hash all current values
        if (loadFactor >= 0.75) {
            this->table = hashingForBiggerBucket();
        }
    }

    // Given an input of food id, search for the food object in the bucket and print out its info
    void search(int food_id) {
        int index = hashFunction(food_id);
        string oldStr = "";
        vector<Food>::iterator iter;
        for (iter = table[index].begin(); iter != table[index].end(); iter++) {
            if (iter->id == food_id) {

                oldStr = iter->ingredients;
                //Change all '!' to ',' in the ingredient string          cite: http://www.cplusplus.com/forum/general/116829/
                stringstream ss(oldStr);
                string newStr = "";
                string token;
                while (getline(ss, token, '!'))
                {
                    newStr = newStr + token + ", ";
                }
                newStr = newStr.substr(0, newStr.length() - 2);

                // Printing food info
                cout << "Food ID: " << iter->id << endl;
                cout << "Ingredients: " << newStr << endl;
                cout << "Category: " << iter->category << endl;
                cout << "Brand: " << iter->brand << endl;
                break;      // Food is found and printed, break out of the searching for-loop
            }
        }
    }

    // Traversing through the bucket and push appropriate items' id in a vector and return the vector
    vector<int> traversal(string unwanted_igd) {
        vector<int> result;
        string ingredients = "";
        for (int i = 0; i < bucketSize; i++) {
            auto iter = table[i].begin();
            for (; iter != table[i].end(); iter++) {
                ingredients = iter->ingredients;
                size_t found = ingredients.find(unwanted_igd);
                // If the unwanted ingredients is present in the string, we continue traversing
                if (found != string::npos) {
                    continue;
                }
                // Else, the unwanted ingredients is not present in the string, we store the food id in the "result" vector
                else {
                    result.push_back(iter->id);
                }
            }
        }
        // Sorting the id in ascending order
        sort(result.begin(), result.end());
        return result;
    }

    // Return current bucket size
    int size() {
        return currSize;
    }


};
