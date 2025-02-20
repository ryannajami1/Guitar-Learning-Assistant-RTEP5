#include "DemoBaker.hpp"

#include <utility>
#include <iostream>
#include <string>
#include "../input/DemoDoughProducer.hpp"


Baker::Baker(std::string name) : name(std::move(std::move(name))) {}

void Baker::ReceiveDough(const Dough& dough) const {
    std::cout << name << " received ";
    dough.Describe();
}
