#pragma once

#include <string>
#include <sstream>
#include <memory>

#include "time24.hpp"
#include "event.hpp"
#include "exception.hpp"

namespace ccms
{

class EventParser {
public:

    EventParser();
    ~EventParser();
    
    std::shared_ptr<iEvent> parse(const std::string& line);

private:
    void checkForFail(std::istream& iss);

    std::shared_ptr<iEvent> parseClientArrivedEvent(std::istream& iss, const Time24& event_time);

    std::shared_ptr<iEvent> parseClientTryToSitEvent(std::istream& iss, const Time24& event_time);

    std::shared_ptr<iEvent> parseClientWaitingEvent(std::istream& iss, const Time24& event_time);

    std::shared_ptr<iEvent> parseClientDeparted(std::istream& iss, const Time24& event_time);
};

} // namespace ccms