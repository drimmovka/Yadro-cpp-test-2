#pragma once

#include "client.hpp"

// most likely, separate classes for storing data are not needed to solve the task, 
// but for the flexibility of the code and the correct representation of the event (time, body), it was left

class iEventBody {
public:
    virtual ~iEventBody();
};

class ClientArrivedEventBody : public iEventBody {
public:
    explicit ClientArrivedEventBody(const Client& client);

    // TODO: ideally, getters and setters are needed, but for now encapsulation is broken
    Client client;
};

class ClientTryToSitEventBody : public iEventBody {
public:
    explicit ClientTryToSitEventBody(const Client& client, id_t table_id);

    // TODO: ideally, getters and setters are needed, but for now encapsulation is broken
    Client client;
    id_t table_id; 
};

class ClientWaitingEventBody : public iEventBody {
public:
    explicit ClientWaitingEventBody(const Client& client);

    // TODO: ideally, getters and setters are needed, but for now encapsulation is broken
    Client client;
};

class ClientDepartedBody : public iEventBody {
public:
    explicit ClientDepartedBody(const Client& client);

    // TODO: ideally, getters and setters are needed, but for now encapsulation is broken
    Client client;
};

class ClientLeftEventBody : public iEventBody {
public:
    explicit ClientLeftEventBody(const Client& client);

    // TODO: ideally, getters and setters are needed, but for now encapsulation is broken
    Client client;
};



class SeatClientEventBody : public iEventBody {
public:
    explicit SeatClientEventBody(const Client& client, id_t table_id);

    // TODO: ideally, getters and setters are needed, but for now encapsulation is broken
    Client client;
    id_t table_id;
};

class ErrorEventBody : public iEventBody {
public:
    explicit ErrorEventBody(const std::string& message);

    // TODO: ideally, getters and setters are needed, but for now encapsulation is broken
    const std::string message;
};

class WorkDayEndEventBody : public iEventBody {
public:
    WorkDayEndEventBody();
};