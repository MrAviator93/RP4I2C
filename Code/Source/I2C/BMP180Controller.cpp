
#include "BMP180Controller.hpp"

// C++
#include <array>
#include <cmath>
#include <chrono>
#include <thread>
#include <iostream>

// https://os.mbed.com/users/onehorse/code/BMP180/file/06dc60296e6e/BMP180.h/

using namespace std::chrono_literals;

namespace I2C
{

namespace
{
template < typename T >
T readCalibConst( CI2CBusController& busController, std::uint8_t bmp180Addr, std::uint8_t laddr, std::uint8_t raddr )
{
	std::uint8_t l {};
	std::uint8_t r {};
	busController.read( bmp180Addr, laddr, l );
	busController.read( bmp180Addr, raddr, r );
	T var {};
	var = l << 8;
	var |= r;
	return var;
}
} // namespace

void CBMP180Controller::readCalibrationConsts()
{
	m_ac1 = readCalibConst< std::int16_t >( m_busController, m_bmp180Address, 0xAA, 0xAB );
	m_ac2 = readCalibConst< std::int16_t >( m_busController, m_bmp180Address, 0xAC, 0xAD );
	m_ac3 = readCalibConst< std::int16_t >( m_busController, m_bmp180Address, 0xAE, 0xAF );
	m_ac4 = readCalibConst< std::uint16_t >( m_busController, m_bmp180Address, 0xB0, 0xB1 );
	m_ac5 = readCalibConst< std::uint16_t >( m_busController, m_bmp180Address, 0xB2, 0xB3 );
	m_ac6 = readCalibConst< std::uint16_t >( m_busController, m_bmp180Address, 0xB4, 0xB5 );
	m_b1 = readCalibConst< std::int16_t >( m_busController, m_bmp180Address, 0xB6, 0xB7 );
	m_b2 = readCalibConst< std::int16_t >( m_busController, m_bmp180Address, 0xB8, 0xB9 );
	m_mb = readCalibConst< std::int16_t >( m_busController, m_bmp180Address, 0xBA, 0xBB );
	m_mc = readCalibConst< std::int16_t >( m_busController, m_bmp180Address, 0xBC, 0xBD );
	m_md = readCalibConst< std::int16_t >( m_busController, m_bmp180Address, 0xBE, 0xBF );
}

float CBMP180Controller::getTrueTemperatureC()
{
	// Read uncompensated temperature (UT) value
	m_busController.write( m_bmp180Address, 0xF4, 0x2E ); // Start temperature measurement
	std::this_thread::sleep_for( 4500us );

	std::array< std::uint8_t, 2 > rawUT { 0, 0 };
	m_busController.read( m_bmp180Address, 0xF6, rawUT.data(), rawUT.size() ); // Read raw temperature measurement
	std::int16_t UT = ( ( int16_t( rawUT[ 0 ] ) << 8 ) | rawUT[ 1 ] );

	// Calculate true temperature
	std::int64_t X1 = ( std::int64_t( UT - m_ac6 ) * m_ac5 ) / std::pow( 2.0, 15 );
	std::int64_t X2 = m_mc * std::pow( 2.0, 11 ) / ( X1 + m_md );
	std::int64_t B5 = X1 + X2;
	std::int64_t T = ( B5 + 8 ) / std::pow( 2.0, 4 ); // Temperature calculated here will be in units of 0.1 deg C

	return static_cast< float >( T ) * 0.1f;
}

float CBMP180Controller::getTruePressurePa()
{
	// Pressure resolution
	const std::uint8_t OSS = static_cast< std::uint8_t >( m_samplingAccuracy );

	// Start temperature measurement
	m_busController.write( m_bmp180Address, 0xF4, 0x2E );
	std::this_thread::sleep_for( 4500us );

	// Read raw temperature measurement
	std::array< std::uint8_t, 2 > rawUT { 0, 0 };
	m_busController.read( m_bmp180Address, 0xF6, rawUT.data(), rawUT.size() );
	std::int16_t UT = ( ( std::int16_t( rawUT[ 0 ] ) << 8 ) | rawUT[ 1 ] );

	// Start pressure measurement
	m_busController.write(
		m_bmp180Address, 0xF4, 0x34 | OSS << 6 ); // Configure pressure measurement for highest resolution

	std::this_thread::sleep_for( std::chrono::milliseconds( static_cast< int >( ( 5.0f + 8.0f * OSS ) ) ) );

	// UP = ( MSB << 16 + LSB << 8 + XLSB ) >> ( 8 - oss );
	std::array< std::uint8_t, 3 > upRawData { 0, 0, 0 };
	m_busController.read( m_bmp180Address, 0xF6, upRawData.data(), upRawData.size() ); // Read raw ...
	std::int64_t UP = ( ( static_cast< std::int64_t >( upRawData[ 0 ] ) << 16 ) |
						( static_cast< std::int64_t >( upRawData[ 1 ] ) << 8 ) | upRawData[ 2 ] ) >>
					  ( 8 - OSS );

	// Temp related calculations
	std::int64_t X1 = ( std::int64_t( UT - m_ac6 ) * m_ac5 ) / std::pow( 2.0, 15 );
	std::int64_t X2 = m_mc * std::pow( 2.0, 11 ) / ( X1 + m_md );
	std::int64_t B5 = X1 + X2;

	// Calculate true pressure
	std::int64_t B6 = B5 - 4000;
	X1 = ( m_b2 * ( B6 * B6 / std::pow( 2, 12 ) ) / std::pow( 2, 11 ) );
	X2 = m_ac2 * B6 / std::pow( 2, 11 );
	std::int64_t X3 = X1 + X2;
	std::int64_t B3 = ( ( ( m_ac1 * 4 + X3 ) << OSS ) + 2 ) / 4;

	std::cerr << std::dec << "B6 " << B6 << std::endl;
	std::cerr << std::dec << "X1 " << X1 << std::endl;
	std::cerr << std::dec << "X2 " << X2 << std::endl;
	std::cerr << std::dec << "X3 " << X3 << std::endl;
	std::cerr << std::dec << "B3 " << B3 << std::endl;

	X1 = m_ac3 * B6 / std::pow( 2, 13 );
	X2 = ( m_b1 * ( B6 * B6 / std::pow( 2, 12 ) ) ) / std::pow( 2, 16 );
	X3 = ( ( X1 + X2 ) + 2 ) / std::pow( 2, 2 );
	std::int64_t B4 = m_ac4 * static_cast< std::int64_t >( X3 + 32768 ) / std::pow( 2, 15 );
	std::int64_t B7 = ( static_cast< std::int64_t >( UP ) - B3 ) * ( 5000 >> OSS );

	std::cerr << std::dec << "X1 " << X1 << std::endl;
	std::cerr << std::dec << "X2 " << X2 << std::endl;
	std::cerr << std::dec << "X3 " << X3 << std::endl;
	std::cerr << std::dec << "B4 " << B4 << std::endl;
	std::cerr << std::dec << "B7 " << B7 << std::endl;

	std::int32_t p {};
	if( B7 < 0x80000000 )
	{
		p = ( B7 * 2 ) / B4;
	}
	else
	{
		p = ( B7 / B4 ) * 2;
	}

	X1 = ( p / std::pow( 2, 8 ) * ( p / std::pow( 2, 8 ) ) );
	std::cerr << std::dec << "X1 " << X1 << std::endl;

	X1 = ( X1 * 3038 ) / std::pow( 2, 16 );
	std::cerr << std::dec << "X1 " << X1 << std::endl;

	X2 = ( -7357 * p ) / std::pow( 2, 16 );
	std::cerr << std::dec << "X2 " << X2 << std::endl;

	p = p + ( X1 + X2 + 3791 ) / std::pow( 2, 4 ); // Pressure in units of Pa.

	std::cerr << std::dec << "P " << p << std::endl;

#warning "This needs investigation the measured pressure appears to be 10 times lower than expected"

	return static_cast< float >( p ) * 10.0f;
}

float CBMP180Controller::getAbsoluteAltitude()
{
	// H = 44330 * [1 - (P/p0)^(1/5.255) ]
	const float pwr = 1.0f / 5.255f;
	const float pressAtSeaLevel = 101325.0f; // Pa
	float pd = getTruePressurePa() / pressAtSeaLevel;
	float r = 1.0f - std::pow( pd, pwr );
	return 44330.0f * r;
}

} // namespace I2C
