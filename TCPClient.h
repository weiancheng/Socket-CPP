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
    int sendToServer(const char *data, int data_len);
    int recvFromServer(char *buffer, int buffer_size);

private:
    int sockfd;
    struct sockaddr_in servaddr;
    bool isConnected;
};

#endif
