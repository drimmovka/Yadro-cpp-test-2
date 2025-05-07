#pragma once

#include <string>
#include <functional>
#include <regex>

#include "exception.hpp"

class Client {
public:
    Client();
    ~Client();


    Client(const std::string& name);

    struct ClientHash {
        size_t operator()(const Client& client) const;
    };

// operator overload
    // io operators
    friend std::ostream& operator<<(std::ostream& os, const Client& client);

    friend std::istream& operator>>(std::istream& is, Client& client);
    
    // comparison operators
    friend bool operator==(const Client& lhs, const Client& rhs);

    friend bool operator<(const Client& lhs, const Client& rhs);

private:
    std::string _name;
    const char *kNameRegex = "^[a-z0-9_-]+$"; // TODO: make it static

private:

    // checks name and initializes fields if validation is OK
    void parseName(const std::string& name);

};