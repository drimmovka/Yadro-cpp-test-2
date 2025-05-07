#pragma once

#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <optional>
#include <exception>

#include "table.hpp"
#include "client.hpp"
#include "exception.hpp"

namespace ccms
{

class ComputerClub {
// The invariant is that:
// if a customer is in a club, 
// then he is either in the waiting queue 
// or
// at the table
// (He can't be both there and there at the same time)
public:

    explicit ComputerClub(int tables_number, const Time24& start_time, const Time24& end_time, int hourly_rate);
    
    ~ComputerClub();

    void reset();

    void acceptClient(const Time24& time, const Client& client);

    void seatClient(const Time24& time, const Client& client, id_t table_id);

    void clientLeft(const Time24& time, const Client& client, std::optional<id_t>& left_table_id);

    Time24 startTime() const;

    Time24 endTime() const;

    bool hasAvailableTables();

    int waitingQueueSize();

    int tablesNumber();

    const Client& waitingQueueFront() const;

    std::vector<Client> sortedListOfClients() const;

    // sorts tables info by id
    std::vector<TableInfo> tablesInfo() const;

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

    void validateInput(int tables_number, const Time24& start_time, const Time24& end_time, int hourly_rate);

};

} // namespace ccms