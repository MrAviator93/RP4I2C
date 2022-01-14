/***********************************************************************
FILE LM75CONTROLLER.HPP CONTAINTS LM75 CONTROLLER CLASS IMPLEMENTATION

FILE UTMOST REVIEW DONE ON (30.09.2021) BY ARTUR K. 
***********************************************************************/

#ifndef I2C_LM75_CONTROLLER_HPP__
#define I2C_LM75_CONTROLLER_HPP__

// C++
#include <cstdint>

namespace I2C
{

/**
 * @class CLM75Controller
 * @brief Implements class that is able to communicate
 * with LM75 IC via I2C means. 
 * Allows to retrieve an ambient temperature measurement.
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
	explicit CLM75Controller( class CI2CBusController& busController, std::uint8_t address = 0x48 ) noexcept( true );

	/// Retrieves the temperature in degrees Celsius
	float getTemperatureC();

	/// Retrieves the temperature in Fahrenheit
	float getTemperatureF();

private:
	class CI2CBusController& m_busController; //!< I2C Bus Controller, allows to interface with I2C
	std::uint8_t m_lm75Address { 0x48 }; //!< Address of an LM75 IC on the I2C bus.

private:
	inline static constexpr std::uint8_t m_lm75_temp_read_reg { 0x00 }; //!< Temperature read register on LM75
};

} // namespace I2C
#endif // I2C_LM75_CONTROLLER_HPP__
