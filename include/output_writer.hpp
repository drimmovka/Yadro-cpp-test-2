#pragma once

#include "computer_club.hpp"
#include "computer_club_manager.hpp"

class OutputWriter {

public:

    explicit  OutputWriter(std::ostream& os);

    ~OutputWriter();

    void outputLog(const ComputerClub& computer_club, const ComputerClubManager& computer_club_manager) const;

    void outputMessage(const std::string& message);

private:
    std::ostream& _os;
};