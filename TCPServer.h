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
    int sendToClient(int client_fd, const void *buf, int len);
    void sendEOFToClient(int client_fd);
    int recvFromClient(int client_fd, void *buf, int len);

private:
    int sockfd;
    int count;
    struct sockaddr_in servaddr;
};

#endif
