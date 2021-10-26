
#ifndef I2C_BMP388_CONTROLLER_HPP__
#define I2C_BMP388_CONTROLLER_HPP__

// I2C
#include "I2CBusController.hpp"

namespace I2C
{

/**
 * @class CBMP388Controller
 * @brief TBW, WIP
 * 
 * @author AK aka MrAviator93
 */
class CBMP388Controller
{
public:
	// Can be selected by the variable oversampling_setting
	enum class SamplingAccuracy : std::uint8_t
	{
		ULTRA_LOW_POWER = 0,
		STANDARD_RESOLUTION = 1,
		HIGH_RESOLUTION = 2,
		ULTRA_HIGH_RESOLUTION = 3
	};

	explicit CBMP388Controller( CI2CBusController& busController )
		: m_busController { busController }
	{ }

private:
	// TODO

private:
	CI2CBusController& m_busController; //!< I2C Bus Controller, allows to interface with I2C
};

} // namespace I2C
#endif // I2C_BMP388_CONTROLLER_HPP__
