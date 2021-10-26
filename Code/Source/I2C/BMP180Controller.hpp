/***********************************************************************
FILE LM75CONTROLLER.HPP CONTAINTS BMP180 CONTROLLER CLASS IMPLEMENTATION

FILE UTMOST REVIEW DONE ON (30.09.2021) BY ARTUR K. 
***********************************************************************/

#ifndef I2C_BMP180_CONTROLLER_HPP__
#define I2C_BMP180_CONTROLLER_HPP__

// I2C
#include "I2CBusController.hpp"

namespace I2C
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
	// Can be selected by the variable oversampling_setting
	enum class SamplingAccuracy : std::uint8_t
	{
		ULTRA_LOW_POWER = 0,
		STANDARD = 1,
		HIGH_RESOLUTION = 2,
		ULTRA_HIGH_RESOLUTION = 3
	};

	explicit CBMP180Controller( CI2CBusController& busController,
								SamplingAccuracy sAccuracy = SamplingAccuracy::STANDARD )
		: m_busController { busController }
		, m_samplingAccuracy { sAccuracy }
	{
		readCalibrationConsts();
	}

	float getTrueTemperatureC();

	float getTruePressurePa();

	/// Calculates the absolute altitude using international barometric formula
	float getAbsoluteAltitude();

private:
	/// Retrieves the calibration data constatns
	void readCalibrationConsts();

private:
	CI2CBusController& m_busController; //!< I2C Bus Controller, allows to interface with I2C
	const SamplingAccuracy m_samplingAccuracy;

	// Calibration constants - every sensor module has individual coefficients
	std::int16_t m_ac1 {};
	std::int16_t m_ac2 {};
	std::int16_t m_ac3 {};
	std::uint16_t m_ac4 {};
	std::uint16_t m_ac5 {};
	std::uint16_t m_ac6 {};
	std::int16_t m_b1 {};
	std::int16_t m_b2 {};
	std::int16_t m_mb {};
	std::int16_t m_mc {};
	std::int16_t m_md {};

private:
	static constexpr std::uint8_t m_bmp180Address {
		0x77 }; //!< Address of an BMP180 IC on the I2C bus, unchangable, singular
	static constexpr std::uint8_t m_bmp180WhoIam { 0xD0 }; //!< Who I am, should return 0x55
	static constexpr std::uint8_t m_bmp180Reset { 0xE0 }; //!<
	static constexpr std::uint8_t m_bmp180Control { 0xF4 }; //!<
	static constexpr std::uint8_t m_bmp180OutMsb { 0xF6 }; //!<
	static constexpr std::uint8_t m_bmp180OutLsb { 0xF7 }; //!<
	static constexpr std::uint8_t m_bmp180OutXlsb { 0xF8 }; //!<
};

} // namespace I2C
#endif // I2C_BMP180_CONTROLLER_HPP__
