#pragma once

#include <string>
#include <memory>

#include "time24.hpp"
#include "event_body.hpp"

namespace ccms
{

using EventInfo = std::string;

class iEvent {
public:
    explicit iEvent(Time24 time);
    virtual ~iEvent();
    virtual EventInfo info() const = 0;
    Time24 time;
};


// INCOMING EVENTS
class ClientArrivedEvent : public iEvent {
public:
    ClientArrivedEvent(Time24 time, std::shared_ptr<ClientArrivedEventBody> body);
    
    EventInfo info() const override;

    // TODO: ideally, getters and setters are needed, but for now encapsulation is broken
    std::shared_ptr<ClientArrivedEventBody> body;

private:
    const id_t _id = 1; 
};


class ClientTryToSitEvent : public iEvent {
public:
    ClientTryToSitEvent(Time24 time, std::shared_ptr<ClientTryToSitEventBody> body);

    EventInfo info() const override;

    // TODO: ideally, getters and setters are needed, but for now encapsulation is broken
    std::shared_ptr<ClientTryToSitEventBody> body;

private:
    const id_t _id = 2; 
};


class ClientWaitingEvent : public iEvent {
public:
    ClientWaitingEvent(Time24 time, std::shared_ptr<ClientWaitingEventBody> body);
    
    EventInfo info() const override;
    
    // TODO: ideally, getters and setters are needed, but for now encapsulation is broken
    std::shared_ptr<ClientWaitingEventBody> body;

private:
    const id_t _id = 3; 
};


class ClientDepartedEvent : public iEvent {
public:
    ClientDepartedEvent(Time24 time, std::shared_ptr<ClientDepartedEventBody> body);

    EventInfo info() const override;

    // TODO: ideally, getters and setters are needed, but for now encapsulation is broken
    std::shared_ptr<ClientDepartedEventBody> body;

private:
    const id_t _id = 4;
};


// OUTGOING EVENTS
class ClientLeftEvent : public iEvent {
public:
    ClientLeftEvent(Time24 time, std::shared_ptr<ClientLeftEventBody> body);
        
    EventInfo info() const override;

    // TODO: ideally, getters and setters are needed, but for now encapsulation is broken
    std::shared_ptr<ClientLeftEventBody> body;

private:
    const id_t _id = 11;
};


class SeatClientEvent : public iEvent {
public:
    SeatClientEvent(Time24 time, std::shared_ptr<SeatClientEventBody> body);
        
    EventInfo info() const override;
    
    // TODO: ideally, getters and setters are needed, but for now encapsulation is broken
    std::shared_ptr<SeatClientEventBody> body;

private:
    const id_t _id = 12;
};


class ErrorEvent : public iEvent {
public:
    ErrorEvent(Time24 time, std::shared_ptr<ErrorEventBody> body);
        
    EventInfo info() const override;

    // TODO: ideally, getters and setters are needed, but for now encapsulation is broken
    std::shared_ptr<ErrorEventBody> body;

private:
    const id_t _id = 13;
};


class WorkDayEndEvent : public iEvent {
public:
    WorkDayEndEvent(Time24 time, std::shared_ptr<WorkDayEndEventBody> body);
        
    EventInfo info() const override;

    // TODO: ideally, getters and setters are needed, but for now encapsulation is broken
    std::shared_ptr<WorkDayEndEventBody> body;

private:
    const id_t _id = 0;
};

} // namespace ccms