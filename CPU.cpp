//
// Created by fawaz on 3/6/2022.
//

#include "CPU.h"


int CPU::startCPU(){
    int userPrograms = 999;
    int systemCode = 1999;
    int cpuCounter = 0;
    int waitingCount = 0;
    bool userMode = true;
    PC = 0;
    SP = userPrograms;

    while (true) {
        char instruction[5] = {'\0', '\0', '\0', '\0', '\0'};
        char* buffer;
        bool jump = false;
        if (userMode && cpuCounter > 0 && (cpuCounter % timer) == 0){
            waitingCount++;
        }

        if ((userMode && cpuCounter > 0 && (cpuCounter % timer) == 0) || (userMode && waitingCount)) {
            userMode = false;
            waitingCount--;
            int userSp = SP;
            int userPc = PC;
            SP = systemCode;
            PC = 1000;
            jump = true;

            buffer = generateInstruction('w', SP);
            write(pIn[1], &(*buffer), 5);
            buffer = generateInstruction('\0', userSp);
            write(pIn[1], &(*buffer), 4);

            SP--;

            buffer = generateInstruction('w', SP);
            write(pIn[1], &(*buffer), 5);
            buffer = generateInstruction('\0', userPc);
            write(pIn[1], &(*buffer), 4);

            SP--;

            continue;
        }

        buffer = generateInstruction('r', PC);
        write(pIn[1], &(*buffer), 5);
        read(pOut[0], &instruction, 4);

        cpuCounter++;

        if (instruction[0] == '\0') {
            continue;
        }

        IR = char2Int(instruction);
        char read_mem[5];

        switch(IR) {
            case 1:
                PC++;
                buffer = generateInstruction('r', PC);
                write(pIn[1], &(*buffer), 5);
                read(pOut[0], &read_mem, 4);
                AC = char2Int(read_mem);
                break;
            case 2:
                PC++;
                buffer = generateInstruction('r', PC);
                write(pIn[1], &(*buffer), 5);
                read(pOut[0], &read_mem, 4);

                if (char2Int(read_mem) > 999 && userMode)
                    cout << "Can't access address in user mode" << endl;
                else {
                    buffer = generateInstruction('r', char2Int(read_mem));
                    write(pIn[1], &(*buffer), 5);
                    read(pOut[0], &read_mem, 4);
                    AC = char2Int(read_mem);
                }
                break;
            case 3:
                PC++;
                buffer = generateInstruction('r', PC);
                write(pIn[1], &(*buffer), 5);
                read(pOut[0], &read_mem, 4);

                if (char2Int(read_mem) > 999 && userMode)
                    cout << "Can't access memory in user mode" << endl;
                else {
                    buffer = generateInstruction('r', char2Int(read_mem));
                    write(pIn[1], &(*buffer), 5);
                    read(pOut[0], &read_mem, 4);
                    AC = char2Int(read_mem);
                    if (char2Int(read_mem) > 999 && userMode)
                        cout << "Can't access memory in user mode" << endl;
                    else {
                        buffer = generateInstruction('r', AC);
                        write(pIn[1], &(*buffer), 5);
                        read(pOut[0], &read_mem, 4);
                        AC = char2Int(read_mem);
                    }
                }
            case 4:
                PC++;
                buffer = generateInstruction('r', PC);
                write(pIn[1], &(*buffer), 5);
                read(pOut[0], &read_mem, 4);

                if (X + char2Int(read_mem) > 999 && userMode)
                    cout << "Can't access memory in user mode" << endl;
                else {
                    buffer = generateInstruction('r', X + char2Int(read_mem));
                    write(pIn[1], &(*buffer), 5);
                    read(pOut[0], &read_mem, 4);
                    AC = char2Int(read_mem);
                }
                break;
            case 5:
                PC++;
                buffer = generateInstruction('r', PC);
                write(pIn[1], &(*buffer), 5);
                read(pOut[0], &read_mem, 4);

                if (Y + char2Int(read_mem) > 999 && userMode)
                    cout << "Can't access memory in user mode" << endl;
                else {
                    buffer = generateInstruction('r', Y + char2Int(read_mem));
                    write(pIn[1], &(*buffer), 5);
                    read(pOut[0], &read_mem, 4);
                    AC = char2Int(read_mem);
                }
                break;
            case 6:
                if (X + SP > 999 && !userMode)
                    cout << "Can't access memory in user mode" << endl;
                else {
                    buffer = generateInstruction('r', SP + 1 + X);
                    write(pIn[1], &(*buffer), 5);
                    read(pOut[0], &read_mem, 4);
                    AC = char2Int(read_mem);
                }
                break;
            case 7:
                PC++;
                buffer = generateInstruction('r', PC);
                write(pIn[1], &(*buffer), 5);
                read(pOut[0], &read_mem, 4);

                buffer = generateInstruction('w', char2Int(read_mem));
                write(pIn[1], &(*buffer), 5);
                buffer = generateInstruction('\0', AC);
                write(pIn[1], &(*buffer), 4);
                break;
            case 8:
                AC = rand() % 100 + 1;
                break;
            case 9:
                PC++;
                buffer = generateInstruction('r', PC);
                write(pIn[1], &(*buffer), 5);
                read(pOut[0], &read_mem, 4);

                if (char2Int(read_mem) == 1) {
                    cout << AC;
                }
                if (char2Int(read_mem) == 2) {
                    cout << char(AC);
                }

                break;
            case 10:
                AC += X;
                break;
            case 11:
                AC += Y;
                break;
            case 12:
                AC -= X;
                break;
            case 13:
                AC -= Y;
                break;
            case 14:
                X = AC;
                break;
            case 15:
                AC = X;
                break;
            case 16:
                Y = AC;
                break;
            case 17:
                AC = Y;
                break;
            case 18:
                SP = AC;
                break;
            case 19:
                AC = SP;
                break;
            case 20:
                PC++;
                buffer = generateInstruction('r', PC);
                write(pIn[1], &(*buffer), 5);
                read(pOut[0], &read_mem, 4);

                PC = char2Int(read_mem);
                jump = true;
                break;
            case 21:
                PC++;
                buffer = generateInstruction('r', PC);
                write(pIn[1], &(*buffer), 5);
                read(pOut[0], &read_mem, 4);

                if (AC == 0) {
                    PC = char2Int(read_mem);
                    jump = true;
                }
                break;
            case 22:
                PC++;
                buffer = generateInstruction('r', PC);
                write(pIn[1], &(*buffer), 5);
                read(pOut[0], &read_mem, 4);

                if (AC != 0) {
                    PC = char2Int(read_mem);
                    jump = true;
                }
                break;
            case 23:
                PC++;
                buffer = generateInstruction('r', PC);
                write(pIn[1], &(*buffer), 5);
                read(pOut[0], &read_mem, 4);

                buffer = generateInstruction('w', SP);
                write(pIn[1], &(*buffer), 5);
                buffer = generateInstruction('\0', PC + 1);
                write(pIn[1], &(*buffer), 4);
                SP--;

                PC = char2Int(read_mem);
                jump = true;
                break;
            case 24:
                SP++;
                buffer = generateInstruction('r', SP);
                write(pIn[1], &(*buffer), 5);
                read(pOut[0], &read_mem, 4);

                PC = char2Int(read_mem);
                jump = true;
                break;
            case 25:
                X++;
                break;
            case 26:
                X--;
                break;
            case 27:
                buffer = generateInstruction('w', SP);
                write(pIn[1], &(*buffer), 5);
                buffer = generateInstruction('\0', AC);
                write(pIn[1], &(*buffer), 4);

                SP--;
                break;
            case 28:
                SP++;
                buffer = generateInstruction('r', SP);
                write(pIn[1], &(*buffer), 5);
                read(pOut[0], &read_mem, 4);

                AC = char2Int(read_mem);
                break;
            case 29:
            {
                userMode = false;
                int usrSP = SP;
                int usrPC = PC + 1;
                SP = systemCode;
                PC = 1500;
                jump = true;

                buffer = generateInstruction('w', SP);
                write(pIn[1], &(*buffer), 5);
                buffer = generateInstruction('\0', usrSP);
                write(pIn[1], &(*buffer), 4);

                SP--;

                buffer = generateInstruction('w', SP);
                write(pIn[1], &(*buffer), 5);
                buffer = generateInstruction('\0', usrPC);
                write(pIn[1], &buffer, 4);

                SP--;
                break;
            }
            case 30:
                userMode = true;
                SP++;

                buffer = generateInstruction('r', SP);
                write(pIn[1], &(*buffer), 5);
                read(pOut[0], &read_mem, 4);

                PC = char2Int(read_mem);
                jump = true;

                SP++;

                buffer = generateInstruction('r', SP);
                write(pIn[1], &(*buffer), 5);
                read(pOut[0], &read_mem, 4);

                SP = char2Int(read_mem);
                break;
            case 50:
                close(pOut[0]);
                close(pOut[1]);
                write(pIn[1], "e", 5);
                close(pIn[0]);
                close(pIn[1]);
                return 0;
        }
        if (!jump){
            PC++;
        }
    }
}

int CPU::char2Int(char *input) {
    try{
        return boost::lexical_cast<int>(input);
    }
    catch(const std::exception& e){
        cerr << "Can't access memory in user mode" << endl;
        exit(0);
    }
}

char* CPU::generateInstruction(char command, int address) {
    char* buffer = new char[10];
    string addressStr = to_string(address);
    if(command == '\0'){
        for(int i = 0; i < addressStr.size(); i++){
            buffer[i] = addressStr[i];
        }
    }
    else{
        buffer[0] = command;
        for(int i = 0; i < addressStr.size(); i++){
            buffer[i+1] = addressStr[i];
        }
    }
    return buffer;
}
