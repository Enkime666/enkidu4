#include "max6959.h"

uint8_t max6959_write (char ab, uint8_t command, uint8_t data)
{
	uint8_t retval = I2C_OK;
	// disable TWI interrupt
	cbi(TWCR, TWIE);

	// send start condition
	i2cSendStart ();
	i2cWaitForComplete ();

	// send device address with write
	i2cSendByte (TWI_SLA_MAX6959 | ab | TW_WRITE);
	i2cWaitForComplete ();

	// check if device is present and live
	if( inb(TWSR) == TW_MT_SLA_ACK)
	{
		i2cSendByte (command);
		i2cWaitForComplete ();

		i2cSendByte (data);
		i2cWaitForComplete ();
	}
	else
	{
		// device did not ACK it's address,
		// data will not be transferred
		// return error
		retval = I2C_ERROR_NODEV;
	}
	
	// transmit stop condition
	// leave with TWEA on for slave receiving
	i2cSendStop ();
	while( !(inb(TWCR) & BV(TWSTO)) );

	// enable TWI interrupt
	sbi (TWCR, TWIE);
	
	return retval;
}

uint8_t max6959_read (char ab, uint8_t command, uint8_t *data) 
{	
	uint8_t retval = I2C_OK;

	// disable TWI interrupt
	cbi(TWCR, TWIE);

	// send start condition
	i2cSendStart ();
	i2cWaitForComplete ();

	// send device address with write
	i2cSendByte (TWI_SLA_MAX6959 | ab | TW_WRITE);
	i2cWaitForComplete ();
	
	if( inb (TWSR) == TW_MT_SLA_ACK)
	{
		i2cSendByte (command);
		i2cWaitForComplete ();
	}
	else
	{
		// device did not ACK it's address,
		// data will not be transferred
		// return error
		retval = I2C_ERROR_NODEV;
	}

	// transmit stop condition
	// leave with TWEA on for slave receiving
	i2cSendStop ();
	while ( !(inb (TWCR) & BV(TWSTO)) );

	// enable TWI interrupt
	sbi (TWCR, TWIE);

	if (retval == I2C_ERROR_NODEV) return -2;

	////////////////////////////////////////////////////

	_delay_ms (1);
	retval = I2C_OK;
	cbi(TWCR, TWIE);

	// send start condition
	i2cSendStart ();
	i2cWaitForComplete ();

	// send device address with read
	i2cSendByte (TWI_SLA_MAX6959 | ab | TW_READ);
	i2cWaitForComplete ();

	// check if device is present and live
	if (inb(TWSR) == TW_MR_SLA_ACK)
	{
		// accept receive data and nack it (last-byte signal)
		i2cReceiveByte (FALSE);
		i2cWaitForComplete ();
		*data = i2cGetReceivedByte ();
	}
	else
	{
		// device did not ACK it's address,
		// data will not be transferred
		// return error
		retval = inb(TWSR)+1;
		retval = I2C_ERROR_NODEV;
	}

	// transmit stop condition
	// leave with TWEA on for slave receiving
	i2cSendStop ();
	while ( !(inb(TWCR) & BV(TWSTO)) );

	// enable TWI interrupt
	sbi (TWCR, TWIE);
	
	return retval;
	
}

void max6959_test (unsigned char ab)
{
	max6959_write (ab, SEGMENTS, 0xff);
	max6959_write (ab, DIGIT_0, 0xff);
	max6959_write (ab, DIGIT_1, 0xff);
	max6959_write (ab, DIGIT_2, 0xff);
	max6959_write (ab, DIGIT_3, 0xff);
	
}

void max6959_write_digits (unsigned char ab, UDigits digits)
{
	max6959_write (ab, DIGIT_0, digits.str.digit0);
	max6959_write (ab, DIGIT_1, digits.str.digit1);
	max6959_write (ab, DIGIT_2, digits.str.digit2);
	max6959_write (ab, DIGIT_3, digits.str.digit3);
}

void max6959_write_number (unsigned char ab, uint16_t number, char base, char a, char b)
{
	char i;
	for (i = a; i < b; ++i)
	{
		max6959_write (ab, DIGIT_0 + i, number % base);
		number /= base;
	}
}

