#include "TCPServer.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <iostream>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>

#define BACKLOG 10

#define TAG "TCPServer"
#define LOGD(format, ...) printf("[%s][%s][%s][%d]: " format "\n", TAG, __FILENAME__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define LOGI(format, ...) printf("[%s] " format "\n", TAG, ##__VA_ARGS__)
#define LOGE(format, ...) printf("[%s] Error: " format "\n", TAG, ##__VA_ARGS__)

using namespace std;


TCPServer::TCPServer(): sockfd(-1) {
}

TCPServer::~TCPServer() {
}

bool TCPServer::start(uint32_t port) {
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        LOGE("create socket error: %s (errno: %d)", strerror(errno), errno);
        return false;
    }
    LOGI("create socket...");

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    int contain = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &contain, sizeof(int));

    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
        LOGE("bind socket error: %s (errno: %d)", strerror(errno), errno);
        return false;
    }
    LOGI("bind socket...");

    if (listen(sockfd, BACKLOG) == -1) {
        LOGI("listen socket error: %s (errno: %d)", strerror(errno), errno);
        return false;
    }

    LOGI("start socket...");

    return true;
}

void TCPServer::stop() {
    LOGI("stop socket...");
    if (sockfd > 0) {
        close(sockfd);
    }
    sockfd = -1;
}

int TCPServer::waitForClient() {
    int client_fd = -1;
    struct sockaddr_in client_addr;
    socklen_t size = sizeof(client_addr);

    LOGI("waiting for client's request...");

    if ((client_fd = accept(sockfd, (struct sockaddr*)&client_addr, &size)) == -1) {
        LOGE("accept socket error: %s (errno: %d)", strerror(errno), errno);
        return -1;
    }

    LOGI("got a connection...");
    return client_fd;
}

int TCPServer::sendToClient(int client_fd, const char *data, int data_len) {
    if (sockfd < 0) {
        LOGE("need to create socket...");
        return -1;
    }

    if (client_fd < 0) {
        LOGE("client FD(%d) is invalid...", client_fd);
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

    int n = send(client_fd, data, data_len, 0);
    if (n < 0) {
        LOGE("send data error: %s (errno: %d)", strerror(errno), errno);
        return -1;
    }

    return n;
}

int TCPServer::recvFromClient(int client_fd, char *buffer, int buffer_size) {
    if (sockfd < 0) {
        LOGE("need to create socket...");
        return -1;
    }

    if (client_fd < 0) {
        LOGE("client FD(%d) is invalid...", client_fd);
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

    return recv(client_fd, buffer, buffer_size, 0);
}
