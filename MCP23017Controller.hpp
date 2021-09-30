
#ifndef RPI_MCP23017_CONTROLLER_HPP__
#define RPI_MCP23017_CONTROLLER_HPP__

#include "I2CBusController.hpp"

namespace RPI
{

/**
 * @class CMCP23017Controller
 * @brief TBW
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
		: m_busController { busController }
		, m_icAddress { address }
	{
		configure();
	}

	/// The secondary ctor, which enables the individual port configuration
	explicit CMCP23017Controller( CI2CBusController& busController,
								  std::uint8_t portAConfig = 0,
								  std::uint8_t portBConfig = 0,
								  std::uint8_t address = 0x20 ) noexcept( true )
		: m_busController { busController }
		, m_icAddress { address }
		, m_portAConfiguration { portAConfig }
		, m_portBConfiguration { portBConfig }
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

	// Set's on port a pin ...
	void setOnPortA( Pins pin )
	{
		// TBW
		// We will also need to perform a check if a pin is
		// set as output pin !
	}

	// TBW ...
	template < typename... Args >
	void setOnPortA( Pins first, Args... args )
	{
		// TBW
	}

private:
	/// Configures A and B ports setting pins as inputs or outputs
	void configure()
	{
		m_busController.write( m_icAddress, m_IODDIRARegister, m_portAConfiguration );
		m_busController.write( m_icAddress, m_IODDIRBRegister, m_portBConfiguration );
	}

private:
	CI2CBusController& m_busController; //!< I2C Bus Controller, allows to interface with I2C
	std::uint8_t m_icAddress { 0x20 }; //!< Address of an MCP23017 IC on the I2C bus.
	std::uint8_t m_portAConfiguration {}; //!< Bits identify if pins are set to output ( 0 ) or input ( 1 ) for port A
	std::uint8_t m_portBConfiguration {}; //!< Bits identify if pins are set to output ( 0 ) or input ( 1 ) for port B

private:
	inline const static std::uint8_t m_IODDIRARegister { 0x00 }; //!< MCP23017 IODIRA register from datasheet
	inline const static std::uint8_t m_IODDIRBRegister { 0x01 }; //!< MCP23017 IODIRB register from datasheet
};

} // namespace RPI
#endif // RPI_MCP23017_CONTROLLER_HPP__
