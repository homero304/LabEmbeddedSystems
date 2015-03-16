#!/usr/bin/env python

import smbus

## As before, we'll create an alias for our addresses, just to make things
##   a bit easier and more readable later on.
RTC = 0x68

## Initialize an smbus object. The parameter passed is the number of the I2C
##   bus; for the Arduino-ish headers on the pcDuino, it will be "2".
i2c = smbus.SMBus(1)

## With both of these devices, the first byte written specifies the address of
##   the register we want to read or write; for both devices, the device ID is
##   stored in location 0. Writing that address, than issuing a read, will
##   give us our answer.



def readRTC():
	seconds = str(i2c.read_byte_data(RTC, 0))
	minutes = str(i2c.read_byte_data(RTC, 1))
	hours = str(i2c.read_byte_data(RTC, 2))
	dayW = str(i2c.read_byte_data(RTC, 3))
	date = str(i2c.read_byte_data(RTC, 4))
	month = str(i2c.read_byte_data(RTC, 5))
	year = str(i2c.read_byte_data(RTC, 6))

	print("the time is %s:%s:%s, the day of the week is %s, and the date is %s/%s/%s\n" % (hours, minutes, seconds, dayW, date, month, year)) 

	pass

def writeRTC(seconds, minutes, hours, day, date, month, year):
	i2c.write_byte_data(RTC, 0, 0x80) #stop timer
	i2c.write_byte_data(RTC, 6, year)
	i2c.write_byte_data(RTC, 5, month)
	i2c.write_byte_data(RTC, 4, date)
	i2c.write_byte_data(RTC, 3, day)
	i2c.write_byte_data(RTC, 2, hours)
	i2c.write_byte_data(RTC, 1, minutes)
	i2c.write_byte_data(RTC, 0, seconds) #restart timer
	pass

writeRTC(0, 56, 3, 1, 16, 3, 15)
while 1:
	readRTC()
