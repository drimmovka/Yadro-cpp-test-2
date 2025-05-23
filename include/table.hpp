#pragma once

#include <optional>
#include <string>

#include "time24.hpp"
#include "exception.hpp"

namespace ccms
{

using TableInfo = std::string;
    
class Table {
// agreement: table doesnt know its id

public:
    explicit Table(int hourly_rate=0);

    ~Table();

    int revenue();

    Time24 totalOccupiedTime();

    TableInfo info() const;

    void reset();

    bool isOccupied() const;

    void setOccupied(const Time24& time);

    void setUnoccupied(const Time24& leave_time);
    

private:
    int _hourly_rate;
    int _total_revenue;
    Time24 _total_occupied_time; // during the day, table cannot be occupied for more than 23:59
    std::optional<Time24> _last_occupied_time;

private:
    void addRevenue(const Time24& occupancy_time);

    void addOccupiedTime(const Time24& time);
};

} // namespace ccms