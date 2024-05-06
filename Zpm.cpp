// Nicholas McCarty
// CSE 465
// Z++ Interpreter

#include <iostream>
#include <map>
#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <algorithm>>

void parseAndExecute(const std::string& filename);
void handlePrint(const std::string& variable);
void handleAssignment(const std::string& line, int lineNumber);
 std::map<std::string, std::string> variables;


/* Main runner function. 
*/
int main(int argc, char* argv[]) {
    
    // Checking valid input
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>.zpm" << std::endl;
        return 1;
    }

    // Calling helper method
    try {
        parseAndExecute(argv[1]);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}

   

void parseAndExecute(const std::string& filename) {
    std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file: " + filename);
        } else {
            std::cout << "File opened sucessfully" << std::endl;
        }

       

        std::string line;
        int lineNumber = 0;

        while (getline(file, line)) {
            lineNumber++; // Increment line number with each new line read

            if (line.empty() || line[0] == '#') { // Skip empty lines or lines starting with '#'
                continue;
            }

            size_t pos = line.find(' '); // Find the first space to isolate the command
            std::string command = line.substr(0, pos);

            if (command == "PRINT") {
                /// handlePrint(line.substr(pos + 1)); // Pass the rest of the line to handlePrint
            } else {
                /// handleAssignment(line, lineNumber);
            }
        }

        file.close();
    }


void handlePrint(const std::string& variable) {

}

void handleAssignment(const std::string& line, int lineNumber) {
   
}

// ChatGPT function for assisting readability. 
void trim(std::string& str) {
        auto left = std::find_if(str.begin(), str.end(), [](char ch) {
            return !std::isspace(ch);
        });
        auto right = std::find_if(str.rbegin(), str.rend(), [](char ch) {
            return !std::isspace(ch);
        }).base();
        if (left < right) {
            str = std::string(left, right);
        } else {
            str.clear(); // The string is all whitespace
        }
    }