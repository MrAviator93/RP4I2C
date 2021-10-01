/***********************************************************************
FILE LM75CONTROLLER.HPP CONTAINTS BMP180 CONTROLLER CLASS IMPLEMENTATION

FILE UTMOST REVIEW DONE ON (30.09.2021) BY ARTUR K. 
***********************************************************************/

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
		getCalibrationConsts();
	}

	float getAltitude();

private:
	/// Retrieves the calibration data constatns
	void getCalibrationConsts();

private:
	CI2CBusController& m_busController; //!< I2C Bus Controller, allows to interface with I2C
	const SamplingAccuracy m_samplingAccuracy;

private:
	// Calibration constants - every sensor module has individual coefficients
	// <EEPROM REG> < ADDRESS 2B > <type>
	// AC1 0xAA 0xAB short
	// AC2 0xAC 0xAD short
	// AC3 0xAE 0xAF short
	// AC4 0xB0 0xB1 ushort
	// AC5 0xB2 0xB3 ushort
	// AC6 0xB4 0xB5 ushort
	// B1 0xB6 0xB7 short
	// B2 0xB8 0xB9 short
	// MB 0xBA 0xBB short
	// MC 0xBC 0xBD short
	// MD 0xBE 0xBF short

	const static std::uint8_t m_bmp180Address {
		0xEE }; //!< Address of an BMP180 IC on the I2C bus, unchangable, singular
	const static std::uint8_t m_bmp180WhoIam { 0xD0 }; //!< Who I am, should return 0x55
	const static std::uint8_t m_bmp180Reset { 0xE0 }; //!<
	const static std::uint8_t m_bmp180Control { 0xF4 }; //!<
	const static std::uint8_t m_bmp180OutMsb { 0xF6 }; //!<
	const static std::uint8_t m_bmp180OutLsb { 0xF7 }; //!<
	const static std::uint8_t m_bmp180OutXlsb { 0xF8 }; //!<
};

} // namespace RPI
#endif // RPI_BMP180_CONTROLLER_HPP__
