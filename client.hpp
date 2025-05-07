#pragma once

#include <string>
#include <functional>
#include <regex>


class ClientException : public std::exception {
public:
    explicit ClientException(const std::string& message)
        : _message(message)
    {}

    const char *what() const noexcept override {
        return _message.c_str();
    }
    
private:
    const std::string _message;
};


class Client {
public:
    Client() = default;
    ~Client() = default;


    Client(const std::string& name) {
        parseName(name);
    }

    struct ClientHash {
        size_t operator()(const Client& client) const {
            return std::hash<std::string>()(client._name);
        }
    };

// operator overload
    // io operators
    friend std::ostream& operator<<(std::ostream& os, const Client& client) {
        os << client._name;
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Client& client) {
        std::string name;
        is >> name;
        client.parseName(name);
        return is;
    }
    
    // comparison operators
    friend bool operator==(const Client& lhs, const Client& rhs) {
        return (lhs._name == rhs._name);
    }

    friend bool operator<(const Client& lhs, const Client& rhs) {
        return (lhs._name < rhs._name);
    }

private:
    std::string _name;
    const char *kNameRegex = "^[a-z0-9_-]+$"; // TODO: make it static

private:

    // checks name and initializes fields if validation is OK
    void parseName(const std::string& name) {
        std::cmatch match;
        std::regex reg(kNameRegex);

        if (!std::regex_match(name.c_str(), match, reg)) {
            throw ClientException("Name format is invalid");
        }

        _name = name;
    }

};