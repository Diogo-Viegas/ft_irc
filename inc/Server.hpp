#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include "Parser.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <poll.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <signal.h>

class Server
{
    private:
        int _port;
        int _serverFd;
        std::string _password;
        std::vector <struct pollfd> _pollFds;
        std::map<int, Client> _clients;

        void initServer();
        void acceptNewClient();
        void handleClientData(int clientFd);
        void removeClient(int clientFd);

        // Funções de comandos e auxiliares
        void executeCommand(Client &client, const IRCmsg &msg);
        void pass_cmd(Client &client, const std::vector<std::string> &args);
        int nickCommand(Client &client, const std::vector<std::string> &args);
        int userCommand(Client &client, const std::vector<std::string> &args);
        bool nickinUse(const std::string &nickname);

    public:
        Server(int port, const std::string &password);
        ~Server();

        void run();
};

#endif