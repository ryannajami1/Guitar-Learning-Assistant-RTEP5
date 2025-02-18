#include "DemoOrganiser.h"

int main() {
    DoughProducer producer;
    Baker baker1("Alice"), baker2("Bob");

    // Using a lambda for the callback:
    producer.deliverDough("Wheat", 50, [&baker1](Dough dough) {  // Capture baker1
        baker1.receiveDough(dough);
    });

    // Using a member function as a callback (needs std::bind or a lambda):
    producer.deliverDough("Rye", 25, std::bind(&Baker::receiveDough, &baker2, std::placeholders::_1));

    // Another example with a lambda:
    producer.deliverDough("Sourdough", 80, [](Dough dough) { // No baker needed, just print
       std::cout << "Dough delivered directly: ";
       dough.describe();
    });


    // Example of not enough dough
    producer.deliverDough("Wheat", 200, [&baker1](Dough dough) {
        baker1.receiveDough(dough); // Won't be called, as no dough will be available
    });

    return 0;
}
