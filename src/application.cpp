#include "application.hpp"


namespace ccms
{

Application::Application(std::istream& is, std::ostream& os)
    : _input_parser(is)
    , _output_writer(os)
{}

Application::~Application() = default;

void Application::run() {
    int tables_number;
    Time24 start_time, end_time;
    int hourly_rate;        
    try {
        tables_number = _input_parser.inputTablesNumber();
        std::tie(start_time, end_time) = _input_parser.inputStartEndTime();
        hourly_rate = _input_parser.inputHourlyRate();
    }
    catch (InputParserException& e) {
        _output_writer.outputMessage(std::string(e.what()) + "\nline (" + std::to_string(_input_parser.lastReadLineNumber()) + "): " + _input_parser.lastReadLine()); return;
    }

    std::unique_ptr<ComputerClub> computer_club;
    try {
        computer_club = std::make_unique<ComputerClub>(tables_number, start_time, end_time, hourly_rate);
    }
    catch (ComputerClubException& e) {
        _output_writer.outputMessage(e.what()); return;
    }

    std::unique_ptr<ComputerClubManager> computer_club_manager;        
    try {
        computer_club_manager = std::make_unique<ComputerClubManager>(*computer_club);
    }
    catch (ComputerClubManagerException& e) {
        _output_writer.outputMessage(e.what()); return;
    }

    std::vector<std::shared_ptr<iEvent>> events;
    try {
        events = _input_parser.inputEvents();
    }
    catch (InputParserException& e) {
        _output_writer.outputMessage(std::string(e.what()) + "\nline (" + std::to_string(_input_parser.lastReadLineNumber()) + "): " + _input_parser.lastReadLine()); return;
    }

    try {
        for (auto& event : events) {
            computer_club_manager->handle(event);
        }
        computer_club_manager->handle( std::make_shared<WorkDayEndEvent>(end_time, std::make_shared<WorkDayEndEventBody>()) );
    }
    catch (std::exception& e) {
        _output_writer.outputMessage(e.what()); return;
    }


    _output_writer.outputLog(*computer_club, *computer_club_manager);
}

} // namespace ccms