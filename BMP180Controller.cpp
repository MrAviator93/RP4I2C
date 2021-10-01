
#include "BMP180Controller.hpp"

// C++
#include <array>
#include <cmath>
#include <chrono>
#include <thread>

// https://os.mbed.com/users/onehorse/code/BMP180/file/06dc60296e6e/BMP180.h/

using namespace std::chrono_literals;

namespace RPI
{
void CBMP180Controller::getCalibrationConsts()
{
	// std::int16_t ac1 = m_busController.readByte( m_bmp180Address, 0xAA ) << 8 | m_busController.readByte( m_bmp180Address, 0xAB );
	// std::int16_t ac2 = m_busController.readByte( m_bmp180Address, 0xAC ) << 8 | m_busController.readByte( m_bmp180Address, 0xAD );
	// std::int16_t ac3 = m_busController.readByte( m_bmp180Address, 0xAE ) << 8 | m_busController.readByte( m_bmp180Address, 0xAF );
	// std::uint16_t ac4 = m_busController.readByte( m_bmp180Address, 0xB0 ) << 8 | m_busController.readByte( m_bmp180Address, 0xB1 );
	// std::uint16_t ac5 = m_busController.readByte( m_bmp180Address, 0xB2 ) << 8 | m_busController.readByte( m_bmp180Address, 0xB3 );
	// std::uint16_t ac6 = m_busController.readByte( m_bmp180Address, 0xB4 ) << 8 | m_busController.readByte( m_bmp180Address, 0xB5 );
	// std::int16_t b1 = m_busController.readByte( m_bmp180Address, 0xB6 ) << 8 | m_busController.readByte( m_bmp180Address, 0xB7 );
	// std::int16_t b2 = m_busController.readByte( m_bmp180Address, 0xB8 ) << 8 | m_busController.readByte( m_bmp180Address, 0xB9 );
	// std::int16_t mb = m_busController.readByte( m_bmp180Address, 0xBA ) << 8 | m_busController.readByte( m_bmp180Address, 0xBB );
	// std::int16_t mc = m_busController.readByte( m_bmp180Address, 0xBC ) << 8 | m_busController.readByte( m_bmp180Address, 0xBD );
	// std::int16_t md = m_busController.readByte( m_bmp180Address, 0xBE ) << 8 | m_busController.readByte( m_bmp180Address, 0xBF );
}

float CBMP180Controller::getAltitude()
{
	const std::uint8_t OSS = static_cast< std::uint8_t >( m_samplingAccuracy ); // Pressure resolution

	// Read uncompensated temperature (UT) value
	m_busController.write( m_bmp180Address, 0xF4, 0x2E ); // Start temperature measurement

	std::this_thread::sleep_for( 4500us );

	std::array< std::uint8_t, 2 > rawUT {};
	m_busController.read( m_bmp180Address, 0xF6, rawUT.data(), rawUT.size() ); // Read raw temperature measurement
	std::int16_t UT = ( ( int16_t( rawUT[ 0 ] ) << 8 ) | rawUT[ 1 ] );

	// Start pressure measurement
	// write 0x34 + ( oss << 6 ) into reg 0xF4
	m_busController.write(
		m_bmp180Address, 0xF4, 0x34 | OSS << 6 ); // Configure pressure measurement for highest resolution

	std::this_thread::sleep_for( std::chrono::milliseconds( static_cast< int >( ( 5.0f + 8.0f * OSS ) ) ) );

	// Read reg 0xF6 (MSB), 0xF7 (LSB), 0xF8(XLSB)
	std::array< std::uint8_t, 3 > upRawData {};
	m_busController.read(
		m_bmp180Address, 0xF6, rawUT.data(), rawUT.size() ); // Read raw pressure measurement of 19 bits

	// UP = ( MSB << 16 + LSB << 8 + XLSB ) >> ( 8 - oss );
	std::int64_t UP =
		( ( (long)upRawData[ 0 ] << 16 ) | ( (long)upRawData[ 1 ] << 8 ) | upRawData[ 2 ] ) >> ( 8 - OSS );

	// Calculate true temperature
	// std::int64_t X1 = ( ( UT - AC6 ) * AC5 ) / std::pow( 2.0, 15 );
	// std::int64_t X2 = MC * std::pow( 2.0, 11 ) / ( X1 + MD );
	// std::int64_t B5 = X1 + X2;
	// std::int64_t T = ( B5 + 8 ) / std::pow( 2.0, 4 );  // Temperature calculated here will be in units of 0.1 deg C

	// Calculate true pressure
	// B6 = B5 - 4000;
	// X1 = ( B2 * ( B6 * B6 / std::pow( 2, 12 ) ) / std::pow( 2, 11 ) );
	// X2 = AC2 * B6 / std::pow( 2, 11 );
	// X3 = X1 + X2;
	// B3 = ( ( ( AC1 * 4 + X3 ) << oss ) + 2 ) / 4;
	// X1 = AC3 * B6 / std::pow( 2, 13 );
	// X2 = ( B1 * ( B6 * B6 / std::pow( 2, 12 ) ) ) / std::pow( 2, 16 );
	// X3 = ( ( X1 + X2 ) + 2 ) / std::pow( 2, 2 );
	// B4 = AC4 * static_cast< unsigned long >( X3 + 32768 ) / std::pow( 2, 15 );
	// B7 = ( (unsigned long)UP - B3 ) * ( 5000 >> oss );
	// if( B7 < 0x80000000 )
	// {
	// 	p = ( B7 * 2 ) / B4;
	// }
	// else
	// {
	// 	p = ( B7 / B4 ) * 2;
	// }

	// X1 = ( p / std::pow( 2, 8 ) * ( p / std::pow( 2, 8 ) ) );
	// X1 = ( X1 * 3038 ) / std::pow( 2, 16 );
	// X2 = ( -7357 * p ) / std::pow( 2, 16 );
	// p = p + ( X1 + X2 + 3791 ) / std::pow( 2, 4 ); // Pressure in units of Pa.

	return {};
}

} // namespace RPI
