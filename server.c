#pragma config(Sensor, dgtl9,  dataPin,        sensorDigitalOut)
#pragma config(Sensor, dgtl10, readyPinOut,    sensorDigitalOut)
#pragma config(Sensor, dgtl11, readyPinIn,     sensorDigitalIn)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define END_LENGTH 32

void sendMessage(char *c);
void endMessage();
bool canSend();
void ready(bool status);
bool clientReady();
void send(bool bit);

bool needToSend = true;
char message[] = "CORAL!";

task main()
{
	clearDebugStream();
	ready(0);
	while(true)
	{
		// Can take over transmission
		if(canSend() && needToSend)
		{
			sendMessage(message);
			//endMessage();

			needToSend = false;
		}
		//message++;
		needToSend = true;
		wait1Msec(300);
	}
}

void sendMessage(char *c)
{
	writeDebugStreamLine("Sending message: %s", c);

	char lastChar = '0';
	for(int charIndex = 0; lastChar != '\0'; charIndex++)
	{
		lastChar = c[charIndex];
		writeDebugStreamLine("%c", lastChar);
		int m = (int)c;
		for(; m > 0; m/=2)
		{
			writeDebugStream("%d",m%2);
			// Send one bit of message
			send(m % 2);

			// Tell client the data is ready to be read
			//writeDebugStreamLine("Setting readyT");
			ready(true);

			// Wait until client reads the data
			while(!clientReady())
			{
				// Could sleep here but see if it lowers transmission speed if you do
			}

			// Tell client we are ready to send again
			//writeDebugStreamLine("Setting readyF");
			ready(false);


			// Wait until client is back to waiting for us to send
			while(clientReady())
			{
				// Could sleep here but see if it lowers transmission speed if you do
			}
		}
	}
}

void endMessage()
{
	for(int i=0; i<END_LENGTH; i++)
	{
		// Send one bit of message
		send(0);

		// Tell client the data is ready to be read
		ready(true);

		// Wait until client reads the data
		while(!clientReady())
		{
			// Could sleep here but see if it lowers transmission speed if you do
		}

		// Tell client we are ready to send again
		ready(false);

		// Wait until client is back to waiting for us to send
		while(clientReady())
		{
			// Could sleep here but see if it lowers transmission speed if you do
		}
	}
}

bool canSend()
{
	return SensorValue[readyPinIn] == 0 && SensorValue[readyPinOut] == 0;
}

void ready(bool status)
{
	SensorValue[readyPinOut] = status;
}

bool clientReady()
{
	return SensorValue[readyPinIn] == 1;
}

void send(bool bit)
{
	SensorValue[dataPin] = bit;
}
