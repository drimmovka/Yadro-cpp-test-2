#pragma once

#include <exception>
#include <string>

namespace ccms
{

class ClientException : public std::exception {
public:
    explicit ClientException(const std::string& message);

    const char *what() const noexcept override;
    
private:
    const std::string _message;
};


class ComputerClubManagerException : public std::exception {
public:
    explicit ComputerClubManagerException(const std::string& message);
    
    const char* what() const noexcept override;

private:
    const std::string _message;
};


class ComputerClubException : public std::exception {
public:
    explicit ComputerClubException(const std::string& message);

    const char *what() const noexcept override;
    
protected:
    const std::string _message;
};


class EventParserException : public std::exception {
public:
    explicit EventParserException(const std::string& message);

    const char *what() const noexcept override;
    
protected:
    const std::string _message;
};


class InputParserException : public std::exception {
public:
    explicit InputParserException(const std::string& message);

    const char *what() const noexcept override;
    
protected:
    const std::string _message;
};


class TableException : public std::exception {
public:
    explicit TableException(const std::string& message);

    const char *what() const noexcept override;
    
protected:
    const std::string _message;
};


class Time24Exception : public std::exception {
public:
    explicit Time24Exception(const std::string& message);

    const char *what() const noexcept override;
    
private:
    const std::string _message;
};

} // namespace ccms