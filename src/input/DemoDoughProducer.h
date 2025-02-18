#ifndef DEMODOUGHPRODUCER_H
#define DEMODOUGHPRODUCER_H

#include <iostream>
#include <vector>
#include <functional>

class Dough {
public:
    std::string type;
    int quantity;

    Dough(std::string type, int quantity);
    void describe() const;
};

class DoughProducer {
private:
    std::vector<Dough> doughInventory;

public:
    DoughProducer();
    Dough produceDough(std::string type, int quantity);
    void deliverDough(std::string type, int quantity, std::function<void(Dough)> deliveryCallback);
};

#endif
