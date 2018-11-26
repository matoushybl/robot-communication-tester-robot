//
// Created by Matous Hybl on 2018-10-19.
//

#ifndef FIRMWARE_UDP_H
#define FIRMWARE_UDP_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

class UDP {
public:
    UDP(uint16_t port);
    ~UDP();

    void receive(unsigned char **buffer);
    void send(const char* host, uint16_t port, unsigned char *buffer, uint32_t size);

    bool available();
private:
    int socketDescriptor = 0;
};


#endif //FIRMWARE_UDP_H
