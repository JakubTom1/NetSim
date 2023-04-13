#ifndef NETSIM_PACKAGE_HPP
#define NETSIM_PACKAGE_HPP

#include "types.hpp"
#include <set>

class Package{
public:
    Package();
    Package(ElementID ID); //usuwanie kopiujących ? noexcept dla przenoszących
    bool operator==(const Package&& p);
//    Package(const Package& pkg) = default;
//    Package(Package&& pkg) noexcept;
//
//    Package& operator= (Package&& pkg) noexcept;
//    ElementID get_id() const {return id_;};
//
//    ~Package() {if(id_!=-1){assigned_IDs.erase(id_); freed_IDs.insert(id_);}};

    ElementID get_id() const {return id_;}

    ~Package(){assigned_IDs.erase(id_); freed_IDs.insert(id_);};


private:
    ElementID id_;
    static std::set<ElementID> assigned_IDs;
    static std::set<ElementID> freed_IDs;

};


#endif