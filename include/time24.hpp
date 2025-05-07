#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <regex>


class Time24Exception : public std::exception {
public:
    explicit Time24Exception(const std::string& message)
        : _message(message)
    {}

    const char *what() const noexcept override {
        return _message.c_str();
    }
    
private:
    const std::string _message;
};


class Time24 {
public:
    explicit Time24(int hours=0, int minutes=0);

    explicit Time24(const std::string& formatted_time);

    ~Time24();

// setters
    void set(int hours, int minutes);

    void set(const std::string& formatted_time);

// getters
    int h() const;

    int m() const;

// operator overload
    // io operators
    friend std::ostream& operator<<(std::ostream& os, const Time24& time24);

    friend std::istream& operator>>(std::istream& is, Time24& time24);

    // additivity operators
    // 1. all arithmetic operations are performed modulo 24 hours
    // 2. when subtracting, it is assumed that lhs is always greater rhs (10:00-20:00 = 01:10:00-00:20:00 = 00:14:00)
    // 3. The previous statement (2) causes a contradiction between some operations, 
    // but in the context of using a class to determine the time within one day, it will not cause inconsistencies.
    Time24& operator+=(const Time24& rhs);

    Time24& operator-=(const Time24& rhs);

    friend Time24 operator+(Time24 lhs, const Time24& rhs);

    friend Time24 operator-(Time24 lhs, const Time24& rhs);
    
    // comparison operators
    friend bool operator<(const Time24& lhs, const Time24& rhs);

    friend bool operator>(const Time24& lhs, const Time24& rhs);
    
    friend bool operator<=(const Time24& lhs, const Time24& rhs);

    friend bool operator>=(const Time24& lhs, const Time24& rhs);

    friend bool operator==(const Time24& lhs, const Time24& rhs);

    friend bool operator!=(const Time24& lhs, const Time24& rhs);

private:
    // int _days; // can be used in the future
    int _hours;
    int _minutes;
    const char *kHHMM24HourRegex = "^([01]?[0-9]|2[0-3]):([0-5][0-9])$"; // TODO: make it static

private:

    // checks format and initializes fields if validation is OK
    void parseFormattedTime(int hours, int minutes);

    // checks format and initializes fields if validation is OK
    void parseFormattedTime(const std::string& formatted_time);
    
};