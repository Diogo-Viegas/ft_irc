#ifndef SERVER_HPP
#define SERVER_HPP
#include "Client.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <poll.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <signal.h>

enum irc_err
{
	ERR_NEEDMOREPARAMS = 461,
	ERR_ALREADYREGISTRED = 462,
};
struct IRCmsg;
class Server
{
    private:
        int _port;
        int _serverFd;
        std::string _password;
        std::vector <struct pollfd> _pollFds;
        std::map<int,Client> _clients;

        void initServer();
        void acceptNewClient();
        void handleClientData(int clientFd);
        void removeClient(int clientFd);
		void pass_cmd(Client &client, const std::vector<std::string> &args);
		int nickCommand(Client &client, const std::vector<std::string> &args);
		int userCommand(Client &client,const std::vector<std::string> &args);
		void executeCommand(Client &client, const IRCmsg &msg);
		bool nickinUse(const std::string &nickname);
    public:
        Server(int port, const std::string &password);
        ~Server();

        void run();
};


#endif