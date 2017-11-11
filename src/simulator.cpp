#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

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