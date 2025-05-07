#include "computer_club.hpp"
#include "computer_club_manager.hpp"

class OutputWriter {

public:

    explicit  OutputWriter(std::ostream& os) 
        : _os(os)
    {}

    ~OutputWriter() = default;

    void outputLog(const ComputerClub& computer_club, const ComputerClubManager& computer_club_manager) const {
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

    void outputMessage(const std::string& message) {
        _os << message << std::endl;
    }

private:
    std::ostream& _os;
};