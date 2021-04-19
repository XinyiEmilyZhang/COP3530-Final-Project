#pragma once

#include <iostream>
#include <vector>
#include "Food.h"
using namespace std;

class unorderedSet {
private:
    int bucketSize;
    int currSize;
    int loadFactor;

    //arrays of vector
    vector<Food>* table;
    //Hash function
    int hashFunction(int x) {
        return (x % bucketSize);
    }

public:
    unorderedSet() {
        this->bucketSize = 100;
        this->currSize = 0;
        this->loadFactor = currSize / bucketSize;
        table = new vector<Food>[bucketSize];
    }

    void updateLoadFactor() {
        this->loadFactor = currSize / bucketSize;
    }

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
        delete[] table;
        return newTable;

    }

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

    void search(int food_id) {
        int index = hashFunction(food_id);
        Food* result;
        string oldStr = "";
        vector<Food>::iterator iter;
        for (iter = table[index].begin(); iter != table[index].end(); iter++) {
            if (iter->id == food_id) {

                oldStr = iter->ingredients;
                //Change all '!' to ','
                stringstream ss(oldStr);
                string newStr = "";
                string token;
                while (getline(ss, token, '!'))
                {
                    newStr = newStr + token + ", ";
                }
                newStr = newStr.substr(0, newStr.length() - 2);
                cout << "Food ID: " << iter->id << endl;
                cout << "Ingredients: " << newStr << endl;
                cout << "Category: " << iter->category << endl;
                cout << "Brand: " << iter->brand << endl;
                break;
            }
        }
    }

    vector<int> traversal(string unwanted_igd) {
        vector<int> result;
        string ingredients = "";
        for (int i = 0; i < bucketSize; i++) {
            auto iter = table[i].begin();
            for (; iter != table[i].end(); iter++) {
                ingredients = iter->ingredients;
                size_t found = ingredients.find(unwanted_igd);
                if (found != string::npos) {
                    continue;
                }
                else {
                    result.push_back(iter->id);
                }
            }
        }
        sort(result.begin(),result.end());
        return result;
    }

    int size() {
        return currSize;
    }


};
