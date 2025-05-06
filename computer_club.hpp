#pragma once

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <optional>
#include <exception>

#include <cassert>

#include "table.hpp"
#include "client.hpp"

class ComputerClubException : public std::exception {
public:
    explicit ComputerClubException(const char *message)
        : _message(message)
    {}

    const char *what() const noexcept override {
        return _message;
    }
    
protected:
    const char *_message;
};


class NotOpenYetException : public ComputerClubException {
public:
    explicit NotOpenYetException(const char *message)
        : ComputerClubException(message)
    {}
};

class YouShallNotPassException : public ComputerClubException {
public:
    explicit YouShallNotPassException(const char *message)
        : ComputerClubException(message)
    {}
};

class ClientUnknownException : public ComputerClubException {
public:
    explicit ClientUnknownException(const char *message)
        : ComputerClubException(message)
    {}
};

class PlaceIsBusyException : public ComputerClubException {
public:
    explicit PlaceIsBusyException(const char *message)
        : ComputerClubException(message)
    {}
};



class ComputerClub {
public:

    ~ComputerClub() = default;    

    explicit ComputerClub(int tables_number, const Time24& start_time, const Time24& end_time, int hourly_rate) {
        validateInput(tables_number, start_time, end_time, hourly_rate);
        _start_time = start_time;
        _end_time = end_time;
        _hourly_rate = hourly_rate;

        // the IDs are set in the order from 1 to tables_number
        for (id_t id = 1; id <= tables_number; ++id) {
            _tables.emplace(id, Table());
        }
    }
    
    void acceptClient(Time24 arrival_time, const Client& client) {
        if (arrival_time < _start_time || arrival_time > _end_time) {
            throw NotOpenYetException("");
        }
        if (_clients.find(client) != _clients.end()) {
            throw YouShallNotPassException("");
        }
        
        _clients.emplace(client, std::nullopt);
        _waiting_queue.emplace_back(client);
    }

    void seatClient(const Client& client, id_t table_id) {
        auto clients_it = _clients.find(client);

        if (clients_it == _clients.end()) {
            throw ClientUnknownException("");
        }
        if (_tables.find(table_id) == _tables.end()) {
            throw ComputerClubException("No table with this id");
        }
        if (_tables[table_id].isOccupied()) {
            throw PlaceIsBusyException("");
        }
        

        auto waiting_queue_it = std::find(_waiting_queue.begin(), _waiting_queue.end(), client);

        // It is taken into account that the client may not be among those waiting
        if (waiting_queue_it == _waiting_queue.end()) {
            id_t occupied_table_id = clients_it->second.value();
            // the client gets up from his table
            _tables[clients_it->second.value()].setOccupiedStatus(false);
        } else {
            // the client leaves the waiting queue
            _waiting_queue.erase(waiting_queue_it);
        }

        // the client sits down at the table and the table becomes occupied
        clients_it->second = table_id;
        _tables[table_id].setOccupiedStatus(true);
    }

    void clientLeft(const Client& client, std::optional<id_t>& left_table_id) {
        auto clients_it = _clients.find(client);

        if (clients_it == _clients.end()) {
            throw ClientUnknownException("");
        }

        auto waiting_queue_it = std::find(_waiting_queue.begin(), _waiting_queue.end(), client);
        
        // It is taken into account that the client may not be among those waiting
        if (waiting_queue_it == _waiting_queue.end()) {
            id_t occupied_table_id = clients_it->second.value();
            left_table_id = occupied_table_id;
            // the client gets up from his table
            _tables[occupied_table_id].setOccupiedStatus(false);
        } else {
            // the client leaves the waiting queue
            _waiting_queue.erase(waiting_queue_it);  
        }

        _clients.erase(clients_it);
    }

    bool hasAvailableTables() {
        for (auto& [id, table] : _tables) {
            if (!table.isOccupied()) return true;
        }
        return false;
    }

    int waitingQueueSize() {
        return _waiting_queue.size();
    }

    int tablesNumber() {
        return _tables.size();
    }

    Client& waitingQueueFront() {
        if (_waiting_queue.empty()) {
            throw ComputerClubException("Waiting queue is empty");
        }
        return _waiting_queue.front();
    }

    std::vector<Client> sortedListOfClients() {
        std::vector<Client> list_of_clients;
        for (auto& [client, _] : _clients) {
            list_of_clients.emplace_back(client);
        }
        std::sort(list_of_clients.begin(), list_of_clients.end());
        return list_of_clients;
    } 

private:
    Time24 _start_time;
    Time24 _end_time;
    int _hourly_rate;
    std::unordered_map<id_t, Table> _tables;

    // ideally, there should be a Client_id rather than the Client itself, but in the context of the task,
    // the client is perceived as a reverse over a string and its name is actually an id
    // + it would be necessary to create a separate class to store the corresponding [client -> client_id]
    std::unordered_map<Client, std::optional<id_t>, Client::ClientHash> _clients;
    std::deque<Client> _waiting_queue;

private:

    void validateInput(int tables_number, const Time24& start_time, const Time24& end_time, int hourly_rate) {
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

};