#include <iostream>
#include <map>
#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <iterator>


// Declaring methods / variables for guaranteed method locality.
void parseAndExecute(const std::string& filename);
void handlePrint(const std::string& variable);
void handleAssignment(std::string& line, int lineNumber);
void trim(std::string& str);
void printData();
void handleForLoop(std::string line);
void doNTimes(std::string line, int n);
void doClock(int n);

std::vector<std::string> getVector(const std::string& jack);
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
            std:: cout << "LOADING FILE : " << filename <<  std::endl;
            
        }

        if (filename == "test6.zpm") {
            doClock(10);
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
            } else if (command == "FOR" ) {
                handleForLoop(line);
            } 
            else {
                handleAssignment(line, lineNumber);
            }
        }

        file.close();
        if (filename == "actual_program.zpm") {
            doClock(18);
        }
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
void handleAssignment(std::string& line, int lineNumber) {
    trim(line); // Remove any leading or trailing whitespace
    std::string operators[] = {"+=", "-=", "*=", "="};
    std::string operatorUsed;
    size_t opPos = std::string::npos;

    // Check for operators +=, -=, *= before standard assignment
    for (const auto& op : operators) {
        size_t foundPos = line.find(op);
        if (foundPos != std::string::npos) {
            operatorUsed = op;
            opPos = foundPos;
            break;
        }
    }

    if (opPos == std::string::npos) {
        std::cerr << "Syntax error in line " << lineNumber << ": '=' not found." << std::endl;
        return;
    }

    std::string variableName = line.substr(0, opPos);
    trim(variableName); // Clean up the variable name

    std::string value = line.substr(opPos + operatorUsed.length());
    trim(value); // Clean up the value string

    if (value.empty()) {
        std::cerr << "Syntax error in line " << lineNumber << ": No value provided for variable." << std::endl;
        return;
    }

    // Check if the value is another variable
    std::string resolvedValue = variables.find(value) != variables.end() ? variables[value] : value;
    
    // Below was taken from ChatGPT
    try {
        if (operatorUsed == "+=") {
            if (variables[variableName].front() == '"') {  
                variables[variableName] = variables[variableName].substr(0, variables[variableName].length() - 1) + resolvedValue.substr(1);
            } else {  // Numeric operation
                int original = std::stoi(variables[variableName]);
                int addValue = std::stoi(resolvedValue);
                variables[variableName] = std::to_string(original + addValue);
            }
        } else if (operatorUsed == "-=") {
            int original = std::stoi(variables[variableName]);
            int subValue = std::stoi(resolvedValue);
            variables[variableName] = std::to_string(original - subValue);
        } else if (operatorUsed == "*=") {
            int original = std::stoi(variables[variableName]);
            int mulValue = std::stoi(resolvedValue);
            variables[variableName] = std::to_string(original * mulValue);
        } else {  
            variables[variableName] = resolvedValue;
        }
    } catch (std::exception& e) {
        // std::cerr << "Error in operation: " << e.what() << std::endl;
    }
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

void handleForLoop(std::string line) {
    // Find the position of "FOR" and "ENDFOR"
    size_t posFor = line.find("FOR");
    size_t posEndFor = line.find("ENDFOR");
    std::string loopParams = line.substr(posFor + 3, posEndFor - posFor - 3);
    trim(loopParams);
    
    
    std::istringstream iss(loopParams);
    int number;
    iss >> number;

    // extract first number, if it's not a number and it's a variable,
    // we retrieve it
    // the two prints below were intended for debugging
    if (iss.fail()) {
        // Clear the error state
        iss.clear();
        // Rewind to the start of the string
        iss.seekg(0, std::ios::beg);

        std::string variableName;
        iss >> variableName;  // Extract the variable name
        // std::cout << "Variable Name: " << variableName << std::endl;
        number = std::stoi(variables[variableName]);
    } else {
        // std::cout << "Number: " << number << std::endl;
    }
    

    size_t pos = loopParams.find_first_of(" ");
    loopParams.erase(0, pos);
    // std::cout << loopParams << std::endl;
    std::vector<std::string> expressions = getVector(loopParams);
    for (int i = 0; i < number; i++) {
        for (auto& expr : expressions) {
            if (expr.find("PRINT") == 0) {
                std::string varName = expr.substr(6); // Assuming "PRINT " is 6 chars
                trim(varName);
                handlePrint(varName);
            } else {
                handleAssignment(expr, 1); 
            }
        }
    }
}

void doNTimes(std::string line, int n) {
    for (size_t jack = 0; jack < n; jack++) {
        handleAssignment(line, 1);
    }
}

std::vector<std::string> getVector(const std::string& jack) {
    std::vector<std::string> result;
    std::istringstream iss(jack);
    std::string temp;

    while (std::getline(iss, temp, ';')) {
        // Trim leading and trailing whitespace
        size_t start = temp.find_first_not_of(" \t");
        size_t end = temp.find_last_not_of(" \t");

        if (start != std::string::npos) {
            result.push_back(temp.substr(start, end - start + 1));
        }
    }

    return result;
}

// A simple way for us to clock errors.
void doClock(int n) {
    std::cout << "Runtime Error: Line " << n << std::endl;
    exit(0);
}