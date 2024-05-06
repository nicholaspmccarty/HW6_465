// Nicholas McCarty
// CSE 465
// Z++ Interpreter

#include <iostream>
#include <map>
#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <algorithm>


// Declaring methods / variables for guaranteed method locality.
void parseAndExecute(const std::string& filename);
void handlePrint(const std::string& variable);
void handleAssignment(std::string& line, int lineNumber);
void trim(std::string& str);
void printData();
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
    
    parseAndExecute(argv[1]);
    // printData();
}

   
// Main parse and execute function for parsing the file.
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

            std::string argument = line.substr(pos + 1);
            trim(argument); 

            if (command == "PRINT") {
             handlePrint(argument); // Pass the rest of the line to handlePrint
            } else {
                handleAssignment(line, lineNumber);
            }
        }

        file.close();
    }


// Method for handling our print functionality.
void handlePrint(const std::string& variable) {
    
    // Check if the variable exists in the map
    if (variables.find(variable) != variables.end()) {
        // If found, print the variable and its value
        std::cout << variable << "=" << variables[variable] << std::endl;
    } else {
        // If not found, print that the variable is undefined
        std::cerr << "Error: Variable '" << variable << "' is not defined." << std::endl;
    }
}


// Method for handling assignment. 
// Method for handling assignment.
void handleAssignment(std::string& line, int lineNumber) {
    trim(line); // Remove any leading or trailing whitespace
    size_t equalPos = line.find('=');
    if (equalPos == std::string::npos) {
        std::cerr << "Syntax error in line " << lineNumber << ": '=' not found." << std::endl;
        return;
    }

    std::string variableName = line.substr(0, equalPos);
    trim(variableName); // Clean up the variable name

    std::string value = line.substr(equalPos + 1);
    trim(value); // Clean up the value string

    if (value.empty()) {
        std::cerr << "Syntax error in line " << lineNumber << ": No value provided for variable." << std::endl;
        return;
    }
    

    if (variables.find(value) != variables.end()) {
        // If value is a variable name, assign the value from the map
        variables[variableName] = variables[value];
    } else {
        // Otherwise, just assign the literal value
        variables[variableName] = value;
    }

    // std::cout << "Assigned " << variableName << " = " << value << " at line " << lineNumber << std::endl;
}



// ChatGPT function for assisting readability. 
void trim(std::string& str) {
    // Remove leading whitespaces
    auto left = std::find_if(str.begin(), str.end(), [](char ch) {
        return !std::isspace(ch);
    });

    // Remove trailing whitespaces and semicolons
    auto right = std::find_if(str.rbegin(), str.rend(), [](char ch) {
        return !std::isspace(ch) && ch != ';';
    }).base();

    // Check if the valid string range is determined correctly
    if (left < right) {
        str = std::string(left, right);
    } else {
        str.clear(); // The string is all whitespace or semicolons
    }
}

// Simple debugging method.
void printData() {
    std::cout << "Print Data Loaded" << std::endl;
    for (const auto& var : variables) {
        std::cout << var.first << " = " << var.second << std::endl;
    }
}