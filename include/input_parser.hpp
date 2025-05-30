#pragma once

#include <utility>

#include "event_parser.hpp"
#include "exception.hpp"

namespace ccms
{

class InputParser {

public:

    explicit InputParser(std::istream& is);

    ~InputParser();

    int inputTablesNumber();

    std::pair<Time24, Time24> inputStartEndTime();

    int inputHourlyRate();

    std::vector<std::shared_ptr<iEvent>> inputEvents();
    
    std::string lastReadLine() const;

    int lastReadLineNumber() const;

private:
    std::istream& _is;
    std::istringstream _buf;
    std::string _last_read_line;
    int _last_read_line_number;
    EventParser _event_parser;

private:
    void checkForFail();

    void readInBuf();
};

} // namespace ccms