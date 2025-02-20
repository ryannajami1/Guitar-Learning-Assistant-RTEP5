#include "DemoDoughProducer.hpp"

#include <utility>
#include <iostream>
#include <ostream>
#include <string>
#include <functional>


Dough::Dough(std::string type, int quantity) : type(std::move(type)), quantity(quantity) {}

void Dough::Describe() const {
    std::cout << "Dough: " << quantity << " units of " << type << '\n';
}

DoughProducer::DoughProducer() {
    doughInventory_.emplace_back("Wheat", 100);
    doughInventory_.emplace_back("Rye", 50);
    doughInventory_.emplace_back("Sourdough", 75);
}

auto DoughProducer::ProduceDough(const std::string& type, int quantity) -> Dough {
    for (auto& dough : doughInventory_) {
        if (dough.type == type && dough.quantity >= quantity) {
            dough.quantity -= quantity;
            return {type, quantity};
        }
    }
    std::cerr << "Not enough " << type << " dough available." << '\n';
    return {"", 0}; // Return empty dough if not available
}

void DoughProducer::DeliverDough(const std::string& type, int quantity, const std::function<void(Dough)>& deliveryCallback) {
    Dough const dough = ProduceDough(type, quantity);
    if (dough.quantity > 0) {  // Only call back if dough was actually produced
        deliveryCallback(dough); // "Organiser" brings the baker
    }
}
