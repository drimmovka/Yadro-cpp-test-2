#pragma once

#include "time24.hpp"

class Table {
public:
    Table() {
        resetFields();
    }

    ~Table() = default;

    bool isOccupied() const {
        return _is_occupied;
    }

    void setOccupiedStatus(bool status) {
        _is_occupied = status;
    } 

    void addOccupiedTime(Time24 time) {
        _occupied_time += time;
    }

private:
    bool _is_occupied;
    Time24 _occupied_time; // during the day, table cannot be occupied for more than 23:59

private:
    void resetFields() {
        _is_occupied = false;
        _occupied_time.set(0, 0);
    }
};