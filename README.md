![MikroE](http://www.mikroe.com/img/designs/beta/logo_small.png)

---

# Temp_Hum_9 Click

- **CIC Prefix**  : TEMPHUM9
- **Author**      : Aleksandar Paunovic
- **Verison**     : 1.0.0
- **Date**        : Nov 2018.

---

### Software Support

We provide a library for the Temp_Hum_9 Click on our [LibStock](https://libstock.mikroe.com/projects/view/2671/temp-hum-9-click) 
page, as well as a demo application (example), developed using MikroElektronika 
[compilers](http://shop.mikroe.com/compilers). The demo can run on all the main 
MikroElektronika [development boards](http://shop.mikroe.com/development-boards).

**Library Description**

 Library contains 

Key functions :

- ``` void temphum9_readRegister( uint16_t registerAddress_, uint8_t nData_, uint16_t *registerBuffer_ ) ``` - reads one or two 16-bit registers
- ``` void temphum9_sendCommand( uint16_t command_ ) ``` - issues (sends) command to device
- ``` void temhum9_getTemperatureAndHumidity( uint8_t mode_, float *measurementData) ``` - performs temperature and relative humidity measurement and calculates temperature and relative humidity

**Examples Description**

Description :

The application is composed of three sections :

- System Initialization - Initializes LOG and I2C
- Application Initialization - Initializes I2C driver and sends SLEEP and WAKEUP dommands
- Application Task - Performs simultaneous temperature and relative humidity measurements and logs both values


```.c

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

```


The full application code, and ready to use projects can be found on our 
[LibStock](https://libstock.mikroe.com/projects/view/2671/temp-hum-9-click) page.

Other mikroE Libraries used in the example:

- I2C
- UART
- Conversions

**Additional notes and informations**

Depending on the development board you are using, you may need 
[USB UART click](http://shop.mikroe.com/usb-uart-click), 
[USB UART 2 Click](http://shop.mikroe.com/usb-uart-2-click) or 
[RS232 Click](http://shop.mikroe.com/rs232-click) to connect to your PC, for 
development systems with no UART to USB interface available on the board. The 
terminal available in all Mikroelektronika 
[compilers](http://shop.mikroe.com/compilers), or any other terminal application 
of your choice, can be used to read the message.

---
---
