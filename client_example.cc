#include "TCPClient.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <cstring>

#define BUFFER_SIZE 4096
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6666

using namespace std;


int main(int argc, char **argv) {
    TCPClient client;
    FILE *filp = nullptr;
    char buffer[BUFFER_SIZE];

    filp = fopen("output.jpg", "wb");
    if (filp == nullptr) {
        printf("Error, failed to create output file...\n");
        return -1;
    }

    if (!client.start(SERVER_IP, SERVER_PORT)) {
        printf("Error, failed to connect to server(%s:%d)\n", SERVER_IP, SERVER_PORT);
        fclose(filp);
        return -1;
    }

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int length = client.recvFromServer(buffer, BUFFER_SIZE);
        if (length == 0) {
            break;
        }

        printf("length: %d\n", length);

        fwrite(buffer, sizeof(char), length, filp);
    }

    client.stop();

    fclose(filp);

    return 0;
}
