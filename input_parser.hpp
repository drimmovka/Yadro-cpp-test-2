#include <utility>
#include "event_parser.hpp"

class InputParserException : public std::exception {
public:
    explicit InputParserException(const std::string& message)
        : _message(message)
    {}

    const char *what() const noexcept override {
        return _message.c_str();
    }
    
protected:
    const std::string _message;
};
    

class InputParser {

public:

    explicit InputParser(std::istream& is) 
        : _is(is)
        , _last_read_line_number(0)
    {}

    ~InputParser() = default;

    int inputTablesNumber() {
        readInBuf();

        int tables_number;
        _buf >> tables_number; checkForFail();
        return tables_number;
    }

    std::pair<Time24, Time24> inputStartEndTime() {
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

    int inputHourlyRate() {
        readInBuf();

        int hourly_rate;
        _buf >> hourly_rate; checkForFail();
        return hourly_rate;
    }

    std::vector<std::shared_ptr<iEvent>> inputEvents() {
        std::vector<std::shared_ptr<iEvent>> events;

        readInBuf();
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
    
    std::string lastReadLine() const {
        return _last_read_line;
    }

    int lastReadLineNumber() const {
        return _last_read_line_number;
    }

private:
    std::istream& _is;
    std::istringstream _buf;
    std::string _last_read_line;
    int _last_read_line_number;
    EventParser _event_parser;

private:
    void checkForFail() {
        if (_buf.fail()) {
            _buf.clear();
            throw InputParserException("Invalid input format");
        }
    }

    void readInBuf() {
        if (!std::getline(_is, _last_read_line)) {
            _last_read_line.clear();
        }
        _buf.clear();
        _buf.str(_last_read_line);
        ++_last_read_line_number;
    }
};