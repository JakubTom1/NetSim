#ifndef NETSIM_NODES_HPP
#define NETSIM_NODES_HPP

#include "types.hpp"
#include "storage_types.hpp"
#include <memory>
#include <optional>
#include <map>
#include "helpers.hpp"
#include "config.hpp"

enum class ReceiverType {
    STOREHOUSE, WORKER
};

class IPackageReceiver{
public:
    virtual void receive_package(Package&& p) = 0;
    virtual ElementID get_id() const = 0;

    #if (defined EXERCISE_ID && EXERCISE_ID != EXERCISE_ID_NODES)
    virtual ReceiverType get_receiver_type() const = 0;
    #endif
    virtual IPackageStockpile::const_iterator begin() const = 0;
    virtual IPackageStockpile::const_iterator cbegin() const = 0;
    virtual IPackageStockpile::const_iterator end() const = 0;
    virtual IPackageStockpile::const_iterator cend() const = 0;

    virtual ~IPackageReceiver() = default;
};

class ReceiverPreferences{

public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;


    explicit ReceiverPreferences(ProbabilityGenerator pg = probability_generator) : pg_(std::move(pg)) {};
    void add_receiver(IPackageReceiver* r);
    void remove_receiver(IPackageReceiver* r);
    IPackageReceiver* choose_receiver();
    ReceiverPreferences::preferences_t& get_preferences() const {return const_cast<preferences_t &>(preferences_);};

    ReceiverPreferences::const_iterator begin() const { return preferences_.cbegin();};
    ReceiverPreferences::const_iterator cbegin() const { return preferences_.cbegin();};
    ReceiverPreferences::const_iterator end() const { return preferences_.cend();};
    ReceiverPreferences::const_iterator cend() const { return preferences_.cend();};

private:
    ProbabilityGenerator pg_;
    preferences_t preferences_;
};

class PackageSender{
public:
    //PackageSender(const PackageSender&) = default;
    //PackageSender(PackageSender&&) = default;
    //PackageSender& operator=(const PackageSender&) = default;
    ReceiverPreferences receiver_preferences_;

    void send_package();
    const std::optional<Package>& get_sending_buffer() {return buffer_;};

protected:
    void push_package(Package&& pkg);
    std::optional<Package> buffer_ = std::nullopt;

};

class Ramp : public PackageSender{
public:
    Ramp(ElementID id, TimeOffset di) : id_(id), di_(di) {};
    void deliver_goods(Time t);
    TimeOffset get_delivery_interval() const { return di_;};
    ElementID get_id() const {return id_;};

private:
    Time sim_start_time_ = 2147483647;
    ElementID id_;
    TimeOffset di_;
};

class Worker: public IPackageReceiver, public PackageSender {
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q) : id_(id), pd_(pd), q_(std::move(q)) {};
    void do_work(Time t);
    TimeOffset get_processing_duration() const { return pd_; };
    Time get_package_processing_stat_time() const {return package_processing_start_time_;};
    ElementID get_id() const override  {return id_;};
    void receive_package(Package&& p) override {q_->push(std::move(p));};
    #if (defined EXERCISE_ID && EXERCISE_ID != EXERCISE_ID_NODES)
    ReceiverType get_receiver_type() const override {return ReceiverType::WORKER;};
    #endif
    const std::optional<Package>& get_current_processing_buffer() const {return current_processing_buffer_;}
    IPackageQueue* get_queue() const { return q_.get();};
    const std::optional<Package>& get_processing_buffer() const {return buffer_;};
    IPackageStockpile::const_iterator cbegin() const override {return q_->cbegin();}
    IPackageStockpile::const_iterator begin() const override {return q_->cbegin();}
    IPackageStockpile::const_iterator cend() const override { return q_->cend();}
    IPackageStockpile::const_iterator end() const override { return q_->cend();}
private:
    std::optional<Package> current_processing_buffer_;
    ElementID id_;
    TimeOffset pd_;
    Time package_processing_start_time_;
    std::unique_ptr<IPackageQueue> q_;

};

class Storehouse: public IPackageReceiver{
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d = std::make_unique<PackageQueue>(PackageQueueType::FIFO)) : id_(id), d_(std::move(d)) {};
    ElementID get_id() const override {return id_;};
    void receive_package(Package&& p) override {d_->push(std::move(p));};

    #if (defined EXERCISE_ID && EXERCISE_ID != EXERCISE_ID_NODES)
    ReceiverType get_receiver_type() const override {return ReceiverType::STOREHOUSE;};
    #endif
    IPackageStockpile::const_iterator cbegin() const override {return d_->cbegin();}
    IPackageStockpile::const_iterator begin() const override {return d_->cbegin();}
    IPackageStockpile::const_iterator cend() const override { return d_->cend();}
    IPackageStockpile::const_iterator end() const override { return d_->cend();}
private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> d_;
};



#endif