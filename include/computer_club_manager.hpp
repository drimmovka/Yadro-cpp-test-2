#pragma once

#include <vector>
#include <typeindex>
#include <functional>
#include <stdexcept>

#include "computer_club.hpp"
#include "event.hpp"
#include "exception.hpp"

namespace ccms
{

class ComputerClubManager {

// This class is actually an event dispatcher, and some events may generate new events,
// since it is assumed that a parent event can generate only one descendant event, 
// then the handler of the generated event is called right inside the handler of the parent event, 
// otherwise you can add a queue of events and process events inside the main handler until the queue is empty.
public:
    using Handler = std::function<void(const std::shared_ptr<iEvent>&)>;

    explicit ComputerClubManager(ComputerClub& computer_club);

    ~ComputerClubManager();

    void reset();

    void handle(const std::shared_ptr<iEvent>& ev);

    const std::vector<EventInfo>& handledEventsInfo() const;

private:
    ComputerClub& _computer_club;
    std::unordered_map<std::type_index, Handler> _handlers;
    std::vector<EventInfo> _handled_events_info;
    std::optional<Time24> _last_handled_event_time;

private:
    template<typename TEvent>
    void registerHandler(void (ComputerClubManager::*handler)(const std::shared_ptr<TEvent>&)) {
        _handlers[typeid(TEvent)] = [this, handler](const std::shared_ptr<iEvent>& ev) {
            (this->*handler)( std::dynamic_pointer_cast<TEvent>(ev) );
        };
    }

    void handleClientArrivedEvent(const std::shared_ptr<ClientArrivedEvent>& ev);

    void handleClientTryToSitEvent(const std::shared_ptr<ClientTryToSitEvent>& ev);

    void handleClientWaitingEvent(const std::shared_ptr<ClientWaitingEvent>& ev);

    void handleClientDepartedEvent(const std::shared_ptr<ClientDepartedEvent>& ev);

    void handleClientLeftEvent(const std::shared_ptr<ClientLeftEvent>& ev);

    void handleSeatClientEvent(const std::shared_ptr<SeatClientEvent>& ev);

    void handleErrorEvent(const std::shared_ptr<ErrorEvent>& ev);

    void handleWorkDayEndEvent(const std::shared_ptr<WorkDayEndEvent>& ev);
};

} // namespace ccms