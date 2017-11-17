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

// copy from example code 
struct State {
    int program_counter;
    int memories[SIZE_OF_MEMORY];
    int registers[SIZE_OF_REGISTER];
    int number_of_instructions;
    bool is_halt; // add halt state
} state;

// copy again
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

// read line from file stream and store it in dynamic array
vector<string> file_to_lines(ifstream &file) {
    vector<string> lines;
    string line;
    while (getline(file, line)) {
        lines.push_back(line);
    }

    return lines;
}

// convert string form to integer form
vector<int> parse(vector<string> lines) {
    vector<int> machineCodes;
    for (string line: lines) {
        machineCodes.push_back(stoi(line));
    }

    return machineCodes;
}

// extract opcode
Opcode GetOpcode(int machine_code) {
    return Opcode(machine_code >> 22);
}

// check if register number is valid
int ValidRegisterNumber(int number) {
    if (number > 7 or number < 0) {
        char error[SIZE_OF_CHAR_BUFFER];
        sprintf(error, "Unknow invalid register %d", number);
        throw error;
    }

    return number;
}

// extract rs
int GetRS(int machine_code) {
    return ValidRegisterNumber((machine_code >> 19) & 7);
}

// extract rt
int GetRT(int machine_code) {
    return ValidRegisterNumber((machine_code >> 16) & 7);
}

// extract rd
int GetRD(int machine_code) {
    return ValidRegisterNumber(machine_code & 7);
}

// extract offset and signed extend to 32-bit signed integer
int GetOffset(int machine_code) {
    int offset = machine_code & 0xffff;
    if (offset & (1 << 15)) {
        offset -= (1 << 16); 
    }
    return offset;
}

// get current instruction
int GetMachineCode(int program_counter) {
    return state.memories[program_counter];
}

// store machine code to memory
void StoreProgramToState(vector<int> machine_codes) {
    state.number_of_instructions = machine_codes.size();

    for (int i = 0; i < machine_codes.size(); i++) {
        state.memories[i] = machine_codes[i];
    }
}

void IncreaseProgramCounter() {
    state.program_counter++;
}

// cant write to register 0
void CheckRD(int rd) {
    if (rd == 0) {
        throw "Can not write to register 0";
    }
}

void Run() {
    while (!state.is_halt) {
        // start clock print state
        printState();

        // thrown an error if program counter is out of legit range
        if ((state.program_counter < 0) or (state.program_counter >= state.number_of_instructions)) {
            throw "Invalid program counter";
        }

        int machine_code = GetMachineCode(state.program_counter);
        IncreaseProgramCounter();

        // extract arguments from machine code
        Opcode opcode = GetOpcode(machine_code);
        int rs = GetRS(machine_code);
        int rt = GetRT(machine_code);
        int rd = GetRD(machine_code);
        int offset = GetOffset(machine_code);

        // simulate action by opcode
        switch (opcode) {
            case add:
                CheckRD(rd);
                state.registers[rd] = state.registers[rs] + state.registers[rt];
                break;
            case nand:
                CheckRD(rd);
                state.registers[rd] = ~(state.registers[rs] & state.registers[rt]);
                break; 
            case lw:
                CheckRD(rt);
                state.registers[rt] = state.memories[state.registers[rs] + offset];
                break; 
            case sw:
                state.memories[state.registers[rs] + offset] = state.registers[rt];
                break; 
            case beq:
                if (state.registers[rs] == state.registers[rt]) {
                    state.program_counter = state.program_counter + offset;
                }
                break; 
            case jalr:
                CheckRD(rt);
                state.registers[rt] = state.program_counter;
                state.program_counter = state.registers[rs];
                break; 
            case halt:
                state.is_halt = true;
                break; 
            case noop:
                break; 
            default:
                char error[SIZE_OF_CHAR_BUFFER];
                sprintf(error, "Unknow opcode %d", opcode);
                throw error;
        }
    }
}

int main(int argc, char **argv) {
    // check argument for input file path
    if (argc == 1) {
        cout << "usage: " << argv[0] << " [input file]" << endl;
        return 0;
    }

    // try to open file stream to read
    ifstream inputFile;
    inputFile.open(argv[1]);
    // check if file stream can open
    if (!inputFile.is_open()) {
        cout << "Can't open " << argv[1] << endl;
        return 1;
    }

    try {
        // read file to array
        vector<string> lines = file_to_lines(inputFile);
        // convert string to int
        vector<int> machine_codes = parse(lines);
        // store instructions in memory
        StoreProgramToState(machine_codes);
        
        // start simulation
        Run();

        // print latest state after program completed
        printState();
    } catch(char const* error) {
        cout << "error: " << error << endl;
        return 1;
    }

    return 0;
}