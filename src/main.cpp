#include <iostream>
#include <fstream>

#include "application.hpp"

int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    std::ifstream file_stream(argv[1]);
    if (!file_stream.is_open()) {
        std::cerr << "Cannot open file: " << argv[1] << std::endl;
        return 1;
    }
    
    ccms::Application application(file_stream, std::cout);
    application.run();
    return 0;
}