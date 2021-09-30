/***********************************************************************
FILE I2CBUSCONTROLLER.HPP CONTAINTS I2C BUS CONTROLLER CLASS IMPLEMENTATION

FILE UTMOST REVIEW DONE ON (21.07.2021) BY ARTUR K. 
***********************************************************************/

#ifndef RPI_I2C_BUS_CONTROLLER_HPP__
#define RPI_I2C_BUS_CONTROLLER_HPP__

#include <cstdint>
#include <string>
#include <mutex>

namespace RPI
{

/**
 * @class CI2CBusController
 * @brief An interface wrapper for I2C bus.
 * To list the I2C buses available: i2cdetect -l
 * or you may use also: ls /dev/i2c*
 * Say, 0 & 1 are available.
 * Then, each bus could be scanned to see what
 * all device addresses exist on each bus.
 * i2cdetect -y 0 or with i2cdetect -y -r 0
 * 
 * @author AK aka MrAviator93
 */
class CI2CBusController
{
public:
	/**
     * @brief Default Ctor opens a file descriptor.
     * @param bus
     */
	explicit CI2CBusController( const std::string& busName );

	/**
     * @brief Default Dtor
     * Closes file m_fd file.
     */
	~CI2CBusController();

	/**
     * @brief Returns the os name of the physical bus
     * Example: /dev/i2c-X
     * @return const std::string&
     */
	auto& bus() const
	{
		return m_i2cBusName;
	}

	/**
     * @brief TBW
     * 
     * @param slaveAddr 
     * @param reg 
     * @param result 
     * @return true 
     * @return false 
     */
	bool read( std::uint8_t slaveAddr, std::uint8_t reg, std::uint8_t& result );

	/**
     * @brief TBW
     * 
     * @param slaveAddr 
     * @param reg 
     * @param data 
     * @param size 
     * @return std::int16_t 
     */
	std::int16_t read( std::uint8_t slaveAddr, std::uint8_t reg, std::uint8_t* data, std::uint16_t size );

	/**
     * @brief TBW
     * 
     * @param slaveAddr 
     * @param reg 
     * @param data 
     * @return true 
     * @return false 
     */
	bool write( std::uint8_t slaveAddr, std::uint8_t reg, std::uint8_t data );

	/**
     * @brief TBW
     * 
     * @param slaveAddr 
     * @param reg 
     * @param data 
     * @param size 
     * @return true 
     * @return false 
     */
	bool write( std::uint8_t slaveAddr, std::uint8_t reg, std::uint8_t* data, std::uint8_t size );

	/**
     * @brief Retrieves error buffer and
     * outputs it into the looger using make_error method.
     */
	void reportError();

	/**
     * @brief An accessor to the last error
     * @return std::string const&
     */
	std::string lastError() const
	{
		std::unique_lock l( m_lastErrMtx );
		return m_lastError;
	}

private:
	/**
     * @brief Requesting the bus for capabilities/features/functionality
     */
	void checkFunc();

private:
	std::string m_i2cBusName; //!< I2C Bus name, i.e. "/dev/i2c-1"

	mutable std::mutex m_fdMtx; //!< Locks the read write operations
	int m_fd {-1}; //!< TBW

	mutable std::mutex m_lastErrMtx; //!< Locks the last_error string operations
	std::string m_lastError; //<! TBW
};

} // namespace RPI
#endif // RPI_I2C_BUS_CONTROLLER_HPP__