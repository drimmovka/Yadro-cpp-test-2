#include "time24.hpp"


Time24::Time24(int hours, int minutes) {
    set(hours, minutes);
}

Time24::Time24(const std::string& formatted_time) {
    set(formatted_time);
}

Time24::~Time24() = default;

// setters
void Time24::set(int hours, int minutes) {
    parseFormattedTime(hours, minutes);
}

void Time24::set(const std::string& formatted_time) {
    parseFormattedTime(formatted_time);
}

// getters
int Time24::h() const {
    return _hours;
}

int Time24::m() const {
    return _minutes;
}

// operator overload
// io operators
std::ostream& operator<<(std::ostream& os, const Time24& time24) {
    os << std::setfill('0') << std::setw(2) << time24._hours << ":" 
        << std::setfill('0') << std::setw(2) << time24._minutes;
    return os;
}

std::istream& operator>>(std::istream& is, Time24& time24) {
    std::string formatted_time;
    is >> formatted_time;
    time24.set(formatted_time);
    return is;
}

// additivity operators
// 1. all arithmetic operations are performed modulo 24 hours
// 2. when subtracting, it is assumed that lhs is always greater rhs (10:00-20:00 = 01:10:00-00:20:00 = 00:14:00)
// 3. The previous statement (2) causes a contradiction between some operations, 
// but in the context of using a class to determine the time within one day, it will not cause inconsistencies.
Time24& Time24::operator+=(const Time24& rhs) {
    _minutes += rhs._minutes;
    _hours += rhs._hours + _minutes / 60;
    _minutes %= 60;
    _hours %= 24;
    return *this;
}

Time24& Time24::operator-=(const Time24& rhs) {
    _minutes -= rhs._minutes;
    if (_minutes < 0) {
        _minutes += 60;
        _hours -= 1;
    }
    
    _hours -= rhs._hours;
    if (_hours < 0) {
        _hours += 24;
    }

    return *this;
}

Time24 operator+(Time24 lhs, const Time24& rhs) {
    lhs += rhs;
    return lhs;
}

Time24 operator-(Time24 lhs, const Time24& rhs) {
    lhs -= rhs;
    return lhs;
}

// comparison operators
bool operator<(const Time24& lhs, const Time24& rhs) {
    if (lhs._hours == rhs._hours) {
        return (lhs._minutes < rhs._minutes);
    }
    return (lhs._hours < rhs._hours);
}

bool operator>(const Time24& lhs, const Time24& rhs) {
    return (rhs < lhs);
}

bool operator<=(const Time24& lhs, const Time24& rhs) {
    return !(lhs > rhs);
}

bool operator>=(const Time24& lhs, const Time24& rhs) {
    return !(lhs < rhs);
}

bool operator==(const Time24& lhs, const Time24& rhs) {
    return (lhs._hours == rhs._hours && lhs._minutes == rhs._minutes);
}

bool operator!=(const Time24& lhs, const Time24& rhs) {
    return !(lhs == rhs);
}

// checks format and initializes fields if validation is OK
void Time24::parseFormattedTime(int hours, int minutes) {
    if (!(hours >= 0 && hours <= 23 && minutes >= 0 && minutes <= 59)) {
        throw Time24Exception("Time format is invalid");
    }

    _hours = hours;
    _minutes = minutes;
}

// checks format and initializes fields if validation is OK
void Time24::parseFormattedTime(const std::string& formatted_time) {
    std::cmatch match;
    std::regex reg(kHHMM24HourRegex);

    if (!std::regex_match(formatted_time.c_str(), match, reg)) {
        throw Time24Exception("Time format is invalid");
    }

    _hours = std::stoi(match[1]);
    _minutes = std::stoi(match[2]);
}