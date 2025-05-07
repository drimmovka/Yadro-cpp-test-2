#include "computer_club_manager.hpp"

namespace ccms
{

ComputerClubManager::ComputerClubManager(ComputerClub& computer_club) 
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

ComputerClubManager::~ComputerClubManager() = default;

void ComputerClubManager::reset() {
    _computer_club.reset();
    _handled_events_info.clear();
    _last_handled_event_time.reset();
}

void ComputerClubManager::handle(const std::shared_ptr<iEvent>& ev) {
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

const std::vector<EventInfo>& ComputerClubManager::handledEventsInfo() const {
    return _handled_events_info;
}


void ComputerClubManager::handleClientArrivedEvent(const std::shared_ptr<ClientArrivedEvent>& ev) {
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

void ComputerClubManager::handleClientTryToSitEvent(const std::shared_ptr<ClientTryToSitEvent>& ev) {
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

void ComputerClubManager::handleClientWaitingEvent(const std::shared_ptr<ClientWaitingEvent>& ev) {
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

void ComputerClubManager::handleClientDeparted(const std::shared_ptr<ClientDeparted>& ev) {
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

void ComputerClubManager::handleClientLeftEvent(const std::shared_ptr<ClientLeftEvent>& ev) {
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

void ComputerClubManager::handleSeatClientEvent(const std::shared_ptr<SeatClientEvent>& ev) {
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

void ComputerClubManager::handleErrorEvent(const std::shared_ptr<ErrorEvent>& ev) {
    _handled_events_info.emplace_back(ev->info());
}

void ComputerClubManager::handleWorkDayEndEvent(const std::shared_ptr<WorkDayEndEvent>& ev) {
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

} // namespace ccms