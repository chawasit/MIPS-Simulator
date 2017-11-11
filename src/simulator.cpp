#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#define SIZE_OF_MEMORY 65536
#define SIZE_OF_REGISTER 8 

using namespace std;

struct State {
    int program_counter;
    int memories[SIZE_OF_MEMORY];
    int registers[SIZE_OF_REGISTER];
    int number_of_instructions;
} state;

void printState(State &state) {
    cout << "\n@@@\nstate:\n";
    cout << "\tpc "<<state.program_counter<<endl;
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
        vector<int> machineCodes = parse(lines);

        // Main code
        for(auto code: machineCodes)
            cout << code << endl;

    } catch(char const* error) {
        cout << "error: " << error << endl;
        return 1;
    }

    return 0;
}