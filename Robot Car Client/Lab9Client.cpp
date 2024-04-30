///////////////////////////////////////////////////////////////////
// Prepared for BCIT ELEX4618, April 2022, by Craig Hennessey
///////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <iostream>

#include "Lab9Client.h"

#ifdef WIN4618
#pragma comment(lib, "ws2_32.lib")
#endif

#define BUFF_SIZE 65535

CLab9Client::CLab9Client(int _port)
{
    _socket = 0;

#ifdef WIN4618
    if (WSAStartup(0x0101, &_wsdat))
    {
        WSACleanup();
        return;
    }

    server_ip = "192.168.137.124";
    server_port = _port;
    _timeout_start = cv::getTickCount();
    connect_socket(server_ip, server_port);
    // thread_exit = false;
#endif
}

bool CLab9Client::setblocking(SOCKET fd, bool blocking)
{
    if (fd < 0) return false;

#ifdef WIN4618
    unsigned long mode = blocking ? 0 : 1;
    return (ioctlsocket(fd, FIONBIO, &mode) == 0) ? true : false;
#else
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) return false;
    flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
    return (fcntl(fd, F_SETFL, flags) == 0) ? true : false;
#endif
}

void CLab9Client::connect_socket(std::string addr, int port)
{
    sockaddr_in ipaddr;

    _socket = socket(AF_INET, SOCK_STREAM, 0);
    if (_socket == SOCKET_ERROR)
    {
#ifdef WIN4618
        WSACleanup();
#endif
        return;
    }

    if (setblocking(_socket, false) == false)
    {
#ifdef WIN4618
        WSACleanup();
#endif
        return;
    }

    ipaddr.sin_family = AF_INET;
    ipaddr.sin_port = htons(port);
    ipaddr.sin_addr.s_addr = inet_addr(addr.c_str());

    connect(_socket, (struct sockaddr*)&ipaddr, sizeof(ipaddr));
}

void CLab9Client::close_socket()
{
#ifdef WIN4618
    closesocket(_socket);
#endif
#ifdef PI4618
    close(_socket);
#endif

    _socket = 0;
}

CLab9Client::~CLab9Client()
{
    close_socket();

#ifdef WIN4618
    WSACleanup();
#endif
}

void CLab9Client::tx_str(std::string str)
{
    send(_socket, str.c_str(), str.length(), 0);
}

bool CLab9Client::rx_str(std::string& str)
{
    char rxbuff[BUFF_SIZE];
    int rxbytes = 0;
    double start_time = cv::getTickCount();

    do
    {
        rxbytes = recv(_socket, rxbuff, BUFF_SIZE - 1, 0); // recvfrom

        if (rxbytes > 0)
        {
            rxbuff[rxbytes] = 0; // Add NULL
            str = rxbuff;
            return true;
        }
    } while (rxbytes == -1 && (cv::getTickCount() - start_time) / cv::getTickFrequency() < 0.05);  // Timeout after 1 second

    return false;
}

bool CLab9Client::rx_im(cv::Mat& im)
{
    char rxbuff[BUFF_SIZE];
    int rxbytes = -1;
    int imagebytes = 0;
    int minimum_image_size = 5000;

    double start_time = cv::getTickCount();

    // Store incoming data into byte array
    do
    {
        rxbytes = recv(_socket, rxbuff, BUFF_SIZE, 0);
    } while (rxbytes == -1 && (cv::getTickCount() - start_time) / cv::getTickFrequency() < 1.0);  // Timeout after 1 second

    // std::cout << "\nRXbytes = " << rxbytes;

    // If all the bytes were recieved, decode JPEG data to image (assumes image size minimum is 5kB)
    if (rxbytes > minimum_image_size)
    {
        im = imdecode(cv::Mat(rxbytes, 1, CV_8U, rxbuff), cv::IMREAD_UNCHANGED);
        return true;
    }

    return false;
}

cv::Mat CLab9Client::send_command(std::string cmd)
{
    std::string str;

    tx_str(cmd);
    // std::cout << "\nClient Tx: " << cmd;

    if (cmd == "im")
    {
        cv::Mat im;
        if (rx_im(im) == true)
        {
            _timeout_start = cv::getTickCount();
            /*
            if (im.empty() == false)
            {
                std::cout << "\nClient Rx: Image received";
                cv::imshow("rx", im);
                cv::waitKey(10);
            }
            */
        }
        else
        {
            if ((cv::getTickCount() - _timeout_start) / cv::getTickFrequency() > 1000)
            {
                // No response, disconnect and reconnect
                _timeout_start = cv::getTickCount();
                close_socket();
                connect_socket(server_ip, server_port);
            }
        }

        return im;
    }
    else
    {
        cv::Mat im;

        if (rx_str(str) == true)
        {
            _timeout_start = cv::getTickCount();
            std::cout << "\nServer reply: " << str;
        }
        else
        {
            if ((cv::getTickCount() - _timeout_start) / cv::getTickFrequency() > 1000)
            {
                // No response, disconnect and reconnect
                _timeout_start = cv::getTickCount();
                close_socket();
                connect_socket(server_ip, server_port);
            }
        }
        return im;
    }
}

cv::Mat CLab9Client::getVideo()
{
    cv::Mat _im;
    while (1)
    {
        send_command("im");
        if (rx_im(_im) == true)
        {
            std::cout << "Got into getVideo if" << std::endl;
            _timeout_start = cv::getTickCount();
            //if (_im.empty() == false)
            //{
            //	std::cout << "\nClient Rx: Image received";
            //	cv::imshow("rx", _im);
            //	cv::waitKey(10);
            //}
        }
        else
        {
            std::cout << "Got into getVideo else" << std::endl;
            if ((cv::getTickCount() - _timeout_start) / cv::getTickFrequency() > 1000)
            {
                // No response, disconnect and reconnect
                std::cout << "No response, disconnect and reconnect" << std::endl;
                _timeout_start = cv::getTickCount();
                close_socket();
                connect_socket("192.168.137.124", 4699);
            }
        }
        return _im;
    }

};

/*
void CLab9Client::image_thread(CLab9Client* client)
{
    cv::Mat image;

    while (client->thread_exit == false)
    {
        if (client->rx_im(image))
        {
            cv::imshow("Server Image", image);
            cv::waitKey(10);
        }
    }
}
*/