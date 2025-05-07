#include "input_parser.hpp"

InputParser::InputParser(std::istream& is) 
    : _is(is)
    , _last_read_line_number(0)
{}

InputParser::~InputParser() = default;

int InputParser::inputTablesNumber() {
    readInBuf();

    int tables_number;
    _buf >> tables_number; checkForFail();
    return tables_number;
}

std::pair<Time24, Time24> InputParser::inputStartEndTime() {
    readInBuf();
    Time24 start_time, end_time;
    try {
        _buf >> start_time; checkForFail();
        _buf >> end_time; checkForFail();
    }
    catch (std::exception& e) {
        throw InputParserException(e.what());
    }
    return std::make_pair(start_time, end_time);
}

int InputParser::inputHourlyRate() {
    readInBuf();

    int hourly_rate;
    _buf >> hourly_rate; checkForFail();
    return hourly_rate;
}

std::vector<std::shared_ptr<iEvent>> InputParser::inputEvents() {
    std::vector<std::shared_ptr<iEvent>> events;

    while (true) {
        readInBuf();
        if (_last_read_line.empty()) {
            break;
        }

        try {
            events.emplace_back(_event_parser.parse(_last_read_line));
        } catch (std::exception& e) {
            throw InputParserException(e.what());
        }
    }
    
    return events;
}

std::string InputParser::lastReadLine() const {
    return _last_read_line;
}

int InputParser::lastReadLineNumber() const {
    return _last_read_line_number;
}

void InputParser::checkForFail() {
    if (_buf.fail()) {
        _buf.clear();
        throw InputParserException("Invalid input format");
    }
}

void InputParser::readInBuf() {
    if (!std::getline(_is, _last_read_line)) {
        _last_read_line.clear();
    }
    _buf.clear();
    _buf.str(_last_read_line);
    ++_last_read_line_number;
}