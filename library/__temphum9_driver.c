/*
    __temphum9_driver.c

-----------------------------------------------------------------------------

  This file is part of mikroSDK.

  Copyright (c) 2017, MikroElektonika - http://www.mikroe.com

  All rights reserved.

----------------------------------------------------------------------------- */

#include "__temphum9_driver.h"
#include "__temphum9_hal.c"

/* ------------------------------------------------------------------- MACROS */

/* device address */
const uint8_t _TEMPHUM9_DEVICE_ADDRESS      = 0x70;

/* measurement modes */
const uint8_t _TEMPHUM9_NORMAL_MODE         = 0x00;
const uint8_t _TEMPHUM9_LOW_POWER_MODE      = 0x01;

/* data */
const uint8_t _TEMPHUM9_SINGLE_DATA         = 0x03;
const uint8_t _TEMPHUM9_DUAL_DATA           = 0x06;

/* commands */
const uint16_t _TEMPHUM9_SLEEP              = 0xB098;
const uint16_t _TEMPHUM9_WAKEUP             = 0x3517;
const uint16_t _TEMPHUM9_SOFT_RESET         = 0x805D;
const uint16_t _TEMPHUM9_GENERAL_CALL_RESET = 0x0006;
const uint16_t _TEMPHUM9_READ_ID            = 0xEFC8;

/* ---------------------------------------------------------------- VARIABLES */

#ifdef   __TEMPHUM9_DRV_I2C__
static uint8_t _slaveAddress;
#endif

/* -------------------------------------------- PRIVATE FUNCTION DECLARATIONS */



/* --------------------------------------------- PRIVATE FUNCTION DEFINITIONS */



/* --------------------------------------------------------- PUBLIC FUNCTIONS */

#ifdef   __TEMPHUM9_DRV_SPI__

void temphum9_spiDriverInit(T_TEMPHUM9_P gpioObj, T_TEMPHUM9_P spiObj)
{
    hal_spiMap( (T_HAL_P)spiObj );
    hal_gpioMap( (T_HAL_P)gpioObj );
}

#endif
#ifdef   __TEMPHUM9_DRV_I2C__

void temphum9_i2cDriverInit(T_TEMPHUM9_P gpioObj, T_TEMPHUM9_P i2cObj, uint8_t slave)
{
    _slaveAddress = slave;
    hal_i2cMap( (T_HAL_P)i2cObj );
    hal_gpioMap( (T_HAL_P)gpioObj );
}

#endif
#ifdef   __TEMPHUM9_DRV_UART__

void temphum9_uartDriverInit(T_TEMPHUM9_P gpioObj, T_TEMPHUM9_P uartObj)
{
    hal_uartMap( (T_HAL_P)uartObj );
    hal_gpioMap( (T_HAL_P)gpioObj );
}

#endif

/* ----------------------------------------------------------- IMPLEMENTATION */

void temphum9_readRegister( uint16_t registerAddress_, uint8_t nData_, uint16_t *registerBuffer_ )
{
    uint8_t writeBuffer[2];
    uint8_t readBuffer[6];
    
    writeBuffer[0] = registerAddress_ >> 8;
    writeBuffer[1] = registerAddress_;

    hal_i2cStart( );
    hal_i2cWrite( _slaveAddress, writeBuffer, 2, END_MODE_RESTART );
    hal_i2cRead( _slaveAddress, readBuffer, nData_, END_MODE_STOP );
    
    if (nData_ == _TEMPHUM9_SINGLE_DATA)
    {
        registerBuffer_[0] = (uint16_t)readBuffer[0];
        registerBuffer_[0] <<= 8;
        registerBuffer_[0] |= (uint16_t)readBuffer[1];
    }
    else if (nData_ == _TEMPHUM9_DUAL_DATA)
    {
        registerBuffer_[0] = (uint16_t)readBuffer[0];
        registerBuffer_[0] <<= 8;
        registerBuffer_[0] |= (uint16_t)readBuffer[1];
        
        registerBuffer_[1] = (uint16_t)readBuffer[3];
        registerBuffer_[1] <<= 8;
        registerBuffer_[1] |= (uint16_t)readBuffer[4];
    }
}

void temphum9_sendCommand( uint16_t command_ )
{
    uint8_t writeBuffer[2];

    writeBuffer[0] = command_ >> 8;
    writeBuffer[1] = command_;

    hal_i2cStart( );
    hal_i2cWrite( _slaveAddress, writeBuffer, 2, END_MODE_STOP );
}

float temphum9_getTemperature( uint8_t mode_ )
{
    float temperature_;
    uint16_t registerBuffer[1];

    if (mode_ == _TEMPHUM9_NORMAL_MODE)
    {
        temphum9_readRegister( 0x7CA2, _TEMPHUM9_SINGLE_DATA, registerBuffer );
    }
    else if (mode_ == _TEMPHUM9_LOW_POWER_MODE)
    {
        temphum9_readRegister( 0x6458, _TEMPHUM9_SINGLE_DATA, registerBuffer );
    }

    temperature_ = (float)registerBuffer[0];
    temperature_ *= 175;
    temperature_ /= 65536;
    temperature_ -= 45;
    
    return temperature_;
}

float temphum9_getRelativeHumidity( uint8_t mode_ )
{
    float relativeHumidity_;
    uint16_t registerBuffer[1];

    if (mode_ == _TEMPHUM9_NORMAL_MODE)
    {
        temphum9_readRegister( 0x5C24, _TEMPHUM9_SINGLE_DATA, registerBuffer );
    }
    else if (mode_ == _TEMPHUM9_LOW_POWER_MODE)
    {
        temphum9_readRegister( 0x44DE, _TEMPHUM9_SINGLE_DATA, registerBuffer );
    }

    relativeHumidity_ = (float)registerBuffer[0];
    relativeHumidity_ *= 100;
    relativeHumidity_ /= 65536;

    return relativeHumidity_;
}

void temhum9_getTemperatureAndHumidity( uint8_t mode_, float *measurementData )
{
    uint16_t registerBuffer[2];
    
    if (mode_ == _TEMPHUM9_NORMAL_MODE)
    {
        temphum9_readRegister( 0x7CA2, _TEMPHUM9_DUAL_DATA, registerBuffer );
    }
    else if (mode_ == _TEMPHUM9_LOW_POWER_MODE)
    {
        temphum9_readRegister( 0x6458, _TEMPHUM9_DUAL_DATA, registerBuffer );
    }
    
    measurementData[0] = (float)registerBuffer[0];
    measurementData[0] *= 175;
    measurementData[0] /= 65536;
    measurementData[0] -= 45;

    measurementData[1] = (float)registerBuffer[1];
    measurementData[1] *= 100;
    measurementData[1] /= 65536;
}


/* -------------------------------------------------------------------------- */
/*
  __temphum9_driver.c

  Copyright (c) 2017, MikroElektonika - http://www.mikroe.com

  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

3. All advertising materials mentioning features or use of this software
   must display the following acknowledgement:
   This product includes software developed by the MikroElektonika.

4. Neither the name of the MikroElektonika nor the
   names of its contributors may be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY MIKROELEKTRONIKA ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL MIKROELEKTRONIKA BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

----------------------------------------------------------------------------- */