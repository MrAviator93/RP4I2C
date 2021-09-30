/***********************************************************************
FILE LM75CONTROLLER.HPP CONTAINTS LM75 CONTROLLER CLASS IMPLEMENTATION

FILE UTMOST REVIEW DONE ON (21.07.2021) BY ARTUR K. 
***********************************************************************/

#ifndef RPI_LM75_CONTROLLER_HPP__
#define RPI_LM75_CONTROLLER_HPP__

// I2C
#include "I2CBusController.hpp"

// C++
#include <array>

namespace RPI
{

/**
 * @class CLM75Controller
 * @brief Implements class that is able to communicate
 * with LM75 IC via I2C means. Currently only read ambient
 * temperature method is implemented.
 * 
 * @author AK aka MrAviator93
 */
class CLM75Controller
{
public:
	/**
     * @brief Default Ctor
     * @param busController 
     * @param address the address of an LM75 IC on the I2C bus.
     */
	explicit CLM75Controller( CI2CBusController& busController, std::uint8_t address = 0x48 ) noexcept( true )
		: m_busController {busController}
		, m_lm75Address {address}
	{}

	/**
     * @brief Retrieves the temperature in degrees Celsius
     * @return float 
     */
	float getTemperature()
	{
		std::array< std::uint8_t, 2 > data {0, 0};

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

private:
	CI2CBusController& m_busController; //!< I2C Bus Controller, allows to interface with I2C

	std::uint8_t m_lm75Address {0}; //!< Address of an LM75 IC on the I2C bus.
	inline static const std::uint8_t m_lm75_temp_read_reg {0x00}; //!< Temperature read register on LM75
};

} // namespace RPI
#endif // RPI_LM75_CONTROLLER_HPP__
