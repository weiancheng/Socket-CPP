#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

#include <string>
#include <netinet/in.h>

class TCPClient {
public:
    TCPClient();
    ~TCPClient();

    bool start(std::string ip, uint32_t port);
    void stop();
    int sendToServer(const void *buf, int len);
    void sendEOFToServer();
    int recvFromServer(void *buf, int len);

private:
    int sockfd;
    int count;
    struct sockaddr_in servaddr;
    bool isConnected;
};

#endif
