#ifndef DEMODOUGHPRODUCER_H
#define DEMODOUGHPRODUCER_H

#include <vector>
#include <functional>
#include <string>


class Dough {
public:
    std::string type;
    int quantity;

    Dough(std::string type, int quantity);
    void Describe() const;
};

class DoughProducer {
private:
    std::vector<Dough> doughInventory_;

public:
    DoughProducer();
    auto ProduceDough(const std::string& type, int quantity) -> Dough;
    void DeliverDough(const std::string& type, int quantity, const std::function<void(Dough)>& deliveryCallback);
};

#endif
