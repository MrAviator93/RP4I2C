
#ifndef RPI_MCP23017_CONTROLLER_HPP__
#define RPI_MCP23017_CONTROLLER_HPP__

#include "I2CBusController.hpp"

namespace RPI
{

enum Port : std::uint8_t
{
	PORT_A = 0x14,
	PORT_B = 0x15
};

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

/**
 * @class CMCP23017Controller
 * @brief This class needs to be completed
 */
class CMCP23017Controller
{
public:

/**
 * @brief TBW
 * This constructor configures all pins as output
 */
explicit CMCP23017Controller( CI2CBusController& busController, std::uint8_t address = 0x20 ) noexcept( true )
: m_busController{ busController }
, m_icAddress{ address }
{
	configure();
}

// Todo implement second ctor that enables to configure the pins
explicit CMCP23017Controller( CI2CBusController& busController, std::uint8_t portAConfig = 0, std::uint8_t portBConfig = 0, std::uint8_t address = 0x20 ) noexcept( true )
: m_busController{ busController }
, m_portAConfiguration{ portAConfig }
, m_portBConfiguration{ portBConfig }
, m_icAddress{ address }
{
	configure();
}

void setOn( Pins pin, Ports port )
{
	// TBW
	// We will also need to perform a check if a pin is
	// set as output pin !
}

template< typename ... Args >
void setOn( Pins first, Ports port, Args ... args )
{
	// TBW
}

private:

/**
 * @brief Configures A and B ports setting pins as inputs or outputs
 */
void configure()
{
	m_busController.write( m_icAddress, m_IODDIRARegister, m_portAConfiguration );
	m_busController.write( m_icAddress, m_IODDIRBRegister, m_portBConfiguration );
}

private:
	CI2CBusController& m_busController; //!< I2C Bus Controller, allows to interface with I2C
	std::uint8_t m_icAddress { 0 }; //!< Address of an MCP23017 IC on the I2C bus.
	std::uint8_t m_portAConfiguration { 0 } //!< Bits ideintify if pins are set to output ( 0 ) or input ( 1 ) for port A
	std::uint8_t m_portBConfiguration { 0 } //!< Bits ideintify if pins are set to output ( 0 ) or input ( 1 ) for port B
	
	
	inline const static m_IODDIRARegister{ 0x00 }; //!< MCP23017 IODIRA register
	inline const static m_IODDIRBRegister{ 0x01 }; //!< MCP23017 IODIRB register	
};

} // namespace RPI
#endif // RPI_MCP23017_CONTROLLER_HPP__
