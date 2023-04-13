#include "package.hpp"
#include <algorithm>
#include <iostream>


//std::set<ElementID> Package::assigned_IDs = {};
//std::set<ElementID> Package::freed_IDs = {};
//
//Package::Package() {
//    //Przypadek gdy zbiór wolnych id jest pusty, ustawiany jest max(assigned) + 1
//    if (!freed_IDs.empty()){
//        id_ = *freed_IDs.begin();
//        freed_IDs.erase(id_);
//        assigned_IDs.insert(id_);
//        //Inaczej brany jest minimalny ze zbioru wolnych
//    } else if (!assigned_IDs.empty()) {
//        id_ = *assigned_IDs.end() + 1;
//        std::cout << id_ << '\n';
//        assigned_IDs.insert(id_);
//
//    }else{
//        id_ = 1;
//        assigned_IDs.insert(id_);
//    }
//}
//
//
//bool Package::operator==(const Package&& p) {
//    return id_ == p.id_;
//}
//Package::Package(ElementID id) {
//        id_ = id;
//        assigned_IDs.insert(id);
//}

//std::set<ElementID> Package::freed_IDs_;
//std::set<ElementID> Package::assigned_IDs_;
//
//
//Package::Package() {
//    if (!freed_IDs_.empty()) {
//        id = *freed_IDs_.begin();
//        assigned_IDs_.insert(id);    /* dodanie nowego id  do zbioru przypsanych */
//        freed_IDs_.erase(id);    /* usunięcie przypisanego ze zbioru wolnych */
//    }
//    else if (!assigned_IDs_.empty()) {  //tutaj brakowało elsa po prostu
//        id = *(assigned_IDs_.end()) + 1;        /* inkrementacja */
//        assigned_IDs_.insert(id);
//    }
//    else {
//        id = 1;
//        assigned_IDs_.insert(id);
//    }
//}
//
//bool Package::operator==(const Package&& p) {
//    return this->id == p.id;
//}
//
//Package::Package(ElementID ID) {
//    id = ID;
//    assigned_IDs_.insert(id);
//}
////
//Package::~Package() {
//    assigned_IDs_.erase(id);
//    freed_IDs_.insert(id);
//    // dodajemy do wolnych te usunięte, aby mogły być znowu użyte
//
//}

std::set<ElementID> Package::freed_IDs;
std::set<ElementID> Package::assigned_IDs;


Package::Package() {
    if (!freed_IDs.empty()) {
        id_ = *freed_IDs.begin();
        assigned_IDs.insert(id_);    /* dodanie nowego id  do zbioru przypsanych */
        freed_IDs.erase(id_);    /* usunięcie przypisanego ze zbioru wolnych */
    }
    else if (!assigned_IDs.empty()) {  //tutaj brakowało elsa po prostu
        id_ = *(assigned_IDs.end()) + 1;        /* inkrementacja */
        assigned_IDs.insert(id_);
    }
    else {
        assigned_IDs.insert(id_);

        id_ = 1;

    }
}

bool Package::operator==(const Package&& pkg) {
    return id_ == pkg.id_;
}

Package::Package(ElementID ID) {
    id_ = ID;
    assigned_IDs.insert(id_);
}
//
