#ifndef DEMOBAKER_H
#define DEMOBAKER_H

#include <iostream>
#include "../input/DemoDoughProducer.h"

class Baker {
public:
    std::string name;

    Baker(std::string name);
    void receiveDough(Dough dough);
};

#endif
