#include "client.hpp"

namespace ccms
{

Client::Client() = default;
Client::~Client() = default;

Client::Client(const std::string& name) {
    parseName(name);
}

size_t Client::ClientHash::operator()(const Client& client) const {
    return std::hash<std::string>()(client._name);
}

// operator overload
// io operators
std::ostream& operator<<(std::ostream& os, const Client& client) {
    os << client._name;
    return os;
}

std::istream& operator>>(std::istream& is, Client& client) {
    std::string name;
    is >> name;
    client.parseName(name);
    return is;
}

// comparison operators
bool operator==(const Client& lhs, const Client& rhs) {
    return (lhs._name == rhs._name);
}

bool operator<(const Client& lhs, const Client& rhs) {
    return (lhs._name < rhs._name);
}

// checks name and initializes fields if validation is OK
void Client::parseName(const std::string& name) {
    std::cmatch match;
    std::regex reg(kNameRegex);

    if (!std::regex_match(name.c_str(), match, reg)) {
        throw ClientException("Name format is invalid");
    }

    _name = name;
}

} // namespace ccms