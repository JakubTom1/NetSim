#include "reports.hpp"
#include <iostream>

void generate_structure_report(const Factory& f, std::ostream& os){
    os<<"\n== LOADING RAMPS ==\n\n";
    std::set<ElementID> workers;
    std::set<ElementID> storehouses;
    for(auto it = f.ramp_cbegin(); it != f.ramp_cend(); it++){
        os << "LOADING RAMP #" << std::to_string(it->get_id()) <<"\n";
        os << "  Delivery interval: " << std::to_string(it->get_delivery_interval()) << "\n";
        os << "  Receivers:\n";


        for(auto receiver = it->receiver_preferences_.cbegin(); receiver != it->receiver_preferences_.cend(); receiver++){
            if(receiver->first->get_receiver_type() == ReceiverType::WORKER){
                workers.insert(receiver->first->get_id());
            }
            else if(receiver->first->get_receiver_type() == ReceiverType::STOREHOUSE){
                storehouses.insert(receiver->first->get_id());
            }
        }
        for(auto worke : workers){
            os << "    worker #" << std::to_string(worke) << "\n";
        }
        for(auto store : storehouses){
            os<<"    storehouse #"<<std::to_string(store)<<"\n";
        }
        os<<"\n";
    }
    workers.clear();
    storehouses.clear();



    os << "\n== WORKERS ==\n\n";
    std::string queue_type;
    for(auto it = f.worker_cbegin(); it != f.worker_cend(); it++){
        switch (it->get_queue()->get_queue_type()){
            case PackageQueueType::LIFO:{
                queue_type = "LIFO";
                break;
            }
            case PackageQueueType::FIFO:{
                queue_type = "FIFO";
                break;
            }
            default:
                break;
        }
        os << "WORKER #" << std::to_string(it->get_id()) << "\n";
        os << "  Processing time: " << std::to_string(it->get_processing_duration()) << "\n";
        os << "  Queue type: " << queue_type << "\n";
        os << "  Receivers:\n";
        for(auto receiver = it->receiver_preferences_.cbegin(); receiver != it->receiver_preferences_.cend(); receiver++){
            if(receiver->first->get_receiver_type() == ReceiverType::WORKER){
                workers.insert(receiver->first->get_id());
            }
            else if(receiver->first->get_receiver_type() == ReceiverType::STOREHOUSE){
                storehouses.insert(receiver->first->get_id());
            }
        }


        for(auto worke : workers){
            os << "    worker #" << std::to_string(worke) << "\n";
        }
        for(auto store : storehouses){
            os << "    storehouse #" << std::to_string(store) << "\n";
        }
        os<< "\n";
    }


    os << "\n== STOREHOUSES ==\n\n";
    for(auto it = f.storehouse_cbegin(); it != f.storehouse_cend(); it++){
        os << "STOREHOUSE #" << std::to_string(it->get_id()) << "\n\n";
    }
}

void generate_simulation_turn_report(const Factory& f, std::ostream& os, Time t){
    os << "=== [ Turn: " << std::to_string(t) << " ] ===\n\n";
    os << "== WORKERS ==\n";
    if(f.worker_cend()!=f.worker_cbegin()){
        os << "\n";
    }
    for(auto it = f.worker_cbegin(); it != f.worker_cend(); it++){
        auto workerr = f.find_worker_by_id(it->get_id());
        os << "WORKER #" << std::to_string(it->get_id()) << "\n";



        if(workerr->get_current_processing_buffer()){
            os << "  PBuffer: #" << workerr->get_current_processing_buffer()->get_id();
            os << " (pt = " << std::to_string(t - workerr->get_package_processing_stat_time() + 1) << ")\n";
        }
        else{
            os << "  PBuffer: (empty)\n";
        }



        if(workerr->cbegin() == workerr->cend()){
            os << "  Queue: (empty)\n";
        }
        else{
            os << "  Queue:";
            for(auto i = workerr->get_queue()->cbegin(); i != workerr->get_queue()->cend() ; i++){
                if(i == workerr->get_queue()->cbegin()){
                    os << " #" << i->get_id();
                }
                else{
                    os << ", #" << i->get_id();
                }
            }
            os << "\n";

        }



        if(workerr->get_processing_buffer()){
            os << "  SBuffer: #" << workerr->get_processing_buffer()->get_id() << "\n";
        }
        else{
            os << "  SBuffer: (empty)\n";
        }
    }



    os<<"\n\n== STOREHOUSES ==\n\n";
    for(auto it = f.storehouse_cbegin(); it != f.storehouse_cend(); it++){
        auto store = f.find_storehouse_by_id(it->get_id());
        os << "STOREHOUSE #" << std::to_string(it->get_id()) << "\n";


        if(store->cbegin()==store->cend()){
            os<<"  Stock: (empty)\n\n";
        }
        else{
            os<<"  Stock:";
            for(auto i = store->cbegin(); i != store->cend() ; i++){
                if (i == store->cbegin()){
                    os << " #" << i->get_id();
                }
                else {
                    os << ", #" << i->get_id();
                }
            }
            os<<"\n\n";
        }
    }

}