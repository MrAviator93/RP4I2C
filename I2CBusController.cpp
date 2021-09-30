
// I2c
#include "I2CBusController.hpp"

// 3rd Party
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

extern "C" {
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
}

// C++
#include <vector>
#include <sstream>
#include <iostream>

namespace RPI
{
CI2CBusController::CI2CBusController( const std::string& busName )
	: m_i2cBusName { busName }
{
	m_fd = ::open( m_i2cBusName.c_str(), O_RDWR | O_NONBLOCK );
	if( m_fd < 0 )
	{
		reportError();
		return;
	}

	checkFunc();
}

CI2CBusController::~CI2CBusController()
{
	close( m_fd );
}

bool CI2CBusController::read( std::uint8_t slave_addr, std::uint8_t reg, std::uint8_t& result )
{
	std::unique_lock l { m_fdMtx };

	std::uint8_t outbuf[ 1 ];
	std::uint8_t inbuf[ 1 ];

	i2c_msg msgs[ 2 ];
	msgs[ 0 ].addr = slave_addr;
	msgs[ 0 ].flags = 0;
	msgs[ 0 ].len = 1;
	msgs[ 0 ].buf = &outbuf[ 0 ];

	msgs[ 1 ].addr = slave_addr;
	msgs[ 1 ].flags = I2C_M_RD | I2C_M_NOSTART;
	msgs[ 1 ].len = 1;
	msgs[ 1 ].buf = inbuf;

	i2c_rdwr_ioctl_data msgset[ 1 ];
	msgset[ 0 ].msgs = msgs;
	msgset[ 0 ].nmsgs = 2;

	outbuf[ 0 ] = reg;

	inbuf[ 0 ] = 0;

	if( ::ioctl( m_fd, I2C_RDWR, &msgset ) < 0 )
	{
		reportError();
		return false;
	}

	result = inbuf[ 0 ];

	return true;
}

std::int16_t
CI2CBusController::read( std::uint8_t slave_addr, std::uint8_t reg, std::uint8_t* data, std::uint16_t size )
{
	std::unique_lock l { m_fdMtx };

	std::uint8_t outbuf[ 2 ];
	outbuf[ 0 ] = reg;

	i2c_msg msgs[ 2 ];
	msgs[ 0 ].addr = slave_addr;
	msgs[ 0 ].flags = 0;
	msgs[ 0 ].len = 1;
	msgs[ 0 ].buf = outbuf;

	msgs[ 1 ].addr = slave_addr;
	msgs[ 1 ].flags = I2C_M_RD | I2C_M_NOSTART;
	msgs[ 1 ].len = size;
	msgs[ 1 ].buf = data;

	i2c_rdwr_ioctl_data msgset[ 1 ];
	msgset[ 0 ].msgs = msgs;
	msgset[ 0 ].nmsgs = 2;

	memset( data, 0x00, size );

	if( ::ioctl( m_fd, I2C_RDWR, &msgset ) < 0 )
	{
		reportError();
		return -1;
	}

	return size;
}

bool CI2CBusController::write( std::uint8_t slave_addr, std::uint8_t reg, std::uint8_t data )
{
	std::unique_lock l { m_fdMtx };

	std::uint8_t outbuf[ 2 ];
	outbuf[ 0 ] = reg;
	outbuf[ 1 ] = data;

	i2c_msg msgs[ 1 ];
	msgs[ 0 ].addr = slave_addr;
	msgs[ 0 ].flags = 0;
	msgs[ 0 ].len = 2;
	msgs[ 0 ].buf = outbuf;

	i2c_rdwr_ioctl_data msgset[ 1 ];
	msgset[ 0 ].nmsgs = 1;
	msgset[ 0 ].msgs = msgs;

	if( ::ioctl( m_fd, I2C_RDWR, &msgset ) < 0 )
	{
		reportError();
		return false;
	}

	return true;
}

bool CI2CBusController::write( std::uint8_t slave_addr, std::uint8_t reg, std::uint8_t* data, std::uint8_t size )
{
	std::unique_lock l { m_fdMtx };

	std::vector< std::uint8_t > data_buffer;
	data_buffer.resize( size + 1 ); // + 1 for register

	// Fill the data buffer
	data_buffer[ 0 ] = reg;
	for( int i = 1; i < size + 1; i++ )
	{
		data_buffer[ i ] = data[ i - 1 ];
	}

	i2c_msg msgs[ 1 ];
	msgs[ 0 ].addr = slave_addr;
	msgs[ 0 ].flags = 0;
	msgs[ 0 ].len = data_buffer.size();
	msgs[ 0 ].buf = data_buffer.data();

	i2c_rdwr_ioctl_data msgset[ 1 ];
	msgset[ 0 ].nmsgs = 1;
	msgset[ 0 ].msgs = msgs;

	if( ioctl( m_fd, I2C_RDWR, &msgset ) < 0 )
	{
		reportError();
		return false;
	}

	return true;
}

void CI2CBusController::reportError()
{
	auto e = errno;
	auto err { std::vector< char >( 256, 0 ) };

	std::unique_lock l { m_lastErrMtx };
	m_lastError = strerror_r( e, err.data(), err.size() );
}

bool check( std::uint32_t fs, std::uint32_t f, char const* what )
{
	if( fs & f )
	{
		std::ostringstream oss;
		oss << what << " SUPPORTED" << std::endl;
		return true;
	}

	return false;
}

void CI2CBusController::checkFunc()
{
	std::uint32_t funcs {};

	if( ::ioctl( m_fd, I2C_FUNCS, &funcs ) < 0 )
	{
		return reportError();
	}

	std::cerr << "Supported funcions are: " << std::hex << funcs << std::endl;
	check( funcs, I2C_FUNC_I2C, "I2C_FUNC_I2C" );
	check( funcs, I2C_FUNC_10BIT_ADDR, "I2C_FUNC_10BIT_ADDR" );
	check( funcs, I2C_FUNC_PROTOCOL_MANGLING, "I2C_FUNC_PROTOCOL_MANGLING" );
	check( funcs, I2C_FUNC_SMBUS_PEC, "I2C_FUNC_SMBUS_PEC" );
	check( funcs, I2C_FUNC_NOSTART, "I2C_FUNC_NOSTART" );
	check( funcs, I2C_FUNC_SLAVE, "I2C_FUNC_SLAVE" );
	check( funcs, I2C_FUNC_SMBUS_BLOCK_PROC_CALL, "I2C_FUNC_SMBUS_BLOCK_PROC_CALL" );
	check( funcs, I2C_FUNC_SMBUS_QUICK, "I2C_FUNC_SMBUS_QUICK" );
	check( funcs, I2C_FUNC_SMBUS_READ_BYTE, "I2C_FUNC_SMBUS_READ_BYTE" );
	check( funcs, I2C_FUNC_SMBUS_WRITE_BYTE, "I2C_FUNC_SMBUS_WRITE_BYTE" );
	check( funcs, I2C_FUNC_SMBUS_READ_BYTE_DATA, "I2C_FUNC_SMBUS_READ_BYTE_DATA" );
	check( funcs, I2C_FUNC_SMBUS_WRITE_BYTE_DATA, "I2C_FUNC_SMBUS_WRITE_BYTE_DATA" );
	check( funcs, I2C_FUNC_SMBUS_READ_WORD_DATA, "I2C_FUNC_SMBUS_READ_WORD_DATA" );
	check( funcs, I2C_FUNC_SMBUS_WRITE_WORD_DATA, "I2C_FUNC_SMBUS_WRITE_WORD_DATA" );
	check( funcs, I2C_FUNC_SMBUS_PROC_CALL, "I2C_FUNC_SMBUS_PROC_CALL" );
	check( funcs, I2C_FUNC_SMBUS_READ_BLOCK_DATA, "I2C_FUNC_SMBUS_READ_BLOCK_DATA" );
	check( funcs, I2C_FUNC_SMBUS_WRITE_BLOCK_DATA, "I2C_FUNC_SMBUS_WRITE_BLOCK_DATA" );
	check( funcs, I2C_FUNC_SMBUS_READ_I2C_BLOCK, "I2C_FUNC_SMBUS_READ_I2C_BLOCK" );
	check( funcs, I2C_FUNC_SMBUS_WRITE_I2C_BLOCK, "I2C_FUNC_SMBUS_WRITE_I2C_BLOCK" );
	check( funcs, I2C_FUNC_SMBUS_HOST_NOTIFY, "I2C_FUNC_SMBUS_HOST_NOTIFY" );
}

static_assert( std::is_same< __u8, std::uint8_t >::value, "__u8 definition differs from std::uint8_t definition." );
static_assert( std::is_same< unsigned int, std::uint32_t >::value,
			   "unsigned int definition differs from std::uint32_t definition." );

} // namespace RPI
