#ifndef __WINSOCKMATTRANSMISSIONCLIENT_H__
#define __WINSOCKMATTRANSMISSIONCLIENT_H__

#include "C:/Program Files/opencv/build/include/opencv2/opencv.hpp"
#include "C:/Program Files/opencv/build/include/opencv2/highgui/highgui.hpp"
#include "C:/Program Files/opencv/build/include/opencv2/imgproc/imgproc.hpp"
#include "C:/Program Files/opencv/build/include/opencv2/core/core.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Winsock2.h>
#include <time.h>
#pragma comment(lib, "WS2_32.lib")

#define IMG_WIDTH 640
#define IMG_HEIGHT 480

#define BUFFER_SIZE IMG_WIDTH*IMG_HEIGHT*3/32
#define FILE_BUFFER_SIZE 102400
#define STR_BUFFER_SIZE 256

struct recvbuf
{
    char buf[BUFFER_SIZE*4/3];
    int flag;
};

struct sentbuf
{
    char buf[BUFFER_SIZE];
    int flag;
};

class WinsockMatTransmissionClient
{
    public:
        WinsockMatTransmissionClient(void);
        ~WinsockMatTransmissionClient(void);

    private:
        SOCKET sockServer;
        struct sentbuf data1;
        struct recvbuf data2;

    public:
        int socketConnect(const char* IP, int PORT);

        int receive(cv::Mat & image);
        int transmit(cv::Mat image);
        void socketDisconnect(void);
};

class WinsockLogTransmissionClient
{
    public:
        WinsockLogTransmissionClient(void);
        ~WinsockLogTransmissionClient(void);

    private:
        SOCKET sockServer;
        fd_set fd;
        timeval tv = {1, 0};

    public:
        int socketConnect(const char* IP, int PORT);

        char * receive(char * str);
        int transmit(char * str);
        void socketDisconnect(void);
};

class WinsockFileTransmissionClient
{
    public:
        WinsockFileTransmissionClient(void);
        ~WinsockFileTransmissionClient(void);

    private:
        SOCKET sockServer;
        fd_set fd;
        timeval tv = {1, 0};
        int index = 0;

    public:
        int socketConnect(const char* IP, int PORT);
        int receive(char * file_name);
        void socketDisconnect(void);
};
#endif
