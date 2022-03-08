#include <iostream>
#include <string>
#include <unistd.h>
#include <signal.h>
#include "Memory.h"
#include "CPU.h"

using namespace std;

int pIn[2]; //Pipe in from cpu to memory
int pOut[2]; //Pipe out from memory to cpu



int main(int argc, char** argv) {
    if (argc != 3){
        cout << "Need filename and timer as arguments" << endl;
        return 0;
    }

    string fileName = argv[1];
    ifstream myFile(fileName);

    if (!myFile.is_open()){
        cout << "Cannot open file" << endl;
        return 0;
    }
    myFile.close();
    if (pipe(pIn) < 0 || pipe(pOut) < 0){
        cerr << "Pipe failed" << endl;
        exit(1);
    }

    switch(fork()){
        case -1:
            write(STDERR_FILENO, "Fork failed\n", 12);
            exit(2);

        case 0:
        {
            CPU cpu(atoi(argv[2]));
            cpu.startCPU();

        }
        default:
            Memory memory(fileName);
            memory.runMemory();
    }
    return 0;
}
