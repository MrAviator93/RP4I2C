/***********************************************************************
FILE LM75CONTROLLER.HPP CONTAINTS MCP23017 CONTROLLER CLASS IMPLEMENTATION

FILE UTMOST REVIEW DONE ON (30.09.2021) BY ARTUR K. 
***********************************************************************/

#ifndef RPI_MCP23017_CONTROLLER_HPP__
#define RPI_MCP23017_CONTROLLER_HPP__

// I2C
#include "I2CBusController.hpp"

// C++
#include <iostream>

namespace I2C
{

/**
 * @class CMCP23017Controller
 * @brief TBW
 * 
 * @note MCP23017 output pin configuration:
 * 1. If pins are set as output after chip restart (power off - on)
 * the pins will be all on 0x00. To set them all of use i2cset -y 1 0x20 0x14 0xff
 * 
 * 
 * 
 * @author AK aka MrAviator93
 */
class CMCP23017Controller
{
public:
	/// MCP23017 chip available pinmodes
	enum class PinMode : std::uint8_t
	{
		OUTPUT = 0,
		INPUT = 1
	};

	/// MCP23017 chip available ports
	enum class Port : std::uint8_t
	{
		PORT_A = 0x14,
		PORT_B = 0x15
	};

	/// MCP23017 chip available pins on port A and B
	enum Pins : std::uint8_t
	{
		PIN_1 = 0x01,
		PIN_2 = 0x02,
		PIN_3 = 0x04,
		PIN_4 = 0x08,
		PIN_5 = 0x10,
		PIN_6 = 0x20,
		PIN_7 = 0x40,
		PIN_8 = 0x80
	};

	/// Default ctor, all pins are configured as output by default
	explicit CMCP23017Controller( CI2CBusController& busController, std::uint8_t address = 0x20 ) noexcept( true )
		: m_busController {busController}
		, m_icAddress {address}
	{
		configure();
	}

	/// The secondary ctor, which enables the individual port configuration
	explicit CMCP23017Controller( CI2CBusController& busController,
								  std::uint8_t portAConfig,
								  std::uint8_t portBConfig,
								  std::uint8_t address = 0x20 ) noexcept( true )
		: m_busController {busController}
		, m_icAddress {address}
		, m_portAConfiguration {portAConfig}
		, m_portBConfiguration {portBConfig}
	{
		configure();
	}

	void serPortConfig( Port port, std::uint8_t config )
	{
		if( port == Port::PORT_A )
		{
			if( m_portAConfiguration != config )
			{
				m_portAConfiguration = config;
				configure();
			}
		}

		if( port == Port::PORT_B )
		{
			if( m_portBConfiguration != config )
			{
				m_portBConfiguration = config;
				configure();
			}
		}
	}

	// Set's pin on on port A
	void setOnPortA( Pins pin )
	{
		// Check if the pin is configured to be an output
		if( ( m_portAConfiguration & static_cast< std::uint8_t >( pin ) ) == 0 )
		{
			m_portAPinStates &= ~static_cast< std::uint8_t >( pin );
			m_busController.write( m_icAddress, 0x14, m_portAPinStates );
		}
	}

	// TBW ...
	template < typename... Args >
	void setOnPortA( Pins pin, Args... pins )
	{
		const std::uint8_t pinValue =
			( static_cast< std::uint8_t >( pin ) | ... | static_cast< std::uint8_t >( pins ) );

		// Check if desired pins are configured as output
		if( ( m_portAConfiguration & pinValue ) == 0 )
		{
			m_portAPinStates &= ~pinValue;
			m_busController.write( m_icAddress, 0x14, m_portAPinStates );
		}
	}

	/// TBW
	void setOffPortA( Pins pin )
	{
		// Check if pin is configured to be an output
		if( ( m_portAConfiguration & static_cast< std::uint8_t >( pin ) ) == 0 )
		{
			m_portAPinStates |= static_cast< std::uint8_t >( pin );
			m_busController.write( m_icAddress, 0x14, m_portAPinStates );
		}
	}

	// TBW ...
	template < typename... Args >
	void setOffPortA( Pins pin, Args... pins )
	{
		const std::uint8_t pinValue =
			( static_cast< std::uint8_t >( pin ) | ... | static_cast< std::uint8_t >( pins ) );

		// Check if desired pins are configured as output
		if( ( m_portAConfiguration & pinValue ) == 0 )
		{
			m_portAPinStates |= pinValue;
			m_busController.write( m_icAddress, 0x14, m_portAPinStates );
		}
	}

private:
	/// Configures A and B ports setting pins as inputs or outputs
	void configure()
	{
		m_busController.write( m_icAddress, m_IODDIRARegister, m_portAConfiguration );
		m_busController.write( m_icAddress, m_IODDIRBRegister, m_portBConfiguration );
		retrieve();
	}

	/// TBW
	void retrieve()
	{
		m_busController.read( m_icAddress, 0x12, m_portAPinStates );
		m_busController.read( m_icAddress, 0x13, m_portBPinStates );
	}

private:
	CI2CBusController& m_busController; //!< I2C Bus Controller, allows to interface with I2C
	std::uint8_t m_icAddress {0x20}; //!< Address of an MCP23017 IC on the I2C bus.
	std::uint8_t m_portAConfiguration {}; //!< Bits identify if pins are set to output ( 0 ) or input ( 1 ) for port A
	std::uint8_t m_portBConfiguration {}; //!< Bits identify if pins are set to output ( 0 ) or input ( 1 ) for port B

	std::uint8_t m_portAPinStates {}; //!< TBW
	std::uint8_t m_portBPinStates {}; //!< TBW

private:
	inline const static std::uint8_t m_IODDIRARegister {0x00}; //!< MCP23017 IODIRA register from datasheet
	inline const static std::uint8_t m_IODDIRBRegister {0x01}; //!< MCP23017 IODIRB register from datasheet
};

} // namespace I2C
#endif // RPI_MCP23017_CONTROLLER_HPP__
