#include "storage_types.hpp"

Package PackageQueue::pop() {
    Package prod;
    switch (queue_type_) {
        case PackageQueueType::FIFO:
            prod = Package(std::move(container_.front()));
            container_.pop_front();
            break;
        case PackageQueueType::LIFO:
            prod = Package(std::move(container_.back()));
            container_.pop_back();
            break;
    }
    return prod;
}
