#pragma once

#include <vector>
#include <typeindex>
#include <functional>
#include <stdexcept>
#include "computer_club.hpp"
#include "event.hpp"

class ComputerClubManagerException : public std::exception {
public:
    explicit ComputerClubManagerException(const std::string& message)
        : _message(message)
    {}
    
    const char* what() const noexcept override { 
        return _message.c_str();
    }

private:
    const std::string _message;
};

class ComputerClubManager {

// This class is actually an event dispatcher, and some events may generate new events,
// since it is assumed that a parent event can generate only one descendant event, 
// then the handler of the generated event is called right inside the handler of the parent event, 
// otherwise you can add a queue of events and process events inside the main handler until the queue is empty.
public:
    using Handler = std::function<void(const std::shared_ptr<iEvent>&)>;

    explicit ComputerClubManager(ComputerClub& computer_club) 
        : _computer_club(computer_club) 
    {
        registerHandler<ClientArrivedEvent>(&ComputerClubManager::handleClientArrivedEvent);
        registerHandler<ClientTryToSitEvent>(&ComputerClubManager::handleClientTryToSitEvent);
        registerHandler<ClientWaitingEvent>(&ComputerClubManager::handleClientWaitingEvent);
        registerHandler<ClientDeparted>(&ComputerClubManager::handleClientDeparted);
        registerHandler<ClientLeftEvent>(&ComputerClubManager::handleClientLeftEvent);
        registerHandler<SeatClientEvent>(&ComputerClubManager::handleSeatClientEvent);
        registerHandler<ErrorEvent>(&ComputerClubManager::handleErrorEvent);
        registerHandler<WorkDayEndEvent>(&ComputerClubManager::handleWorkDayEndEvent);

        reset();
    }

    ~ComputerClubManager() = default;

    void reset() {
        _computer_club.reset();
        _handled_events_info.clear();
        _last_handled_event_time.reset();
    }

    void handle(const std::shared_ptr<iEvent>& ev) {
        auto it = _handlers.find(typeid(*ev));
        if (it == _handlers.end()) {
            throw ComputerClubManagerException("Event handler is missing");
        }
        if (_last_handled_event_time.has_value() && ev->time < _last_handled_event_time) {
            throw ComputerClubManagerException("Chronology of events has been disrupted");
        }

        _last_handled_event_time = ev->time;
        it->second(ev);
    }

    const std::vector<EventInfo>& handledEventsInfo() const {
        return _handled_events_info;
    }

private:
    ComputerClub& _computer_club;
    std::unordered_map<std::type_index, Handler> _handlers;
    std::vector<EventInfo> _handled_events_info;
    std::optional<Time24> _last_handled_event_time;

    template<typename TEvent>
    void registerHandler(void (ComputerClubManager::*handler)(const std::shared_ptr<TEvent>&)) {
        _handlers[typeid(TEvent)] = [this, handler](const std::shared_ptr<iEvent>& ev) {
            (this->*handler)( std::dynamic_pointer_cast<TEvent>(ev) );
        };
    }

    void handleClientArrivedEvent(const std::shared_ptr<ClientArrivedEvent>& ev) {
        _handled_events_info.emplace_back(ev->info());
        try {
            _computer_club.acceptClient(ev->time, ev->body->client);
        } 
        catch (ComputerClubException& e) {
            handle(
                std::make_unique<ErrorEvent>(
                    ev->time,
                    std::make_unique<ErrorEventBody>(e.what())
                )
            );
        }
    }

    void handleClientTryToSitEvent(const std::shared_ptr<ClientTryToSitEvent>& ev) {
        _handled_events_info.emplace_back(ev->info());
        try {
            _computer_club.seatClient(ev->time, ev->body->client, ev->body->table_id);
        }
        catch (ComputerClubException& e) {
            handle(
                std::make_unique<ErrorEvent>(
                    ev->time,
                    std::make_unique<ErrorEventBody>(e.what())
                )
            );
        }
    }

    void handleClientWaitingEvent(const std::shared_ptr<ClientWaitingEvent>& ev) {
        _handled_events_info.emplace_back(ev->info());
        if (_computer_club.hasAvailableTables()) {
            handle(
                std::make_unique<ErrorEvent>(
                    ev->time,
                    std::make_unique<ErrorEventBody>("ICanWaitNoLonger!")
                )
            );
        } 
        else if (_computer_club.waitingQueueSize() > _computer_club.tablesNumber()) {
            handle(
                std::make_unique<ClientLeftEvent>(
                    ev->time,
                    std::make_unique<ClientLeftEventBody>(ev->body->client)
                )
            );
        }
    }

    void handleClientDeparted(const std::shared_ptr<ClientDeparted>& ev) {
        _handled_events_info.emplace_back(ev->info());
        try {
            std::optional<id_t> left_table_id;
            _computer_club.clientLeft(ev->time, ev->body->client, left_table_id);
            
            if (left_table_id.has_value() && _computer_club.waitingQueueSize() > 0) {
                handle(
                    std::make_unique<SeatClientEvent>(
                        ev->time, 
                        std::make_unique<SeatClientEventBody>(
                            _computer_club.waitingQueueFront(), 
                            left_table_id.value()
                        )
                    )
                );
            }
        } 
        catch (ComputerClubException& e) {
            handle(
                std::make_unique<ErrorEvent>(
                    ev->time,
                    std::make_unique<ErrorEventBody>(e.what())
                )
            );
        }
    }

    void handleClientLeftEvent(const std::shared_ptr<ClientLeftEvent>& ev) {
        _handled_events_info.emplace_back(ev->info());
        try {
            std::optional<id_t> left_table_id;
            _computer_club.clientLeft(ev->time, ev->body->client, left_table_id);
        } 
        catch (ComputerClubException& e) {
            handle(
                std::make_unique<ErrorEvent>(
                    ev->time,
                    std::make_unique<ErrorEventBody>(e.what())
                )
            );
        }   
    }

    void handleSeatClientEvent(const std::shared_ptr<SeatClientEvent>& ev) {
        _handled_events_info.emplace_back(ev->info());
        try {
            _computer_club.seatClient(ev->time, ev->body->client, ev->body->table_id);
        } 
        catch (ComputerClubException& e) {
            handle(
                std::make_unique<ErrorEvent>(
                    ev->time,
                    std::make_unique<ErrorEventBody>(e.what())
                )
            );
        }
    }

    void handleErrorEvent(const std::shared_ptr<ErrorEvent>& ev) {
        _handled_events_info.emplace_back(ev->info());
    }

    void handleWorkDayEndEvent(const std::shared_ptr<WorkDayEndEvent>& ev) {
        auto sorted_clients = _computer_club.sortedListOfClients();
        for (auto& client : sorted_clients) {
            try {
                handle(
                    std::make_unique<ClientLeftEvent>(
                        ev->time,
                        std::make_unique<ClientLeftEventBody>(client)
                    )
                );
            } 
            catch (ComputerClubException& e) {
                handle(
                    std::make_unique<ErrorEvent>(
                        ev->time,
                        std::make_unique<ErrorEventBody>(e.what())
                    )
                );
            }
        }
    }
};