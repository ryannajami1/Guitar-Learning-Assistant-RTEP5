#include <iostream>
#include "../src/mult5.hpp"

auto main(int, char **) -> int
{
    if (Mult5(6) != 30) {
        std::cerr << "Error: 5*6 != 30\n";
        return 1; // Nonzero return indicates an error
    }
    return 0;
}
