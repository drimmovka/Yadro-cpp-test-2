#pragma once

#include <vector>
#include <typeindex>
#include <functional>
#include <stdexcept>
#include "computer_club.hpp"
#include "event.hpp"

class ComputerClubManagerException : public std::exception {
public:
    explicit ComputerClubManagerException(const char* message) : _message(message) {}
    const char* what() const noexcept override { return _message; }
private:
    const char* _message;
};

class ComputerClubManager {
public:
    using Handler = std::function<void(iEvent*)>;

    explicit ComputerClubManager(ComputerClub& computer_club) 
        : _computer_club(computer_club) 
    {
        registerHandler<ClientArrivedEvent>(&ComputerClubManager::handleClientArrivedEvent);
        registerHandler<ClientTryToSitEvent>(&ComputerClubManager::handleClientTryToSitEvent);
        registerHandler<ClientWaitingEvent>(&ComputerClubManager::handleClientWaitingEvent);
        registerHandler<ClientLeftEvent4>(&ComputerClubManager::handleClientLeftEvent4);
        registerHandler<ClientLeftEvent11>(&ComputerClubManager::handleClientLeftEvent11);
        registerHandler<SeatClientEvent>(&ComputerClubManager::handleSeatClientEvent);
        registerHandler<ErrorEvent>(&ComputerClubManager::handleErrorEvent);
        registerHandler<WorkDayEndEvent>(&ComputerClubManager::handleWorkDayEndEvent);
    }

    ~ComputerClubManager() = default;

    void handle(iEvent* ev) {
        auto it = _handlers.find(typeid(*ev));
        if (it == _handlers.end()) {
            throw ComputerClubManagerException("Event handler is missing");
        }
        
        // Сохраняем событие для последующего удаления
        it->second(ev);
    }

    const std::vector<EventInfo>& handledEventsInfo() const {
        return _handled_events_info;
    }

private:
    ComputerClub& _computer_club;
    std::unordered_map<std::type_index, Handler> _handlers;
    std::vector<EventInfo> _handled_events_info;

    template<typename TEvent>
    void registerHandler(void (ComputerClubManager::*handler)(TEvent*)) {
        _handlers[typeid(TEvent)] = [this, handler](iEvent* ev) {
            (this->*handler)(static_cast<TEvent*>(ev));
        };
    }

    void handleClientArrivedEvent(ClientArrivedEvent* ev) {
        _handled_events_info.emplace_back(ev->info());
        try {
            _computer_club.acceptClient(ev->time, ev->body->client);
        } 
        catch (NotOpenYetException&) {
            handle(new ErrorEvent(ev->time, new ErrorEventBody("NotOpenYet")));
        } 
        catch (YouShallNotPassException&) {
            handle(new ErrorEvent(ev->time, new ErrorEventBody("YouShallNotPass")));
        }
    }

    void handleClientTryToSitEvent(ClientTryToSitEvent* ev) {
        _handled_events_info.emplace_back(ev->info());
        try {
            _computer_club.seatClient(ev->body->client, ev->body->table_id);
        } 
        catch (ClientUnknownException&) {
            handle(new ErrorEvent(ev->time, new ErrorEventBody("ClientUnknown")));
        } 
        catch (PlaceIsBusyException&) {
            handle(new ErrorEvent(ev->time, new ErrorEventBody("PlaceIsBusy")));
        }
    }

    void handleClientWaitingEvent(ClientWaitingEvent* ev) {
        _handled_events_info.emplace_back(ev->info());
        if (_computer_club.hasAvailableTables()) {
            handle(new ErrorEvent(ev->time, new ErrorEventBody("ICanWaitNoLonger!")));
        } 
        else if (_computer_club.waitingQueueSize() > _computer_club.tablesNumber()) {
            handle(new ClientLeftEvent11(ev->time, new ClientLeftEvent11Body(ev->body->client)));
        }
    }

    void handleClientLeftEvent4(ClientLeftEvent4* ev) {
        _handled_events_info.emplace_back(ev->info());
        try {
            std::optional<id_t> left_table_id;
            _computer_club.clientLeft(ev->body->client, left_table_id);
            
            if (left_table_id.has_value() && _computer_club.waitingQueueSize() > 0) {
                handle(new SeatClientEvent(
                    ev->time, 
                    new SeatClientEventBody(
                        _computer_club.waitingQueueFront(), 
                        left_table_id.value()
                    )
                ));
            }
        } 
        catch (ClientUnknownException&) {
            handle(new ErrorEvent(ev->time, new ErrorEventBody("ClientUnknown")));
        }
    }

    void handleClientLeftEvent11(ClientLeftEvent11* ev) {
        _handled_events_info.emplace_back(ev->info());
        try {
            std::optional<id_t> left_table_id;
            _computer_club.clientLeft(ev->body->client, left_table_id);
        } 
        catch (ClientUnknownException&) {
            handle(new ErrorEvent(ev->time, new ErrorEventBody("ClientUnknown")));
        }
    }

    void handleSeatClientEvent(SeatClientEvent* ev) {
        _handled_events_info.emplace_back(ev->info());
        try {
            _computer_club.seatClient(ev->body->client, ev->body->table_id);
        } 
        catch (ClientUnknownException&) {
            handle(new ErrorEvent(ev->time, new ErrorEventBody("ClientUnknown")));
        } 
        catch (PlaceIsBusyException&) {
            handle(new ErrorEvent(ev->time, new ErrorEventBody("PlaceIsBusy")));
        }
    }

    void handleErrorEvent(ErrorEvent* ev) {
        _handled_events_info.emplace_back(ev->info());
    }

    void handleWorkDayEndEvent(WorkDayEndEvent* ev) {
        auto sorted_clients = _computer_club.sortedListOfClients();
        for (auto& client : sorted_clients) {
            try {
                handle(new ClientLeftEvent11(ev->time, new ClientLeftEvent11Body(client)));
            } 
            catch (ClientUnknownException&) {
                handle(new ErrorEvent(ev->time, new ErrorEventBody("ClientUnknown")));
            }
        }
    }
};