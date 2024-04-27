#include "stdafx.h"
#include "Control.h"
#include <string>

enum {DIGITAL = 0, ANALOG, SERVO};

void CControl::init_com(int comport)
{
	// Comport class (change port to your MSP device port)
	std::string port = "COM";
	char port_number = comport + '0'; // Convert the number into its char form
	port = port + port_number;
	_com.open(port);
}

bool CControl::get_data(int type, int channel, int& result)
{
	std::string tx_str = "G ";
	std::string rx_str;
	char data_type = type + '0';
	char char_channel = channel + '0';
	tx_str = tx_str + data_type + " " + char_channel + " <LF>"; // Make the string to send
	_com.write(tx_str.c_str(), tx_str.length());
	Sleep(10); // maybe not needed?

	// temportary storage
	char buff[2];

	rx_str = "";

	// start timeout count
	double start_time = cv::getTickCount();

	buff[0] = 0;
	// Read 1 byte and if an End Of Line then exit loop
    // Timeout after 1 second, if debugging step by step this will cause you to exit the loop
	while (buff[0] != '\n')
	{
		if (_com.read(buff, 1) > 0)
		{
			rx_str = rx_str + buff[0];
		}

		if ((cv::getTickCount() - start_time) / cv::getTickFrequency() < 1.0)
		{
			return false;
		}
	}

	int spacecounter = 0;
	std::string value_got = "";

	for (int rx_index = 0; rx_index < rx_str.size(); rx_index++)
	{
		if (spacecounter == 3)
		{
			value_got += rx_str[rx_index];
		}

		if (rx_str[rx_index] == ' ')
		{
			spacecounter++;
		}
	}

	result = std::stod(value_got);

	return true;
}

bool CControl::set_data(int type, int channel, int val)
{
	// Set the data?

	return true;
}

float CControl::get_analog(int channel)
{
	// Calls get_data and returns the analog input as a % of full scale

	return 0;
}

bool CControl::get_button(int button, int& result, int prev)
{
	// reads a digital input and debounces it (use a 1 second timeout)

	return true;
}