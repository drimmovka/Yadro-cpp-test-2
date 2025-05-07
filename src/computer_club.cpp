#include "computer_club.hpp"

namespace ccms
{

ComputerClub::ComputerClub(int tables_number, const Time24& start_time, const Time24& end_time, int hourly_rate) {
    validateInput(tables_number, start_time, end_time, hourly_rate);
    _start_time = start_time;
    _end_time = end_time;
    _hourly_rate = hourly_rate;

    // the IDs are set in the order from 1 to tables_number
    for (id_t id = 1; id <= tables_number; ++id) {
        _tables.emplace(id, Table(_hourly_rate));
    }
}

ComputerClub::~ComputerClub() = default;

void ComputerClub::reset() {
    for (auto& [id, table] : _tables) {
        table.reset();
    }
    _clients.clear();
    _waiting_queue.clear();
}

void ComputerClub::acceptClient(const Time24& time, const Client& client) {
    if (time < _start_time || time > _end_time) {
        throw ComputerClubException("NotOpenYet");
    }
    if (_clients.find(client) != _clients.end()) {
        throw ComputerClubException("YouShallNotPass");
    }
    
    _clients.emplace(client, std::nullopt);
    _waiting_queue.emplace_back(client);
}

void ComputerClub::seatClient(const Time24& time, const Client& client, id_t table_id) {
    auto clients_it = _clients.find(client);

    if (clients_it == _clients.end()) {
        throw ComputerClubException("ClientUnknown");
    }
    if (_tables.find(table_id) == _tables.end()) {
        throw ComputerClubException("No table with this id");
    }
    if (_tables[table_id].isOccupied()) {
        throw ComputerClubException("PlaceIsBusy");
    }
    

    auto waiting_queue_it = std::find(_waiting_queue.begin(), _waiting_queue.end(), client);

    // It is taken into account that the client may not be among those waiting
    if (waiting_queue_it == _waiting_queue.end()) {
        id_t occupied_table_id = clients_it->second.value();
        // the client gets up from his table
        _tables[clients_it->second.value()].setUnoccupied(time);
    } else {
        // the client leaves the waiting queue
        _waiting_queue.erase(waiting_queue_it);
    }

    // the client sits down at the table and the table becomes occupied
    clients_it->second = table_id;
    _tables[table_id].setOccupied(time);
}

void ComputerClub::clientLeft(const Time24& time, const Client& client, std::optional<id_t>& left_table_id) {
    auto clients_it = _clients.find(client);

    if (clients_it == _clients.end()) {
        throw ComputerClubException("ClientUnknown");
    }

    auto waiting_queue_it = std::find(_waiting_queue.begin(), _waiting_queue.end(), client);
    
    // It is taken into account that the client may not be among those waiting
    if (waiting_queue_it == _waiting_queue.end()) {
        id_t occupied_table_id = clients_it->second.value();
        left_table_id = occupied_table_id;
        // the client gets up from his table
        _tables[occupied_table_id].setUnoccupied(time);
    } else {
        // the client leaves the waiting queue
        _waiting_queue.erase(waiting_queue_it);  
    }

    // the client leaves the club
    _clients.erase(clients_it);
}

Time24 ComputerClub::startTime() const { return _start_time; }

Time24 ComputerClub::endTime() const { return _end_time; }

bool ComputerClub::hasAvailableTables() {
    for (auto& [id, table] : _tables) {
        if (!table.isOccupied()) return true;
    }
    return false;
}

int ComputerClub::waitingQueueSize() {
    return _waiting_queue.size();
}

int ComputerClub::tablesNumber() {
    return _tables.size();
}

const Client& ComputerClub::waitingQueueFront() const {
    if (_waiting_queue.empty()) {
        throw ComputerClubException("Waiting queue is empty");
    }
    return _waiting_queue.front();
}

std::vector<Client> ComputerClub::sortedListOfClients() const {
    std::vector<Client> list_of_clients;
    for (auto& [client, _] : _clients) {
        list_of_clients.emplace_back(client);
    }
    std::sort(list_of_clients.begin(), list_of_clients.end());
    return list_of_clients;
}

// sorts tables info by id
std::vector<TableInfo> ComputerClub::tablesInfo() const {
    std::vector<std::pair<int, TableInfo>> tmp_tables_info;
    for (auto& [id, table] : _tables) {
        tmp_tables_info.emplace_back(id, table.info());
    }
    
    std::sort(tmp_tables_info.begin(), tmp_tables_info.end(),
                [](const auto& a, const auto& b) {
                    return a.first < b.first;
                });
    
    std::vector<TableInfo> tables_info;
    for (auto& [id, table_info] : tmp_tables_info) {
        tables_info.emplace_back(std::to_string(id) + " " + table_info);
    }
    return tables_info;
}

void ComputerClub::validateInput(int tables_number, const Time24& start_time, const Time24& end_time, int hourly_rate) {
    if (tables_number < 0) {
        throw ComputerClubException("Tables number cannot be negative");
    }
    if (start_time > end_time) {
        throw ComputerClubException("Start time cannot be later than end time");
    }
    if (hourly_rate < 0) {
        throw ComputerClubException("Hourly rate cannot be negative");
    }
}

} // namespace ccms