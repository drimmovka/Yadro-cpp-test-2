#pragma once

// most likely, separate classes for storing data are not needed to solve the task, 
// but for the flexibility of the code and the correct representation of the event (time, body), it was left

#include "client.hpp"

class iEventBody {
public:
    virtual ~iEventBody() = default;
};

class ClientArrivedEventBody : public iEventBody {
public:
    ClientArrivedEventBody(Client client)
        : client(client)
    {}

    // TODO: ideally, getters and setters are needed, but for now encapsulation is broken
    Client client;
};

class ClientTryToSitEventBody : public iEventBody {
public:
    ClientTryToSitEventBody(Client client, id_t table_id)
        : client(client)
        , table_id(table_id)
    {}

    // TODO: ideally, getters and setters are needed, but for now encapsulation is broken
    Client client;
    id_t table_id; 
};

class ClientWaitingEventBody : public iEventBody {
public:
    ClientWaitingEventBody(Client client)
        : client(client)
    {}

    // TODO: ideally, getters and setters are needed, but for now encapsulation is broken
    Client client;
};

class ClientLeftEvent4Body : public iEventBody {
public:
    ClientLeftEvent4Body(Client client)
        : client(client)
    {}

    // TODO: ideally, getters and setters are needed, but for now encapsulation is broken
    Client client;
};

class ClientLeftEvent11Body : public iEventBody {
public:
    ClientLeftEvent11Body(Client client)
        : client(client)
    {}

    // TODO: ideally, getters and setters are needed, but for now encapsulation is broken
    Client client;
};



class SeatClientEventBody : public iEventBody {
public:
SeatClientEventBody(Client client, id_t table_id)
        : client(client)
        , table_id(table_id)
    {}

    // TODO: ideally, getters and setters are needed, but for now encapsulation is broken
    Client client;
    id_t table_id;
};

class ErrorEventBody : public iEventBody {
public:
    ErrorEventBody(const char *message)
        : message(message)
    {}

    // TODO: ideally, getters and setters are needed, but for now encapsulation is broken
    const char *message;
};

class WorkDayEndEventBody : public iEventBody {
public:
    WorkDayEndEventBody() {}
};