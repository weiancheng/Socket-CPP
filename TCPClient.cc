#include "TCPClient.h"
#include "DataPackage.h"

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


TCPClient::TCPClient(): sockfd(-1), isConnected(false), count(0) {
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

int TCPClient::sendToServer(const void *buf, int len) {
    if (!isConnected) {
        LOGE("connection hasn't been created...");
        return -1;
    }

    if (buf == nullptr) {
        LOGE("buffer is null...");
        return -1;
    }

    if (len < 0) {
        LOGE("buffer size is invalid...");
        return -1;
    }

    struct DataPackage package;
    memset(&package, 0, sizeof(package));
    package.index = count;
    package.isEOF = false;
    package.length = len;
    memcpy(package.buffer, buf, len);

    if (send(sockfd, (void *)&package, sizeof(package), 0)) {
        LOGE("send data error: %s (errno: %d)", strerror(errno), errno);
        return -1;
    }

    count++;

    return package.length;
}

void TCPClient::sendEOFToServer() {
    if (!isConnected) {
        LOGE("connection hasn't been created...");
        return;
    }

    struct DataPackage package;
    memset(&package, 0, sizeof(package));
    package.index = count;
    package.isEOF = true;
    package.length = 0;

    if (send(sockfd, (void *)&package, sizeof(package), 0)) {
        LOGE("send data error: %s (errno: %d)", strerror(errno), errno);
    }

    count = 0;
}

int TCPClient::recvFromServer(void *buf, int len) {
    if (!isConnected) {
        LOGE("connection hasn't been created...");
        return -1;
    }

    if (buf == nullptr) {
        LOGE("buffer is null...");
        return -1;
    }

    if (len < 0) {
        LOGE("buffer size is invalid...");
        return -1;
    }

    struct DataPackage package;

    recv(sockfd, (void *)&package, sizeof(package), 0);
    if (package.isEOF)
        return 0;

    memcpy(buf, package.buffer, package.length);
    return package.length;
}
