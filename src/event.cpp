#include "event.hpp"

namespace ccms
{

iEvent::iEvent(Time24 time) : time(time) {}
iEvent::~iEvent() = default;

// INCOMING EVENTS
ClientArrivedEvent::ClientArrivedEvent(Time24 time, std::shared_ptr<ClientArrivedEventBody> body)
    : iEvent(time)
    , body(body)
{}

EventInfo ClientArrivedEvent::info() const {
    std::ostringstream oss;
    oss << time << " " << _id << " " << body->client;
    return oss.str();
}


ClientTryToSitEvent::ClientTryToSitEvent(Time24 time, std::shared_ptr<ClientTryToSitEventBody> body)
    : iEvent(time)
    , body(body)
{}

EventInfo ClientTryToSitEvent::info() const {
    std::ostringstream oss;
    oss << time << " " << _id << " " << body->client << " " << body->table_id;
    return oss.str();
}


ClientWaitingEvent::ClientWaitingEvent(Time24 time, std::shared_ptr<ClientWaitingEventBody> body)
    : iEvent(time)
    , body(body)
{}

EventInfo ClientWaitingEvent::info() const {
    std::ostringstream oss;
    oss << time << " " << _id << " " << body->client;
    return oss.str();
}


ClientDepartedEvent::ClientDepartedEvent(Time24 time, std::shared_ptr<ClientDepartedEventBody> body)
    : iEvent(time)
    , body(body)
{}

EventInfo ClientDepartedEvent::info() const {
    std::ostringstream oss;
    oss << time << " " << _id << " " << body->client;
    return oss.str();
}


// OUTGOING EVENTS
ClientLeftEvent::ClientLeftEvent(Time24 time, std::shared_ptr<ClientLeftEventBody> body)
    : iEvent(time)
    , body(body)
{}
    
EventInfo ClientLeftEvent::info() const {
    std::ostringstream oss;
    oss << time << " " << _id << " " << body->client;
    return oss.str();
}


SeatClientEvent::SeatClientEvent(Time24 time, std::shared_ptr<SeatClientEventBody> body)
    : iEvent(time)
    , body(body)
{}
    
EventInfo SeatClientEvent::info() const {
    std::ostringstream oss;
    oss << time << " " << _id << " " << body->client << " " << body->table_id;
    return oss.str();
}


ErrorEvent::ErrorEvent(Time24 time, std::shared_ptr<ErrorEventBody> body)
    : iEvent(time)
    , body(body)
{}
    
EventInfo ErrorEvent::info() const {
    std::ostringstream oss;
    oss << time << " " << _id << " " << body->message;
    return oss.str();
}


WorkDayEndEvent::WorkDayEndEvent(Time24 time, std::shared_ptr<WorkDayEndEventBody> body)
    : iEvent(time)
    , body(body)
{}
    
EventInfo WorkDayEndEvent::info() const {
    return EventInfo();
}

} // namespace ccms