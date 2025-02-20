#ifndef DEMOBAKER_H
#define DEMOBAKER_H

#include <string>
#include "../input/DemoDoughProducer.hpp"

class Baker {
public:
    std::string name;

    explicit Baker(std::string name);
    void ReceiveDough(const Dough& dough) const;
};

#endif
