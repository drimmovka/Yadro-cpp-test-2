#include "output_writer.hpp"

OutputWriter::OutputWriter(std::ostream& os) 
    : _os(os)
{}

OutputWriter::~OutputWriter() = default;

void OutputWriter::outputLog(const ComputerClub& computer_club, const ComputerClubManager& computer_club_manager) const {
    _os << computer_club.startTime() << std::endl;

    auto handled_events_info = computer_club_manager.handledEventsInfo();
    for (auto& ei : handled_events_info) {
        _os << ei << std::endl;
    }

    _os << computer_club.endTime() << std::endl;

    auto tables_info = computer_club.tablesInfo();
    for (auto& ti : tables_info) {
        _os << ti << std::endl;
    }
}

void OutputWriter::outputMessage(const std::string& message) {
    _os << message << std::endl;
}