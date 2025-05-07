#pragma once

#include <memory>

#include "input_parser.hpp"
#include "output_writer.hpp"
#include "computer_club.hpp"
#include "computer_club_manager.hpp"


class Application {
public:
    
    explicit Application(std::istream& is, std::ostream& os);
    
    ~Application();

    void run();

private:
    InputParser _input_parser;
    OutputWriter _output_writer;
};