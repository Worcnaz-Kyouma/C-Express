#ifndef CE_SOCKET_H
#define CE_SOCKET_H

class Socket {
public:
    Socket();

    bind();
    listen();
    accept();

    read();
    write();
};

#endif