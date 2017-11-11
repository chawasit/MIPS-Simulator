#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdio>

#define SIZE_OF_MEMORY 65536
#define SIZE_OF_REGISTER 8
#define SIZE_OF_CHAR_BUFFER 100
enum Opcode {add, nand, lw, sw, beq, jalr, halt, noop};

using namespace std;

struct State {
    int program_counter;
    int memories[SIZE_OF_MEMORY];
    int registers[SIZE_OF_REGISTER];
    int number_of_instructions;
    bool is_halt;
} state;

void printState() {
    cout << "\n@@@\nstate:\n";
    cout << "\tpc " << state.program_counter << endl;
    cout << "\tmemory:\n";

    for (int i = 0; i < state.number_of_instructions; i++)
    {
        cout << "\t\tmem[ " << i << " ] " << state.memories[i] << endl;
    }

    cout << "\tregisters:\n";
    
    for (int i = 0; i < SIZE_OF_REGISTER; i++)
    {
        cout << "\t\treg[ " << i << " ] " << state.registers[i] << endl;
    }

    cout << "end state" << endl;
}
vector<string> file_to_lines(ifstream &file) {
    vector<string> lines;
    string line;
    while (getline(file, line)) {
        lines.push_back(line);
    }

    return lines;
}

vector<int> parse(vector<string> lines) {
    vector<int> machineCodes;
    for (string line: lines) {
        machineCodes.push_back(stoi(line));
    }

    return machineCodes;
}

Opcode GetOpcode(int machine_code) {
    return Opcode(machine_code >> 22);
}

int ValidRegisterNumber(int number) {
    if (number > 7 or number < 0) {
        char error[SIZE_OF_CHAR_BUFFER];
        sprintf(error, "Unknow invalid register %d", number);
        throw error;
    }

    return number;
}

int GetRS(int machine_code) {
    return ValidRegisterNumber((machine_code >> 19) & 7);
}

int GetRT(int machine_code) {
    return ValidRegisterNumber((machine_code >> 16) & 7);
}

int GetRD(int machine_code) {
    return ValidRegisterNumber(machine_code & 7);
}

int GetOffset(int machine_code) {
    return (machine_code << 16) >> 16;
}

int GetMachineCode(int program_counter) {
    return state.memories[program_counter];
}

void StoreProgramToState(vector<int> machine_codes) {
    state.number_of_instructions = machine_codes.size();

    for (int i = 0; i < machine_codes.size(); i++) {
        state.memories[i] = machine_codes[i];
    }
}

void IncreaseProgramCounter() {
    state.program_counter++;
}

void Run() {
    while (!state.is_halt) {
        printState();

        int machine_code = GetMachineCode(state.program_counter);
        IncreaseProgramCounter();

        Opcode opcode = GetOpcode(machine_code);
        int rs = GetRS(machine_code);
        int rt = GetRT(machine_code);
        int rd = GetRD(machine_code);
        if (rd == 0) {
            throw "Can not write to register 0";
        }
        int offset = GetOffset(machine_code);
        switch (opcode) {
            case add:
                cout << "add" << endl;
                state.registers[rd] = state.registers[rs] + state.registers[rt];
                break;
            case nand:
                cout << "nand" << endl;
                state.registers[rd] = ~(state.registers[rs] & state.registers[rt]);
                break; 
            case lw:
                cout << "lw" << endl;
                state.registers[rt] = state.memories[state.registers[rs] + offset];
                break; 
            case sw:
                cout << "sw" << endl;
                break; 
            case beq:
                cout << "beq" << endl;
                break; 
            case jalr:
                cout << "jalr" << endl;
                state.registers[rt] = state.program_counter;
                state.program_counter = state.registers[rs];
                break; 
            case halt:
                cout << "halt" << endl;
                state.is_halt = true;
                break; 
            case noop:
                cout << "noop" << endl;
                break; 
            default:
                char error[SIZE_OF_CHAR_BUFFER];
                sprintf(error, "Unknow opcode %d", opcode);
                throw error;
        }
    }
}

int main(int argc, char **argv) {

    if (argc == 1) {
        cout << "usage: " << argv[0] << " [input file]" << endl;
        return 0;
    }

    ifstream inputFile;
    inputFile.open(argv[1]);
    if (!inputFile.is_open()) {
        cout << "Can't open " << argv[1] << endl;
        return 69;
    }

    try {
        vector<string> lines = file_to_lines(inputFile);
        vector<int> machine_codes = parse(lines);   
        StoreProgramToState(machine_codes);
        
        Run();
        printState();
    } catch(char const* error) {
        cout << "error: " << error << endl;
        return 1;
    }

    return 0;
}