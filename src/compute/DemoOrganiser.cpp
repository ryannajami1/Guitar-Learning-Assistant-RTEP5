#include "DemoOrganiser.hpp"

#include <iostream>
#include <functional>
#include "../input/DemoDoughProducer.hpp"
#include "../output/DemoBaker.hpp"

auto main() -> int{
    DoughProducer producer;
    Baker baker1("Alice");
    Baker baker2("Bob");

    // Using a lambda for the callback:
    producer.DeliverDough("Wheat", 50, [&baker1](const Dough &dough) {  // Capture baker1
        baker1.ReceiveDough(dough);
    });

    // Using a member function as a callback (needs std::bind or a lambda - modern cpp prefers lambda):
    producer.DeliverDough("Rye", 25, [object_ptr = &baker2](auto && PH1) { object_ptr->ReceiveDough(std::forward<decltype(PH1)>(PH1)); });

    // Another example with a lambda:
    producer.DeliverDough("Sourdough", 80, [](const Dough &dough) { // No baker needed, just print
       std::cout << "Dough delivered directly: ";
       dough.Describe();
    });


    // Example of not enough dough
    producer.DeliverDough("Wheat", 200, [&baker1](const Dough &dough) {
        baker1.ReceiveDough(dough); // Won't be called, as no dough will be available
    });

    return 0;
}
