#include "DemoBaker.h"

Baker::Baker(std::string name) : name(name) {}

void Baker::receiveDough(Dough dough) {
    std::cout << name << " received ";
    dough.describe();
}
