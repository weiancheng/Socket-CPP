#include "TCPClient.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define TAG "TCPClient"
#define LOGD(format, ...) printf("[%s][%s][%s][%d]: " format "\n", TAG, __FILENAME__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LOGI(format, ...) printf("[%s] " format "\n", TAG, ##__VA_ARGS__)
#define LOGE(format, ...) printf("[%s] Error: " format "\n", TAG, ##__VA_ARGS__)

using namespace std;


TCPClient::TCPClient(): sockfd(-1), isConnected(false) {
}

TCPClient::~TCPClient() {
}

bool TCPClient::start(string ip, uint32_t port) {
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        LOGE("create socket error: %s (errno: %d)", strerror(errno), errno);
        return false;
    }
    LOGI("create socket...");

    memset(&servaddr, 0, sizeof(servaddr));;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &servaddr.sin_addr) <= 0) {
        LOGE("inet_pton error for %s", ip.c_str());
        return false;
    }

    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        LOGE("connect error: %s (errno: %d)", strerror(errno), errno);
        return false;
    }

    LOGI("create a connection with server (%s:%d)", ip.c_str(), port);
    isConnected = true;

    return true;
}

void TCPClient::stop() {
    if (sockfd > 0) {
        close(sockfd);
    }

    isConnected = false;
}

int TCPClient::sendToServer(const char *data, int data_len) {
    if (!isConnected) {
        LOGE("connection hasn't been created...");
        return -1;
    }

    if (data == nullptr) {
        LOGE("buffer is null...");
        return -1;
    }

    if (data_len < 0) {
        LOGE("data size is invalid...");
        return -1;
    }

    int n = send(sockfd, data, data_len, 0);
    if (n < 0) {
        LOGE("send data error: %s (errno: %d)", strerror(errno), errno);
        return -1;
    }

    return n;
}

int TCPClient::recvFromServer(char *buffer, int buffer_size) {
    if (!isConnected) {
        LOGE("connection hasn't been created...");
        return -1;
    }

    if (buffer == nullptr) {
        LOGE("buffer is null...");
        return -1;
    }

    if (buffer_size < 0) {
        LOGE("buffer size is invalid...");
        return -1;
    }

    return recv(sockfd, buffer, buffer_size, 0);
}
