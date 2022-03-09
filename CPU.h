//
// Created by fawaz on 3/6/2022.
//

#ifndef PROJECT1_CPU_H
#define PROJECT1_CPU_H
#include <iostream>
#include <string>
#include <unistd.h>
#include "Memory.h"
#include "common.h"

using namespace std;

class CPU {
    int PC, SP, IR, AC, X, Y, timer;
public:
    CPU(int timer){
        this->timer = timer;
    }
    int startCPU();
    static int char2Int(char input[]);
    static char* generateInstruction(char command, int address);
};


#endif //PROJECT1_CPU_H
