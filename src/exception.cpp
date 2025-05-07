#include "exception.hpp"

namespace ccms
{

ClientException::ClientException(const std::string& message)
    : _message(message)
{}

const char *ClientException::what() const noexcept {
    return _message.c_str();
}


ComputerClubManagerException::ComputerClubManagerException(const std::string& message)
    : _message(message)
{}

const char* ComputerClubManagerException::what() const noexcept { 
    return _message.c_str();
}


ComputerClubException::ComputerClubException(const std::string& message)
    : _message(message)
{}

const char *ComputerClubException::what() const noexcept {
    return _message.c_str();
}
    

EventParserException::EventParserException(const std::string& message)
    : _message(message)
{}

const char *EventParserException::what() const noexcept {
    return _message.c_str();
}


InputParserException::InputParserException(const std::string& message)
    : _message(message)
{}

const char *InputParserException::InputParserException::what() const noexcept {
    return _message.c_str();
}


TableException::TableException(const std::string& message)
    : _message(message)
{}

const char *TableException::what() const noexcept {
    return _message.c_str();
}


Time24Exception::Time24Exception(const std::string& message)
    : _message(message)
{}

const char *Time24Exception::what() const noexcept {
    return _message.c_str();
}

} // namespace ccms