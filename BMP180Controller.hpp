
#ifndef RPI_BMP180_CONTROLLER_HPP__
#define RPI_BMP180_CONTROLLER_HPP__

// I2C
#include "I2CBusController.hpp"

namespace RPI
{

/**
 * @class CBMP180Controller
 * @brief TBW
 * 
 * @author AK aka MrAviator93
 */
class CBMP180Controller
{
public:
	//  can be selected by the variable oversampling_setting
	enum class SamplingAccuracy
	{
		ULTRA_LOW_POWER,
		STANDARD,
		HIGH_RESOLUTION,
		ULTRA_HIGH_RESOLUTION
	};

	explicit CBMP180Controller( CI2CBusController& busController,
								SamplingAccuracy sAccuracy = SamplingAccuracy::STANDARD )
		: m_busController { busController }
		, m_samplingAccuracy { sAccuracy }
	{ }

private:
	CI2CBusController& m_busController; //!< I2C Bus Controller, allows to interface with I2C

	const SamplingAccuracy m_samplingAccuracy;

private:
	// Calibration constants
	const static std::uint16_t m_AC1MSBAddress { 0xAA }; //!< AC MSB
	const static std::uint16_t m_AC1LSBAddress { 0xAB }; //!< AC LSB
	// AC1 0xAA 0xAB
	// AC2 0xAC 0xAD
	// AC3 0xAE 0xAF
	// AC4 0xB0 0xB1
	// AC5 0xB2 0xB3
	// AC6 0xB4 0xB5
	// B1 0xB6 0xB7
	// B2 0xB8 0xB9
	// MB 0xBA 0xBB
	// MC 0xBC 0xBD
	// MD 0xBE 0xBF

	const static std::uint8_t m_bmp180Address { 0xEF }; //!< Address of an BMP180 IC on the I2C bus, unchangable, singular
};

} // namespace RPI
#endif // RPI_BMP180_CONTROLLER_HPP__
