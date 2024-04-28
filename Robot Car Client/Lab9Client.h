///////////////////////////////////////////////////////////////////
// Prepared for BCIT ELEX4618, March 25 2022, by Bryce Adam
///////////////////////////////////////////////////////////////////
#pragma once

#include <string>
#include <opencv2/opencv.hpp>

#define WIN4618
//#define PI4618

#ifdef WIN4618
#include "Winsock2.h"
#endif

#ifdef PI4618
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
typedef int SOCKET;
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <thread>
#endif

class CLab9Client
{
private:
#ifdef WIN4618
	WSADATA _wsdat;
#endif

	std::string _addr;
	SOCKET _socket;
	std::string server_ip;
	int server_port;

	bool setblocking(SOCKET fd, bool blocking);

public:
	CLab9Client(int _port);
	~CLab9Client();

	void connect_socket(std::string addr, int port);
	void close_socket();
	void send_command(std::string cmd);

	void tx_str(std::string str);
	bool rx_str(std::string& str);
	bool rx_im(cv::Mat& im);

	// video getting functionality
	cv::Mat getVideo();
	float _timeout_start;

	// static void image_thread(CLab9Client* client);
	// bool thread_exit;
};