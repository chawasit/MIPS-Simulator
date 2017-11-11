#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>

using namespace std;

vector<string> file_to_lines(ifstream &file) {
    vector<string> lines;
    string line;
    while (getline(file, line)) {
        lines.push_back(line);
    }
    return lines;
}

vector<string> parse_argument(string line) {
    istringstream iss(line);
    vector<string> arguments;
    string argument;
    while(iss >> argument) {
        arguments.push_back(argument);
    }
    cout << endl;
    return arguments;
}

vector<int> parse(vector<string> lines) {
    vector<int> machineCodes;
    for (string line: lines) {
        parse_argument(line);
    }
    return machineCodes;
}

int main(int argc, char **argv) {

    if (argc == 1) {
        cout << "usage: " << argv[0] << " [in] [out]" << endl;
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
    } catch(char const* error) {
        cout << "error: " << error << endl;
        return 1;
    }

    if (argc == 3) {
        ofstream outputFile;
        outputFile.open(argv[2]);
        if (!outputFile.is_open()) {
            cout << "Can't open " << argv[2] << endl;
            return 69;
        }
    }

    return 0;
}