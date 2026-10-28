#ifndef SERVER_HPP
#define SERVER_HPP
#include <iostream>
#include <vector>
#include <poll.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <signal.h>
class Client
{
    private:
        int Fd;
        std::string IPaddr;
    public:
        Client(){};
        int getFd(){return Fd;}
        void setFd(int fd){Fd = fd;}
        void setIPaddr(std::string ipaddr){IPaddr = ipaddr;}
};

class Server
{
    private:
        int port;
        int serverSocketFD;
        static bool signal;
        std::vector <Client> clients;
        std::vector <struct pollfd> fds;
    public:
        Server(){serverSocketFD = -1;}

        void serverInit();
        void serverSocket();
        void acceptNewClient();
        void receiveNewData(int fd);

        static void signalHandler(int signum);

        void closeFds();
        void clearClients(int fd);
};


#endif