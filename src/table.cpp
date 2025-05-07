#include "table.hpp"


Table::Table(int hourly_rate) {
    if (hourly_rate < 0) {
        throw TableException("Hourly rate cannot be negative");
    }
    _hourly_rate = hourly_rate;
    reset();
}

Table::~Table() = default;

int Table::revenue() { return _total_revenue; }

Time24 Table::totalOccupiedTime() { return _total_occupied_time; }

TableInfo Table::info() const {
    std::ostringstream oss;
    oss << _total_revenue << " " << _total_occupied_time;
    return oss.str();
}

void Table::reset() {
    _total_revenue = 0;
    _total_occupied_time.set(0, 0);
    _last_occupied_time.reset();
}

bool Table::isOccupied() const {
    return (_last_occupied_time.has_value());
}

void Table::setOccupied(const Time24& time) {
    _last_occupied_time = time;
} 

void Table::setUnoccupied(const Time24& leave_time) {
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

void Table::addRevenue(const Time24& occupancy_time) {
    _total_revenue += _hourly_rate * (occupancy_time.h() + static_cast<int>(occupancy_time.m() > 0));
}

void Table::addOccupiedTime(const Time24& time) {
    _total_occupied_time += time;
}