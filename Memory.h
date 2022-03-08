//
// Created by fawaz on 3/6/2022.
//

#ifndef PROJECT1_MEMORY_H
#define PROJECT1_MEMORY_H
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <fstream>
#include "common.h"
#include <boost/lexical_cast.hpp>

using namespace std;

class Memory {
private:
    int memoryArray[2000];
public:
    Memory(const string& fileName){
        initializeMemory(fileName);
    }
    void initializeMemory(const string& fileName);
    void writeMem(int address, int data);
    int runMemory();
    int char2Int(char input[]);
};


#endif //PROJECT1_MEMORY_H
