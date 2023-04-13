#ifndef NETSIM_STORAGE_HPP
#define NETSIM_STORAGE_HPP

#include "types.hpp"
#include "package.hpp"
#include <list>
#include <algorithm>

enum class PackageQueueType {
    FIFO, LIFO
};
class IPackageStockpile{
public:
    using const_iterator = std::list<Package>::const_iterator;

    virtual void push(Package&& pkg) = 0;
    virtual bool empty() const = 0;
    virtual size_t size() const = 0;

    virtual IPackageStockpile::const_iterator begin() const = 0;
    virtual IPackageStockpile::const_iterator cbegin() const = 0;
    virtual IPackageStockpile::const_iterator end() const = 0;
    virtual IPackageStockpile::const_iterator cend() const = 0;

    virtual ~IPackageStockpile() = default;

};

class IPackageQueue : public IPackageStockpile{
public:
    virtual Package pop() = 0;
    virtual PackageQueueType get_queue_type() const = 0;
};

class PackageQueue : public IPackageQueue{
public:
    explicit PackageQueue(PackageQueueType type) : container_({}), queue_type_(type) {};

    PackageQueueType get_queue_type() const override {return queue_type_;};
    size_t size() const override { return container_.size();};
    bool empty() const override {return container_.empty();};


    void push(Package&& pkg) override {container_.emplace_back(std::move(pkg));};
    Package pop() override;


    IPackageStockpile::const_iterator begin() const override {return container_.cbegin();};
    IPackageStockpile::const_iterator cbegin() const override {return container_.cbegin();};
    IPackageStockpile::const_iterator end() const override {return container_.cend();};
    IPackageStockpile::const_iterator cend() const override {return container_.cend();};

    ~PackageQueue() override = default;
private:
    std::list<Package> container_;
    PackageQueueType queue_type_;

};

#endif