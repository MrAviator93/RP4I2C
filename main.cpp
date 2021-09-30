#include "I2CBusController.hpp"
#include "LM75Controller.hpp"

#include <iostream>

int main( int, char** )
{
    std::cout << "Welcome!" << std::endl;

    RPI::CI2CBusController bc{ "/dev/i2c-1" };
    RPI::CLM75Controller lc{ bc, 0x48 };
    std::cout << "Ambient temperature: " << lc.getTemperatureC() << std::endl;

    return 0;
}
