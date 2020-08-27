#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#include <netinet/in.h>

class TCPServer {
public:
    TCPServer();
    ~TCPServer();

    bool start(uint32_t port);
    void stop();
    int waitForClient();
    int sendToClient(int client_fd, const char *data, int data_len);
    int recvFromClient(int client_fd, char *buffer, int buffer_size);

private:
    int sockfd;
    struct sockaddr_in servaddr;
};

#endif
