#ifndef NETSIM_SIMULATION_HPP
#define NETSIM_SIMULATION_HPP

#include "factory.hpp"

void simulate(Factory& f, TimeOffset d, std::function<void(Factory&, Time)> rf);

class SpecificTurnsReportNotifier{
public:
    SpecificTurnsReportNotifier(std::set<Time> turns) : turns_(std::move(turns)) {};
    bool should_generate_report(Time t){return turns_.count(t);};

private:
    std::set<Time> turns_;
};

class IntervalReportNotifier{
public:
    IntervalReportNotifier(TimeOffset to) : to_(to) {};
    bool should_generate_report(Time t){return t % to_ == 1;}

private:
    TimeOffset to_;
};

#endif
