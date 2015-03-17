#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
void changeTime(int i2cHandle, int seconds, int minutes, int hours, int day, int date, int month, int year) {
	char txBuffer[32];
	int RTCAddress = 0x68;
	int tenBitAddress = 0;
	memset(txBuffer, 0, sizeof(txBuffer));
	txBuffer[0] = 0;
	txBuffer[1] = seconds;
	txBuffer[2] = minutes;
	txBuffer[3] = hours;
	txBuffer[4] = day;
	txBuffer[5] = date;
	txBuffer[6] = month;
	txBuffer[7] = year;
	int opResult = 0;
	opResult = write(i2cHandle, txBuffer, 8);  			
	
}

void readRTC(int i2cHandle, char* registers){
	char txBuffer[32];
	char rxBuffer[32];
	int RTCAddress = 0x68;
	int tenBitAddress = 0;
	memset(txBuffer, 0, sizeof(txBuffer));
	memset(rxBuffer, 0, sizeof(rxBuffer));
	txBuffer[0] = 0;
	int opResult = 0;
	opResult = write(i2cHandle, txBuffer, 1);  			
	opResult = read(i2cHandle, rxBuffer, 7);
	int i = 0;
	for(i;i<7;i++) registers[i] = rxBuffer[i];
}	
int main(void)
{
  // Set up some variables that we'll use along the way
  char rxBuffer[32];  // receive buffer
  char txBuffer[32];  // transmit buffer
  int RTCAddress= 0x68; // RTC device address
  int tenBitAddress = 0;  // is the device's address 10-bit? Usually not.
  int opResult = 0;   // for error checking of operations
  int seconds=0, minutes=0, hours=0, day=0, date=0, month=0, year=0;
  // Create a file descriptor for the I2C bus
  int i2cHandle = open("/dev/i2c-1", O_RDWR);

  // Tell the I2C peripheral that the device address is (or isn't) a 10-bit
  //   value. Most probably won't be.
  opResult = ioctl(i2cHandle, I2C_TENBIT, tenBitAddress);

  // Tell the I2C peripheral what the address of the device is. We're going to
  //   start out by talking to the gyro.
  opResult = ioctl(i2cHandle, I2C_SLAVE, RTCAddress);

  // Clear our buffers
  memset(rxBuffer, 0, sizeof(rxBuffer));
  memset(txBuffer, 0, sizeof(txBuffer));

 // txBuffer[0] = 0x00; // This is the address we want to read from.
  //opResult = write(i2cHandle, txBuffer, 1);
  //if (opResult != 1) printf("No ACK bit!\n");
  //opResult = read(i2cHandle, rxBuffer, 1);
  //printf("seconds %d\n", (int)rxBuffer[0]); // should print 105
//  changeTime(i2cHandle, 0, 49, 13, 2, 17, 3, 15);
  while(1)
  {
	readRTC(i2cHandle, rxBuffer);
	printf("the time is %d:%d:%d\nthe day of week is %d\n the date is %d/%d/%d\n", rxBuffer[2], rxBuffer[1], rxBuffer[0], rxBuffer[3], rxBuffer[4], rxBuffer[5], rxBuffer[6]);	
  }    
}
