//
// Created by fawaz on 3/6/2022.
//

#include "Memory.h"

void Memory::writeMem(int address, int data) {
    memoryArray[address] = data;
}

void Memory::initializeMemory(const string& fileName) {
    size_t i = 0;
    string line;
    ifstream myFile(fileName);

    while(getline(myFile,line)){
        size_t n = line.find_first_not_of("0123456789.");
        string result = line.substr(0,n);

        if(result.empty()){
            continue;
        }
        if (result[0] == '.')
        {
            i = stoi(result.substr(1));
        }
        else{
            writeMem(i, stoi(result));
            i++;
        }
    }
}

int Memory::runMemory() {
    while (true) {
        char input[6] = {'\0', '\0', '\0', '\0', '\0', '\0'};
        read(pIn[0], &input, 5);

        char command = input[0];

        for (int i = 0; i < 5; i++){
            input[i] = input[i + 1];
        }
        switch(command){
            case 'r':
                char convertToStr[5];
                snprintf(convertToStr, 5, "%d", memoryArray[char2Int(input)]);
                write(pOut[1], &convertToStr, 4);
                break;
            case 'w':
                char write_val[5];
                read(pIn[0], &write_val, 4);
                writeMem(char2Int(input), char2Int(write_val));
                break;
            case 'e':
                exit(0);
            default:
                break;
        }
    }
}

int Memory::char2Int(char *input) {
    try{
        return boost::lexical_cast<int>(input);
    }
    catch(const std::exception& e){
        cerr << "Can't access memory in user modes" << endl;
        exit(0);
    }
}
