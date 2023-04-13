#include "factory.hpp"
#include <istream>
#include <string>
#include <sstream>

bool Factory::is_consistent(){
    std::map<const PackageSender *,NodeColor> colour;
    for (auto ptr = cont_w.begin(); ptr != cont_w.end(); ptr++){
        Worker* worker = &(*ptr);
        colour[worker] = NodeColor::UNVISITED;
    }
    for (auto ptr = cont_r.begin(); ptr != cont_r.end(); ptr++){
        Ramp* ramp = &(*ptr);
        colour[ramp] = NodeColor::UNVISITED;
    }
    try {
        for (auto ptr = cont_r.begin(); ptr != cont_r.end(); ptr++){
            Ramp* ramp = &(*ptr);
            has_reachable_storehouse(ramp, colour);
        }
    }
    catch (const std::logic_error&) {
        return false;
    }
    return true;
}

bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& color) {
    if (color[sender] == NodeColor::VERIFIED) {
        return true;
    }
    color[sender] = NodeColor::VISITED;
    if (sender->receiver_preferences_.get_preferences().empty()) {
        throw std::logic_error("Sender has not any defined receivers");
    }
    bool has_sender_any_other_receiver_than_himself = false;
    for (auto receiver: sender->receiver_preferences_.get_preferences()) {
        if (receiver.first->get_receiver_type() == ReceiverType::STOREHOUSE) {
            has_sender_any_other_receiver_than_himself = true;
        } else if (receiver.first->get_receiver_type() == ReceiverType::WORKER) {
            IPackageReceiver *receiver_ptr = receiver.first;
            auto worker_ptr = dynamic_cast<Worker *>(receiver_ptr);
            auto sendrecv_ptr = dynamic_cast<PackageSender *>(worker_ptr);
            if (sendrecv_ptr == sender) {
                continue;
            }
            has_sender_any_other_receiver_than_himself = true;
            if (color[sendrecv_ptr] == NodeColor::UNVISITED) {
                has_reachable_storehouse(sendrecv_ptr, color);
            }
        }
    }
    color[sender] = NodeColor::VERIFIED;
    if (has_sender_any_other_receiver_than_himself) {
        return true;
    } else {
        throw std::logic_error("Storehouse is unavailable");
    }

}
void Factory::do_deliveries(Time t) {
    for (auto &ramp : cont_r) {
        ramp.deliver_goods(t);
    }
}

void Factory::do_work(Time t ) {
    for (auto& worker : cont_w)
        worker.do_work(t);
}
void Factory::do_package_possing() {
    for (auto &ramp: cont_r) {
        ramp.send_package();
    }
    for (auto &worker: cont_w) {
        worker.send_package();
    }
}

std::pair<std::string, std::string> split_string(std::string strng, char delimiter){
    std::pair<std::string, std::string> res;
    std::string token = strng.substr(0, strng.find(delimiter));
    std::string part2 = strng.substr(strng.find(delimiter)+1, std::string::npos);
    res = std::make_pair(token, part2);
    return res;
}

void parse_pair(std::string src, std::map<std::string, std::string>& dest){
    dest.emplace(split_string(src, '='));
}

ParsedLineData parse_line(std::string line){
    ParsedLineData data = {};
    std::vector<std::string> tokens;
    std::string token;

    std::istringstream token_stream(line);
    char delimiter = ' ';

    while (std::getline(token_stream, token, delimiter)) {
        tokens.push_back(token);
    }

    std::map<std::string, std::string> parameters = {};

    for(auto it = tokens.begin() + 1; it != tokens.end(); ++it){
        parse_pair(*it, parameters);
    }

    std::map<std::string, ElementType> type = {{"LOADING_RAMP", ElementType::RAMP},{"WORKER", ElementType::WORKER},{"STOREHOUSE", ElementType::STOREHOUSE},{"LINK", ElementType::LINK}};

    ElementType current_type = type[tokens[0]];
    data = {current_type, parameters};

    return data;

}

Factory load_factory_structure(std::istream& is){
    Factory fac = Factory();

    std::string line;

    while (std::getline(is, line)) {
        if(line == "" || line[0] == ';') {
            continue;
        }else{
            ParsedLineData data = parse_line(line);

            switch (data.elementType){
                case ElementType::RAMP:
                    fac.add_ramp(Ramp(std::stoi(data.parameters["id"]), std::stoi(data.parameters["delivery-interval"])));
                    break;
                case ElementType::WORKER:
                    PackageQueueType queue_type;
                    if(data.parameters["queue-type"] == "FIFO"){
                        queue_type = PackageQueueType::FIFO;
                    }else{
                        queue_type = PackageQueueType::LIFO;
                    }
                    fac.add_worker(Worker(std::stoi(data.parameters["id"]), std::stoi(data.parameters["processing-time"]), std::make_unique<PackageQueue>(queue_type)));
                    break;
                case ElementType::STOREHOUSE:
                    fac.add_storehouse(Storehouse(std::stoi(data.parameters["id"])));
                    break;
                case ElementType::LINK:
                    auto [src_type, src_id] = split_string(data.parameters["src"], '-');
                    auto [dest_type, dest_id] = split_string(data.parameters["dest"], '-');
//
                    if(src_type == "ramp" && dest_type == "worker") {
                        std::list<Worker>::iterator dest = fac.find_worker_by_id(std::stoi(dest_id));
                        fac.find_ramp_by_id(std::stoi(src_id))->receiver_preferences_.add_receiver(&*dest);
                    }else if(src_type == "ramp" && dest_type == "store"){
                        std::list<Storehouse>::iterator dest = fac.find_storehouse_by_id(std::stoi(dest_id));
                        fac.find_ramp_by_id(std::stoi(src_id))->receiver_preferences_.add_receiver(&*dest);
                    }else if(src_type == "worker" && dest_type == "worker"){
                        std::list<Worker>::iterator dest = fac.find_worker_by_id(std::stoi(dest_id));
                        fac.find_worker_by_id(std::stoi(src_id))->receiver_preferences_.add_receiver(&*dest);
                    }else if(src_type == "worker" && dest_type == "store"){
                        std::list<Storehouse>::iterator dest = fac.find_storehouse_by_id(std::stoi(dest_id));
                        fac.find_worker_by_id(std::stoi(src_id))->receiver_preferences_.add_receiver(&*dest);
                    }
                    break;
            }
        }
    }
    return fac;
}


void save_factory_structure(Factory& factory, std::ostream& os){
    os << "; == LOADING RAMPS ==\n\n";
    for(auto it = factory.ramp_cbegin(); it != factory.ramp_cend(); it++){
        os << "LOADING_RAMP id=" << it->get_id() << " delivery-interval=" << it->get_delivery_interval() << '\n';
    }
    os << "\n; == WORKERS ==\n\n";
    for(auto it = factory.worker_cbegin(); it != factory.worker_cend(); it++){
        std::string q_type_str;
        switch(it->get_queue()->get_queue_type()){
            case PackageQueueType::LIFO:
                q_type_str = "LIFO";
                break;
            case PackageQueueType::FIFO:
                q_type_str = "FIFO";
                break;
        }
        os << "WORKER id=" << it->get_id() << " processing-time=" << it->get_processing_duration() << " queue-type=" << q_type_str << '\n';
    }
    os << "\n; == STOREHOUSES ==\n\n";
    for(auto it=factory.storehouse_cbegin(); it != factory.storehouse_cend(); ++it){
        os << "STOREHOUSE id=" << it->get_id() << '\n';
    }

    os << "\n; == LINKS ==\n\n";
    for(auto it = factory.ramp_cbegin(); it != factory.ramp_cend(); it++){
        for(auto iterator = it->receiver_preferences_.cbegin(); iterator != it->receiver_preferences_.cend(); iterator++){
            std::string dest_str;
            switch(iterator->first->get_receiver_type()){
                case ReceiverType::WORKER:
                    dest_str = "worker";
                    break;
                case ReceiverType::STOREHOUSE:
                    dest_str = "store";
                    break;
            }

            os << "LINK src=ramp-" << it->get_id() << " dest=" << dest_str << "-" << iterator->first->get_id() << '\n';
        }
        os << '\n';
    }
    for(auto it = factory.worker_cbegin(); it != factory.worker_cend(); it++){
        for(auto iterator = it->receiver_preferences_.cbegin(); iterator != it->receiver_preferences_.cend(); iterator++){
            std::string dest_str;
            switch(iterator->first->get_receiver_type()){
                case ReceiverType::WORKER:
                    dest_str = "worker";
                    break;
                case ReceiverType::STOREHOUSE:
                    dest_str = "store";
                    break;
            }

            os << "LINK src=worker-" << it->get_id() << " dest=" << dest_str << "-" << iterator->first->get_id() << '\n';
        }
        os << '\n';
    }
}





