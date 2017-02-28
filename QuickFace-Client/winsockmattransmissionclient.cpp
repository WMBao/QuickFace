#include "WinsockMatTransmissionClient.h"
WinsockMatTransmissionClient::WinsockMatTransmissionClient(void){}

WinsockMatTransmissionClient::~WinsockMatTransmissionClient(void){}

WinsockLogTransmissionClient::WinsockLogTransmissionClient(void){}

WinsockLogTransmissionClient::~WinsockLogTransmissionClient(void){}

WinsockFileTransmissionClient::WinsockFileTransmissionClient(void){}

WinsockFileTransmissionClient::~WinsockFileTransmissionClient(void){}

int WinsockLogTransmissionClient::socketConnect(const char* IP, int PORT){
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested = MAKEWORD( 1, 1 );
    err = WSAStartup( wVersionRequested, &wsaData );
    if ( err != 0 ) {
        return -1;
    }
    if ( LOBYTE( wsaData.wVersion ) != 1 ||HIBYTE( wsaData.wVersion ) != 1 ) {
        WSACleanup( );
        return -1;
    }
    err = (sockServer = socket(AF_INET,SOCK_STREAM,0));
    if (err < 0) {
        printf("create socket error: %s(errno: %d)\n\n", strerror(errno), errno);
        return -1;
    }
    else{
        printf("create socket successful!\nnow connect ...\n\n");
    }
    SOCKADDR_IN addrSrv;
    addrSrv.sin_addr.S_un.S_addr=inet_addr(IP);
    addrSrv.sin_family=AF_INET;
    addrSrv.sin_port=htons(PORT);
    err = connect(sockServer,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
    if (err < 0) {
        printf("connect error: %s(errno: %d)\n\n", strerror(errno), errno);
        return -1;
    }
    else {
        printf("connect successful!\n\n");
        return 1;
    }
}

void WinsockLogTransmissionClient::socketDisconnect(void){
    closesocket(sockServer);
    WSACleanup();
}

int WinsockFileTransmissionClient::socketConnect(const char* IP, int PORT){
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested = MAKEWORD( 1, 1 );
    err = WSAStartup( wVersionRequested, &wsaData );
    if ( err != 0 ) {
        return -1;
    }
    if ( LOBYTE( wsaData.wVersion ) != 1 ||HIBYTE( wsaData.wVersion ) != 1 ) {
        WSACleanup( );
        return -1;
    }
    err = (sockServer = socket(AF_INET,SOCK_STREAM,0));
    if (err < 0) {
        printf("create socket error: %s(errno: %d)\n\n", strerror(errno), errno);
        return -1;
    }
    else{
        printf("create socket successful!\nnow connect ...\n\n");
    }
    SOCKADDR_IN addrSrv;
    addrSrv.sin_addr.S_un.S_addr=inet_addr(IP);
    addrSrv.sin_family=AF_INET;
    addrSrv.sin_port=htons(PORT);
    err = connect(sockServer,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
    if (err < 0) {
        printf("connect error: %s(errno: %d)\n\n", strerror(errno), errno);
        return -1;
    }
    else {
        printf("connect successful!\n\n");
        return 1;
    }
}

void WinsockFileTransmissionClient::socketDisconnect(void){
    closesocket(sockServer);
    WSACleanup();
}

int WinsockMatTransmissionClient::socketConnect(const char* IP, int PORT){
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested = MAKEWORD( 1, 1 );
    err = WSAStartup( wVersionRequested, &wsaData );
    if ( err != 0 ) {
        return -1;
    }
    if ( LOBYTE( wsaData.wVersion ) != 1 ||HIBYTE( wsaData.wVersion ) != 1 ) {
        WSACleanup( );
        return -1;
    }
    err = (sockServer = socket(AF_INET,SOCK_STREAM,0));
    if (err < 0) {
        printf("create socket error: %s(errno: %d)\n\n", strerror(errno), errno);
        return -1;
    }
    else{
        printf("create socket successful!\nnow connect ...\n\n");
    }
    SOCKADDR_IN addrSrv;
    addrSrv.sin_addr.S_un.S_addr=inet_addr(IP);
    addrSrv.sin_family=AF_INET;
    addrSrv.sin_port=htons(PORT);
    err = connect(sockServer,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
    if (err < 0) {
        printf("connect error: %s(errno: %d)\n\n", strerror(errno), errno);
        return -1;
    }
    else {
        printf("connect successful!\n\n");
        return 1;
    }
}

void WinsockMatTransmissionClient::socketDisconnect(void){
    closesocket(sockServer);
    WSACleanup();
}

int WinsockMatTransmissionClient::transmit(cv::Mat image){
    if (image.empty()){
        printf("empty image\n\n");
        return -1;
    }
    if(image.cols != IMG_WIDTH || image.rows != IMG_HEIGHT || image.type() != CV_8UC3){
        printf("the image must satisfy : cols == IMG_WIDTH（%d）  rows == IMG_HEIGHT（%d） type == CV_8UC3\n\n", IMG_WIDTH, IMG_HEIGHT);
        return -1;
    }
    for(int k = 0; k < 32; k++) {
        int num1 = IMG_HEIGHT / 32 * k;
        for (int i = 0; i < IMG_HEIGHT / 32; i++){
            int num2 = i * IMG_WIDTH * 3;
            uchar* ucdata = image.ptr(i + num1);
            for (int j = 0; j < IMG_WIDTH * 3; j++){
                data1.buf[num2 + j] = ucdata[j];
            }
        }
        if(k == 31)
            data1.flag = 2;
        else
            data1.flag = 1;
        if (send(sockServer, (char *)(&data1), sizeof(data1), 0) < 0){
            printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
            return -1;
        }
    }

}

int WinsockMatTransmissionClient::receive(cv::Mat& image)
{
    cv::Mat img(IMG_HEIGHT,IMG_WIDTH,CV_8UC4,cv::Scalar(0));
    int needRecv = sizeof(recvbuf);
    int count = 0;
        for (int i = 0; i < 32; i++)
        {
            int pos = 0;
            int len0 = 0;

            while (pos < needRecv)
            {
                len0 = recv(sockServer, (char*)(&data2) + pos, needRecv - pos, 0);
                if (len0 < 0)
                {
                    printf("Server Recieve Data Failed!\n");
                    return -1;
                }
                pos += len0;
            }

            count = count + data2.flag;

            int num1 = IMG_HEIGHT / 32 * i;
            for (int j = 0; j < IMG_HEIGHT / 32; j++)
            {
                int num2 = j * IMG_WIDTH * 4;
                uchar* ucdata = img.ptr(j + num1);
                for (int k = 0; k < IMG_WIDTH * 4; k++)
                {
                    ucdata[k] = data2.buf[num2 + k];
                }
            }

            if (data2.flag == 2)
            {
                if (count == 33)
                {
                    image = img;
                    return 1;
                    count = 0;
                }
                else{
                    count = 0;
                    i = 0;
                }
            }
        }
        return -1;
}

char* WinsockLogTransmissionClient::receive(char* str)
{
    FD_ZERO(&fd);
    FD_SET(sockServer, &fd);
    select(0, &fd, NULL, NULL, &tv);
    if (!FD_ISSET(sockServer, &fd))  return NULL;
    char buffer[STR_BUFFER_SIZE];
    memset(buffer, 0, STR_BUFFER_SIZE);
    if (recv(sockServer, buffer, STR_BUFFER_SIZE, 0) < 0)
    {
        printf("Server receiving str failed.\n");
        return NULL;
    }
    printf("String: %s Receive successful!\n",  buffer);
    str[0] = '\0';
    strcat(str, buffer);
    return str;
}

int WinsockLogTransmissionClient::transmit(char* str)
{
    char buffer[STR_BUFFER_SIZE];
    memset(buffer, 0, STR_BUFFER_SIZE);
    strncpy(buffer, str, STR_BUFFER_SIZE);
    if (send(sockServer, buffer, STR_BUFFER_SIZE, 0) < 0)
    {
        printf("Server sending str failed.\n");
        return 1;
    }
    printf("String: %s Transfer successful!\n",  str);
    return 0;
}

int WinsockFileTransmissionClient::receive(char* file_name)
{
    FD_ZERO(&fd);
    FD_SET(sockServer, &fd);
    select(0, &fd, NULL, NULL, &tv);
    if (!FD_ISSET(sockServer, &fd))  return -1;
    char buffer[FILE_BUFFER_SIZE];
    FILE *fp = NULL;
    do
    {
        fp = fopen(file_name, "wb+");
    }while (!fp);

    memset(buffer, 0, FILE_BUFFER_SIZE);
    int length = 0;

    while((length = recv(sockServer, buffer, FILE_BUFFER_SIZE, 0)) > 0)
    {
        if (fwrite(buffer, sizeof(char), length, fp) < length)
        {
            fclose(fp);
            printf("File: %s receiving failure.\n", file_name);
            return 1;
        }
        memset(buffer, 0, FILE_BUFFER_SIZE);
    }
    fclose(fp);
//    char filename[80] = {'\0'};
//    if (index == 0)
//    {
//    strncpy(filename, file_name, strlen(file_name)-5);
//    result= rename(filename, file_name);
//    }else{
//        char tmp[3] = "";
//        char tmpsub[4] = "";
//        itoa(index/2, tmp, 10);
//        tmpsub = filename.substr(strlen(file_name)-9,strlen(file_name)-6);
//        std::string  new_file_name = filename.substr(0, strlen(file_name)-10);
//        strcat(new_file_name, tmp);
//        strcat(new_file_name, tmpsub);
//        do
//        {
//            result= rename(new_file_name, file_name);
//        }while (result != 0);
//    }
//    if (index % 2 == 0)
//    {
//        rename("current_merged.obj", file_name);
//    }else
//    {
//        rename("current_merged.isomap.png", file_name);
//    }
//    index++;
    printf("File: %s Receive successful.\n", file_name);


    FILE *fpn = NULL;
    do
    {
        fpn = fopen("current_merged.mtl", "wb+");
    }while (!fpn);

    char mtltmp[] = "newmtl FaceTexture\nmap_Kd current_merged.isomap.png\n" ;

    fwrite(mtltmp, sizeof(char), strlen(mtltmp), fpn);
    fclose(fpn);
//    rename(filename, file_name);
    return 0;
}
