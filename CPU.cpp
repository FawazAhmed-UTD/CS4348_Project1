//
// Created by fawaz on 3/6/2022.
//

#include "CPU.h"


int CPU::startCPU(){    //runs for CPU
    int userPrograms = 999;
    int systemCode = 1999;
    int instuctionCount = 0;
    int interrupt = 0;
    bool userMode = true;
    PC = 0;
    SP = userPrograms;

    while (true) {
        char instruction[5] = {'\0', '\0', '\0', '\0', '\0'};   //instruction buffer
        char* buffer;
        bool jump = false;

        //checks if there is another interrupt going on
        if (!userMode && instuctionCount > 0 && (instuctionCount % timer) == 0){
            interrupt++;
        }
        //For kernal mode
        if ((userMode && instuctionCount > 0 && (instuctionCount % timer) == 0) || (userMode && interrupt)) {
            userMode = false;
            interrupt--;
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

        instuctionCount++;

        if (instruction[0] == '\0') {
            continue;
        }

        IR = char2Int(instruction); //get instuction
        char read_mem[5];

        switch(IR) {    //executions
            case 1://    1 = Load value
                PC++;
                buffer = generateInstruction('r', PC);
                write(pIn[1], &(*buffer), 5);
                read(pOut[0], &read_mem, 4);
                AC = char2Int(read_mem);
                break;
            case 2://    2 = Load addr
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
            case 3://    3 = LoadInd addr
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
            case 4://    4 = LoadIdxX addr
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
            case 5://    5 = LoadIdxY addr
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
            case 6://    6 = LoadSpX
                if (X + SP > 999 && userMode)
                    cout << "Can't access memory in user mode" << endl;
                else {
                    buffer = generateInstruction('r', SP + 1 + X);
                    write(pIn[1], &(*buffer), 5);
                    read(pOut[0], &read_mem, 4);
                    AC = char2Int(read_mem);
                }
                break;
            case 7://    7 = Store addr
                PC++;
                buffer = generateInstruction('r', PC);
                write(pIn[1], &(*buffer), 5);
                read(pOut[0], &read_mem, 4);

                buffer = generateInstruction('w', char2Int(read_mem));
                write(pIn[1], &(*buffer), 5);
                buffer = generateInstruction('\0', AC);
                write(pIn[1], &(*buffer), 4);
                break;
            case 8://    8 = Get
                AC = rand() % 100 + 1;
                break;
            case 9://    9 = Put port
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
            case 10://AddX
                AC += X;
                break;
            case 11://addy
                AC += Y;
                break;
            case 12://subx
                AC -= X;
                break;
            case 13://suby
                AC -= Y;
                break;
            case 14://   14 = CopyToX
                X = AC;
                break;
            case 15://   15 = CopyFromX
                AC = X;
                break;
            case 16://   16 = CopyToY
                Y = AC;
                break;
            case 17://   17 = CopyFromY
                AC = Y;
                break;
            case 18://   18 = CopyToSp
                SP = AC;
                break;
            case 19://   19 = CopyFromSp
                AC = SP;
                break;
            case 20://   20 = Jump addr
                PC++;
                buffer = generateInstruction('r', PC);
                write(pIn[1], &(*buffer), 5);
                read(pOut[0], &read_mem, 4);

                PC = char2Int(read_mem);
                jump = true;
                break;
            case 21://   21 = JumpIfEqual addr
                PC++;
                buffer = generateInstruction('r', PC);
                write(pIn[1], &(*buffer), 5);
                read(pOut[0], &read_mem, 4);

                if (AC == 0) {
                    PC = char2Int(read_mem);
                    jump = true;
                }
                break;
            case 22://   22 = JumpIfNotEqual addr
                PC++;
                buffer = generateInstruction('r', PC);
                write(pIn[1], &(*buffer), 5);
                read(pOut[0], &read_mem, 4);

                if (AC != 0) {
                    PC = char2Int(read_mem);
                    jump = true;
                }
                break;
            case 23://   23 = Call addr
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
            case 24://   24 = Ret
                SP++;
                buffer = generateInstruction('r', SP);
                write(pIn[1], &(*buffer), 5);
                read(pOut[0], &read_mem, 4);

                PC = char2Int(read_mem);
                jump = true;
                break;
            case 25://   25 = IncX
                X++;
                break;
            case 26://   26 = DecX
                X--;
                break;
            case 27://   27 = Push
                buffer = generateInstruction('w', SP);
                write(pIn[1], &(*buffer), 5);
                buffer = generateInstruction('\0', AC);
                write(pIn[1], &(*buffer), 4);

                SP--;
                break;
            case 28://   28 = Pop
                SP++;
                buffer = generateInstruction('r', SP);
                write(pIn[1], &(*buffer), 5);
                read(pOut[0], &read_mem, 4);

                AC = char2Int(read_mem);
                break;
            case 29://   29 = Int
                if(userMode){
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
                }
                break;
            case 30://   30 = IRet
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
            case 50: //   50 = End
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

int CPU::char2Int(char *input) {//array of char to int
    try{
        return boost::lexical_cast<int>(input);
    }
    catch(const std::exception& e){
        cerr << " Can't access memory in user mode" << endl;//error stuff for sample4.txt which I couldnt get ot work
        exit(0);
    }
}

char* CPU::generateInstruction(char command, int address) {//helper method
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