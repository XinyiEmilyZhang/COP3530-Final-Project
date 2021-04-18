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
    int hashFunction(int x){
        return (x % bucketSize);
    }

public:
    unorderedSet() {
        this->bucketSize = 100;
        this->currSize = 0;
        this->loadFactor = currSize/bucketSize;
        table = new vector<Food> [bucketSize];
    }

    void updateLoadFactor(){
        this->loadFactor = currSize/bucketSize;
    }

    vector<Food>* hashingForBiggerBucket(){
        int oldBucketSize = bucketSize;
        this->bucketSize *= 2;
        vector<Food>* newTable = new vector<Food> [bucketSize];
        for (int i = 0; i < oldBucketSize; i++){
            auto iter = table[i].begin();
            for (; iter != table[i].end(); iter++){
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
        vector<Food>::iterator iter;
        for (iter = table[index].begin(); iter != table[index].end(); iter++){
            if (iter->id == food_id) {
                cout << "Food ID: " << iter->id << endl;
                cout << "Ingredients: " << iter->ingredients << endl;
                cout << "Category: " << iter->category << endl;
                cout << "Brand: " << iter->brand << endl;
                break;
            }
        }
    }

    void traversal(string unwanted_igd){
        string ingredients = "";
        for (int i = 0; i < table->size(); i++){
            auto iter = table[i].begin();
            for (; iter != table[i].end(); iter++){
                ingredients = iter->ingredients;
                size_t found = ingredients.find(unwanted_igd);
                if (found != string::npos) {
                    continue;
                }
                else {
                    cout << iter->id << " ";
                }
            }
        }
        cout << endl;
    }

    int size() {
        return currSize;
    }
    
    
};
