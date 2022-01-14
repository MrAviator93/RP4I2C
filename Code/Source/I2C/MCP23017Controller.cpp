
#include "MCP23017Controller.hpp"
#include "I2CBusController.hpp"

namespace I2C
{

CMCP23017Controller::CMCP23017Controller( CI2CBusController& busController, std::uint8_t address ) noexcept( true )
	: m_busController { busController }
	, m_icAddress { address }
{
	configure();
}

CMCP23017Controller::CMCP23017Controller( CI2CBusController& busController,
										  std::uint8_t portAConfig,
										  std::uint8_t portBConfig,
										  std::uint8_t address ) noexcept( true )
	: m_busController { busController }
	, m_icAddress { address }
	, m_portAConfiguration { portAConfig }
	, m_portBConfiguration { portBConfig }
{
	configure();
}

void CMCP23017Controller::serPortConfig( Port port, std::uint8_t config )
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

void CMCP23017Controller::setOnPortA( Pins pin )
{
	// Check if the pin is configured to be an output
	if( ( m_portAConfiguration & static_cast< std::uint8_t >( pin ) ) == 0 )
	{
		m_portAPinStates &= ~static_cast< std::uint8_t >( pin );
		m_busController.write( m_icAddress, 0x14, m_portAPinStates );
	}
}

void CMCP23017Controller::setOffPortA( Pins pin )
{
	// Check if pin is configured to be an output
	if( ( m_portAConfiguration & static_cast< std::uint8_t >( pin ) ) == 0 )
	{
		m_portAPinStates |= static_cast< std::uint8_t >( pin );
		m_busController.write( m_icAddress, 0x14, m_portAPinStates );
	}
}

void CMCP23017Controller::configure()
{
	m_busController.write( m_icAddress, m_IODDIRARegister, m_portAConfiguration );
	m_busController.write( m_icAddress, m_IODDIRBRegister, m_portBConfiguration );
	retrieve();
}

void CMCP23017Controller::retrieve()
{
	m_busController.read( m_icAddress, 0x12, m_portAPinStates );
	m_busController.read( m_icAddress, 0x13, m_portBPinStates );
}

void CMCP23017Controller::write( std::uint8_t portAddress, std::uint8_t pinStates )
{
	m_busController.write( m_icAddress, portAddress, pinStates );
}

} // namespace I2C
