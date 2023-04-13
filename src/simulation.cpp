#include "simulation.hpp"

void simulate(Factory& f, TimeOffset d, std::function<void(Factory&, Time)> rf){
    if (!f.is_consistent()){
        throw std::logic_error("Niespojna siec!!!");
    }

    for(TimeOffset time = 1; time <= d; ++time){
        f.do_deliveries(time);
        f.do_package_possing();
        f.do_work(time);
        rf(f,time);
    }
}