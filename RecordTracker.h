#ifndef RECORDTRACKER_H
#define RECORDTRACKER_H

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class records
{
    private:
    vector<vector<string>> allRecords;
    string fileName;

    public:
    records(string);
    void printMenu();
    void readFile(); //store file records inside of a 2D-vector 
    void addRecord();
    void deleteRecord();
    void removeBlankLines();
    void viewRecords();
   
};

#endif