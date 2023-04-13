#include "nodes.hpp"
#include <iostream>

void ReceiverPreferences::add_receiver(IPackageReceiver *r) {
    preferences_.emplace(r, 1);
    for(auto elem_ptr = preferences_.begin(); elem_ptr != preferences_.end(); elem_ptr++)
    {
        elem_ptr->second = 1 / (double)(preferences_.size());
    }

}

void ReceiverPreferences::remove_receiver (IPackageReceiver* r) {
    preferences_.erase(r);
    if (!preferences_.empty()) {
        for(auto elem_ptr = preferences_.begin(); elem_ptr != preferences_.end(); elem_ptr++)
        {
            elem_ptr->second = 1 / (double)(preferences_.size());
        }
    }
}

IPackageReceiver *ReceiverPreferences::choose_receiver() {
    double dist = pg_(); //canonical?
    double probability = 0;
    for(auto elem_ptr = preferences_.begin(); elem_ptr != preferences_.end(); elem_ptr++)
    {
        probability += elem_ptr->second;
        if (probability >= dist) {
            return elem_ptr->first;

        }
    }
    throw std::logic_error("No choosen");
}

void Worker::do_work(Time t) {
    // Jeżeli bufor jest pusty, a w liscie produktow robotnika cos jest to przekazujemy mu to
    if(!current_processing_buffer_ and !q_->empty()){
        package_processing_start_time_ = t;
        current_processing_buffer_ = q_->pop();
    }


    //sprawdzenie czy polprodukt zostal przetworzony ???
    if(t - package_processing_start_time_+1>= pd_ and current_processing_buffer_){
        package_processing_start_time_ = 0;
        push_package(std::move(current_processing_buffer_.value()));
        current_processing_buffer_.reset();

    }


}

void PackageSender::push_package(Package && pkg) {
    if(!buffer_){
        buffer_.emplace(std::move(pkg));
    }
}

void PackageSender::send_package() {
    if(buffer_){
        receiver_preferences_.choose_receiver()->receive_package(std::move(buffer_.value()));
        buffer_.reset();
    }
}



void Ramp::deliver_goods(Time t) {
    if(sim_start_time_ == 2147483647){
        sim_start_time_ = t;
    }
    if((t - sim_start_time_) % di_ == 0){
        push_package(std::move(Package()));
    }
}