#include "event_body.hpp"

namespace ccms
{

iEventBody::~iEventBody() = default;


ClientArrivedEventBody::ClientArrivedEventBody(const Client& client)
    : client(client)
{}

ClientTryToSitEventBody::ClientTryToSitEventBody(const Client& client, id_t table_id)
    : client(client)
    , table_id(table_id)
{}

ClientWaitingEventBody::ClientWaitingEventBody(const Client& client)
    : client(client)
{}

ClientDepartedEventBody::ClientDepartedEventBody(const Client& client)
    : client(client)
{}

ClientLeftEventBody::ClientLeftEventBody(const Client& client)
    : client(client)
{}

SeatClientEventBody::SeatClientEventBody(const Client& client, id_t table_id)
    : client(client)
    , table_id(table_id)
{}

ErrorEventBody::ErrorEventBody(const std::string& message)
    : message(message)
{}

WorkDayEndEventBody::WorkDayEndEventBody() {}

} // namespace ccms