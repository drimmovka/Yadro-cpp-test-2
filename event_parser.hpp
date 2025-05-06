#include <string>
#include <sstream>
#include "time24.hpp"
#include "event.hpp"

class EventParser {
public:
    static iEvent* parse(const std::string& line) {
        std::istringstream iss(line);
        Time24 evtime;
        id_t event_id;
        
        iss >> evtime >> event_id;

        switch(event_id) {
            case 1: return parseClientArrivedEvent(iss, evtime);
            case 2: return parseClientTryToSitEvent(iss, evtime);
            case 3: return parseClientWaitingEvent(iss, evtime);
            case 4: return parseClientLeftEvent4(iss, evtime);
            default: throw std::runtime_error("Unknown event id");
        }
    }

private:
    static iEvent* parseClientArrivedEvent(std::istream& iss, Time24 evtime) {
        std::string client_name;
        iss >> client_name;
        return new ClientArrivedEvent(
            evtime, 
            new ClientArrivedEventBody(Client(client_name))
        );
    }

    static iEvent* parseClientTryToSitEvent(std::istream& iss, Time24 evtime) {
        std::string client_name;
        int table_id;
        iss >> client_name >> table_id;
        return new ClientTryToSitEvent(
            evtime,
            new ClientTryToSitEventBody(Client(client_name), table_id)
        );
    }

    static iEvent* parseClientWaitingEvent(std::istream& iss, Time24 evtime) {
        std::string client_name;
        iss >> client_name;
        return new ClientWaitingEvent(
            evtime,
            new ClientWaitingEventBody(Client(client_name))
        );
    }

    static iEvent* parseClientLeftEvent4(std::istream& iss, Time24 evtime) {
        std::string client_name;
        iss >> client_name;
        return new ClientLeftEvent4(
            evtime,
            new ClientLeftEvent4Body(Client(client_name))
        );
    }
};