#include "TCPClient.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <cstring>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6666
#define BUFFER_SIZE 4096

using namespace std;


int main(int argc, char **argv) {
    TCPClient client;
    char buffer[BUFFER_SIZE];
    FILE *filp = nullptr;

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
        int length = client.recvFromServer(buffer, BUFFER_SIZE);
        if (length == 0) {
            printf("got EOF...\n");
            break;
        }

        printf("length: %d\n", length);

        fwrite(buffer, sizeof(char), length, filp);
    }

    printf("close file...\n");
    fclose(filp);
    client.stop();

    return 0;
}
