#pragma once

#include "time24.hpp"

using TableInfo = std::string;

class TableException : public std::exception {
public:
    explicit TableException(const std::string& message)
        : _message(message)
    {}

    const char *what() const noexcept override {
        return _message.c_str();
    }
    
protected:
    const std::string _message;
};


class Table {
// agreement: table doesnt know its id

public:
    explicit Table(int hourly_rate=0) {
        if (hourly_rate < 0) {
            throw TableException("Hourly rate cannot be negative");
        }
        _hourly_rate = hourly_rate;
        reset();
    }

    ~Table() = default;

    int revenue() { return _total_revenue; }

    Time24 totalOccupiedTime() { return _total_occupied_time; }

    TableInfo info() const {
        std::ostringstream oss;
        oss << _total_revenue << " " << _total_occupied_time;
        return oss.str();
    }

    void reset() {
        _total_revenue = 0;
        _total_occupied_time.set(0, 0);
        _last_occupied_time.reset();
    }

    bool isOccupied() const {
        return (_last_occupied_time.has_value());
    }

    void setOccupied(const Time24& time) {
        _last_occupied_time = time;
    } 

    void setUnoccupied(const Time24& leave_time) {
        if (!isOccupied()) {
            throw TableException("Additional revenue cannot be calculated (table was not occupied)");
        }
        if (leave_time < _last_occupied_time) {
            throw TableException("Additional revenue cannot be calculated (time mismatch)");
        }

        Time24 occupancy_time = leave_time - _last_occupied_time.value();
        addRevenue(occupancy_time);
        addOccupiedTime(occupancy_time);
        _last_occupied_time.reset();
    }
    

private:
    int _hourly_rate;
    int _total_revenue;
    Time24 _total_occupied_time; // during the day, table cannot be occupied for more than 23:59
    std::optional<Time24> _last_occupied_time;

private:
    void addRevenue(const Time24& occupancy_time) {
        _total_revenue += _hourly_rate * (occupancy_time.h() + static_cast<int>(occupancy_time.m() > 0));
    }

    void addOccupiedTime(const Time24& time) {
        _total_occupied_time += time;
    }
};