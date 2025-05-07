#include "event_parser.hpp"

namespace ccms
{

EventParser::EventParser() = default;
EventParser::~EventParser() = default;

std::shared_ptr<iEvent> EventParser::parse(const std::string& line) {
    std::istringstream iss(line);
    Time24 event_time;
    id_t event_id;
    
    iss >> event_time; checkForFail(iss);
    iss >> event_id; checkForFail(iss);

    switch(event_id) {
        case 1: return parseClientArrivedEvent(iss, event_time);
        case 2: return parseClientTryToSitEvent(iss, event_time);
        case 3: return parseClientWaitingEvent(iss, event_time);
        case 4: return parseClientDeparted(iss, event_time);
        default: throw EventParserException("Unknown event id");
    }
}

void EventParser::checkForFail(std::istream& iss) {
    if (iss.fail()) {
        iss.clear();
        throw EventParserException("Invalid input format");
    }
}

std::shared_ptr<iEvent> EventParser::parseClientArrivedEvent(std::istream& iss, const Time24& event_time) {
    std::string client_name;
    iss >> client_name; checkForFail(iss);
    return std::make_shared<ClientArrivedEvent>(
        event_time, 
        std::make_shared<ClientArrivedEventBody>(Client(client_name))
    );
}

std::shared_ptr<iEvent> EventParser::parseClientTryToSitEvent(std::istream& iss, const Time24& event_time) {
    std::string client_name;
    int table_id;
    iss >> client_name; checkForFail(iss);
    iss >> table_id; checkForFail(iss);
    return std::make_shared<ClientTryToSitEvent>(
        event_time,
        std::make_shared<ClientTryToSitEventBody>(Client(client_name), table_id)
    );
}

std::shared_ptr<iEvent> EventParser::parseClientWaitingEvent(std::istream& iss, const Time24& event_time) {
    std::string client_name;
    iss >> client_name; checkForFail(iss);
    return std::make_shared<ClientWaitingEvent>(
        event_time,
        std::make_shared<ClientWaitingEventBody>(Client(client_name))
    );
}

std::shared_ptr<iEvent> EventParser::parseClientDeparted(std::istream& iss, const Time24& event_time) {
    std::string client_name;
    iss >> client_name; checkForFail(iss);
    return std::make_shared<ClientDeparted>(
        event_time,
        std::make_shared<ClientDepartedBody>(Client(client_name))
    );
}

} // namespace ccms