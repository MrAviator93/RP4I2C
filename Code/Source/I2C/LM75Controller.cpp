
#include "LM75Controller.hpp"
#include "I2CBusController.hpp"

// C++
#include <array>

namespace I2C
{

CLM75Controller::CLM75Controller( CI2CBusController& busController, std::uint8_t address ) noexcept( true )
	: m_busController { busController }
	, m_lm75Address { address }
{ }

float CLM75Controller::getTemperatureC()
{
	std::array< std::uint8_t, 2 > data { 0, 0 };

	auto size = m_busController.read( m_lm75Address, m_lm75_temp_read_reg, data.data(), data.size() );
	if( size > 0 )
	{
		// Calculate temperature in Celsius
		std::int16_t itemp = ( data[ 0 ] << 8 ) | data[ 1 ];
		float temp = itemp / 256.0f;
		return temp;
	}

	return 0.0f;
}

float CLM75Controller::getTemperatureF()
{
	return getTemperatureC() * 1.8f + 32.0f;
}

} // namespace I2C
