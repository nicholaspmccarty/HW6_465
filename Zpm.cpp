#include <iostream>
#include <map>
#include <string>


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>.zpm" << std::endl;
        return 1;
    }

    try {
        parseAndExecute(argv[1]);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}

   



void parseAndExecute(const std::string& filename) {


}
