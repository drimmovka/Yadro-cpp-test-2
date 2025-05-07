#include <string>
#include <sstream>
#include <memory>
#include "time24.hpp"
#include "event.hpp"


class EventParserException : public std::exception {
public:
    explicit EventParserException(const std::string& message)
        : _message(message)
    {}

    const char *what() const noexcept override {
        return _message.c_str();
    }
    
protected:
    const std::string _message;
};

class EventParser {
public:
    std::shared_ptr<iEvent> parse(const std::string& line) {
        std::istringstream iss(line);
        Time24 event_time;
        id_t event_id;
        
        iss >> event_time >> event_id;

        switch(event_id) {
            case 1: return parseClientArrivedEvent(iss, event_time);
            case 2: return parseClientTryToSitEvent(iss, event_time);
            case 3: return parseClientWaitingEvent(iss, event_time);
            case 4: return parseClientDeparted(iss, event_time);
            default: throw EventParserException("Unknown event id");
        }
    }

private:
    std::shared_ptr<iEvent> parseClientArrivedEvent(std::istream& iss, const Time24& event_time) {
        std::string client_name;
        iss >> client_name;
        return std::make_shared<ClientArrivedEvent>(
            event_time, 
            std::make_shared<ClientArrivedEventBody>(Client(client_name))
        );
    }

    std::shared_ptr<iEvent> parseClientTryToSitEvent(std::istream& iss, const Time24& event_time) {
        std::string client_name;
        int table_id;
        iss >> client_name >> table_id;
        return std::make_shared<ClientTryToSitEvent>(
            event_time,
            std::make_shared<ClientTryToSitEventBody>(Client(client_name), table_id)
        );
    }

    std::shared_ptr<iEvent> parseClientWaitingEvent(std::istream& iss, const Time24& event_time) {
        std::string client_name;
        iss >> client_name;
        return std::make_shared<ClientWaitingEvent>(
            event_time,
            std::make_shared<ClientWaitingEventBody>(Client(client_name))
        );
    }

    std::shared_ptr<iEvent> parseClientDeparted(std::istream& iss, const Time24& event_time) {
        std::string client_name;
        iss >> client_name;
        return std::make_shared<ClientDeparted>(
            event_time,
            std::make_shared<ClientDepartedBody>(Client(client_name))
        );
    }
};