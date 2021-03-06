/*
Example for Temp_Hum_9 Click

    Date          : Nov 2018.
    Author        : Aleksandar Paunovic

Test configuration CEC :
    
    MCU              : CEC1702
    Dev. Board       : Clicker 2 for CEC1702
    ARM Compiler ver : v6.0.0.0

---

Description :

The application is composed of three sections :

- System Initialization - Initializes LOG and I2C
- Application Initialization - Initializes I2C driver and sends SLEEP and WAKEUP dommands
- Application Task - Performs simultaneous temperature and relative humidity measurements and logs both values

*/

#include "Click_Temp_Hum_9_types.h"
#include "Click_Temp_Hum_9_config.h"

char text[50];
float measurementData[2];

void systemInit( )
{
    mikrobus_i2cInit( _MIKROBUS1, &_TEMPHUM9_I2C_CFG[0] );
    Delay_ms(500);
    mikrobus_logInit( _MIKROBUS2, 9600 );
    Delay_ms(100);
    
    mikrobus_logWrite( "< < < sys init done > > >", _LOG_LINE );
}

void applicationInit( )
{
    temphum9_i2cDriverInit( (T_TEMPHUM9_P)&_MIKROBUS1_GPIO, (T_TEMPHUM9_P)&_MIKROBUS1_I2C, _TEMPHUM9_DEVICE_ADDRESS );
    temphum9_sendCommand( _TEMPHUM9_SLEEP );
    Delay_ms(500);
    temphum9_sendCommand( _TEMPHUM9_WAKEUP );
    Delay_ms(100);
    
    mikrobus_logWrite( "< < < app init done > > >", _LOG_LINE );
}

void applicationTask( )
{
    mikrobus_logWrite( " ", _LOG_LINE );

    temhum9_getTemperatureAndHumidity( _TEMPHUM9_NORMAL_MODE, &measurementData[0] );
    
    FloatToStr( measurementData[0], text );
    mikrobus_logWrite( "> > > Temperature       : ", _LOG_TEXT );
    mikrobus_logWrite( text, _LOG_TEXT );
    mikrobus_logWrite( " C", _LOG_LINE );
    
    FloatToStr( measurementData[1], text );
    mikrobus_logWrite( "> > > Relative humidity : ", _LOG_TEXT );
    mikrobus_logWrite( text, _LOG_TEXT );
    mikrobus_logWrite( " %", _LOG_LINE );
    
    mikrobus_logWrite( " ", _LOG_LINE );
    
    Delay_ms(1000);
}

void main( )
{
    systemInit( );
    applicationInit( );

    while (1)
    {
        applicationTask( );
    }
}