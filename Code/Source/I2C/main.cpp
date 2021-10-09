
// I2C
#include "MCP23017Controller.hpp"
#include "I2CBusController.hpp"
#include "BMP180Controller.hpp"
#include "LM75Controller.hpp"

// C++
#include <iostream>

int main( int, char** )
{
	std::cout << "Welcome!" << std::endl;

	I2C::CI2CBusController bc { "/dev/i2c-1" };
	I2C::CLM75Controller lc { bc, 0x48 };

	// We will use MCP here to control LED's, configure ports A & B as output
	I2C::CMCP23017Controller mcp { bc, 0x00, 0x00, 0x20 };

	mcp.setOnPortA( I2C::CMCP23017Controller::Pins::PIN_8,
					I2C::CMCP23017Controller::Pins::PIN_7,
					I2C::CMCP23017Controller::Pins::PIN_6 );

	mcp.setOffPortA( I2C::CMCP23017Controller::Pins::PIN_8 );

	I2C::CBMP180Controller bmp { bc, I2C::CBMP180Controller::SamplingAccuracy::ULTRA_HIGH_RESOLUTION };

	std::cerr << "True temp: " << bmp.getTrueTemperatureC() << " C" << std::endl;
	std::cerr << "True pressure: " << bmp.getTruePressurePa() << " Pa" << std::endl;
	std::cerr << "Absolute altitude: " << bmp.getAbsoluteAltitude() << " m" << std::endl;

	return 0;
}
