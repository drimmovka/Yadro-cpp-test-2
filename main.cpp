#include <iostream>
#include "computer_club_manager.hpp"
#include "event.hpp"
#include "event_parser.hpp"


int main() {
    // ComputerClub cc(10, Time24("10:00"), Time24("23:00"), 50);
    // ComputerClubManager ccm(cc);
    // ccm.handle(new ClientArrivedEvent(Time24("10:00"), new ClientArrivedEventBody(Client("somename"))));
    // ccm.handle(new ClientArrivedEvent(Time24("11:00"), new ClientArrivedEventBody(Client("somename"))));
    // auto handled_events_info = ccm.handledEventsInfo();
    // for (auto& i : handled_events_info) {
    //     std::cout << i << "\n";
    // }

    int n; std::cin >> n;
    Time24 start, end; std::cin >> start >> end;
    int hourly_rate; std::cin >> hourly_rate;
    ComputerClub cc(n, start, end, hourly_rate);
    
    
    ComputerClubManager ccm(cc);
    int q; std::cin >> q; std::cin.ignore();
    while (q--) {
        std::string line;
        std::getline(std::cin, line);
        iEvent *ev = EventParser::parse(line);
        ccm.handle(ev);
    }

    auto handled_events_info = ccm.handledEventsInfo();
    for (auto& ei : handled_events_info) {
        std::cout << ei << std::endl;
    }
}