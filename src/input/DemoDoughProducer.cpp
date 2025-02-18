#include "DemoDoughProducer.h"

Dough::Dough(std::string type, int quantity) : type(type), quantity(quantity) {}

void Dough::describe() const {
    std::cout << "Dough: " << quantity << " units of " << type << std::endl;
}

DoughProducer::DoughProducer() {
    doughInventory.emplace_back("Wheat", 100);
    doughInventory.emplace_back("Rye", 50);
    doughInventory.emplace_back("Sourdough", 75);
}

Dough DoughProducer::produceDough(std::string type, int quantity) {
    for (auto& dough : doughInventory) {
        if (dough.type == type && dough.quantity >= quantity) {
            dough.quantity -= quantity;
            return Dough(type, quantity);
        }
    }
    std::cerr << "Not enough " << type << " dough available." << std::endl;
    return Dough("", 0); // Return empty dough if not available
}

void DoughProducer::deliverDough(std::string type, int quantity, std::function<void(Dough)> deliveryCallback) {
    Dough dough = produceDough(type, quantity);
    if (dough.quantity > 0) {  // Only call back if dough was actually produced
        deliveryCallback(dough); // "Organiser" brings the baker
    }
}
