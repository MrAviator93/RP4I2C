#include "MCP23017Controller.hpp"
#include "I2CBusController.hpp"
#include "LM75Controller.hpp"

#include <iostream>

int main( int, char** )
{
	std::cout << "Welcome!" << std::endl;

	RPI::CI2CBusController bc {"/dev/i2c-1"};
	RPI::CLM75Controller lc {bc, 0x48};

	// We will use MCP here to control LED's, configure ports A & B as output
	RPI::CMCP23017Controller mcp {bc, 0x00, 0x00, 0x20};

	mcp.setOnPortA( RPI::CMCP23017Controller::Pins::PIN_8,
					RPI::CMCP23017Controller::Pins::PIN_7,
					RPI::CMCP23017Controller::Pins::PIN_6 );

    mcp.setOffPortA( RPI::CMCP23017Controller::Pins::PIN_8 );

	std::cout << "Ambient temperature: " << lc.getTemperatureC() << std::endl;

	return 0;
}
