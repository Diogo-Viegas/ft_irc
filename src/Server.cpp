#include "../inc/Server.hpp"
#include "../inc/Parser.hpp"
#include <cerrno>
#include <cstring>
Server::Server(int port,const std::string &password):_serverFd(-1),_port(port),_password(password)
{
}

Server::~Server()
{
	for(int i = 0; i < _pollFds.size(); ++i)
	{
		close(_pollFds[i].fd);
	}
	_clients.clear();
}

//Starts TCP Socket and starts listening

void Server::initServer()
{
	//Create  IPv4 TCP Socket
	_serverFd = socket(AF_INET,SOCK_STREAM,0);
	if(_serverFd < 0)
	{
		std::cerr << "Error Creating Socket" << std::endl;
		std::exit(1);
	}
	//avoid error address already in use
	int opt = 1;
	if(setsockopt(_serverFd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt)) < 0)
	{
		std::cerr << "Error at setsockopt" << std::endl;
		std::exit(1);
	}
	//socket config for NON_BLOCK
	if(fcntl(_serverFd,F_SETFL,O_NONBLOCK) < 0)
	{
		std::cerr << "Error at fcntl" << std::endl;
		std::exit(1);
	}
	//bind the socket to the door
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(_port);

	if(bind(_serverFd,(struct sockaddr*)&serverAddr,sizeof(serverAddr)) < 0)
	{
		std::cerr << "Error bind at port " << _port << std::strerror(errno)<< std::endl;
		std::exit(1);
	}
	//socket listening (passive)
	if(listen(_serverFd,SOMAXCONN) < 0)
	{
		std::cerr << "Error listening" << std::endl;
		std::exit(1);
	} 
	pollfd pfd;
	pfd.fd = _serverFd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	_pollFds.push_back(pfd);
	std::cout << "[Server] Waiting on port " << _port << "..." << std::endl;
}



//Accept new connection
void Server::acceptNewClient()
{
	sockaddr_in clientAddr;
	socklen_t clientLen = sizeof(clientAddr);

	int clientFd = accept(_serverFd,(struct sockaddr *)&clientAddr, &clientLen);
	if(clientFd < 0)
		return;
	fcntl(clientFd,F_SETFL,O_NONBLOCK);

	pollfd pfd;
	pfd.fd = clientFd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	_pollFds.push_back(pfd);

	_clients[clientFd] = Client(clientFd,"127.0.0.1");

	std::cout << "[Server] New Client Logged in at FD: " << clientFd << std::endl;
}
// Step 2 -> Receive and Proccess data from client
void Server::handleClientData(int clientFd)
{
	char buffer[512];
	int bytesRead = recv(clientFd, buffer,sizeof(buffer) - 1,0);
	if(bytesRead <= 0)
	{
		removeClient(clientFd);
		return;
	}
	buffer[bytesRead] = '\0';

	Client &client = _clients[clientFd];
	client.appendReadBuffer(buffer);


	std::string rawLine;
	while (client.extractCommand(rawLine)) {
		if (rawLine.empty())
			continue;

		IRCmsg msg = Parser::parseLine(rawLine);
		if (msg.command.empty())
			continue;

		// Debug visual para validares o parsing:
		std::cout << "[CMD] " << msg.command << " | Args (" << msg.args.size() << "): ";
		for (size_t i = 0; i < msg.args.size(); ++i) {
			std::cout << "[" << msg.args[i] << "] ";
		}
		std::cout << std::endl;

    // Próximo passo: encaminhar para execução
    // executeCommand(client, msg);
	}
}

// Step 3 -> Remove and Clean Client

void Server::removeClient(int clientFd)
{
	std::cout << "[Server] Cliente Logged out at FD: " << clientFd << std::endl;
	close(clientFd);
	for(std::vector<pollfd>::iterator it = _pollFds.begin(); it != _pollFds.end(); ++it)
	{
		if(it->fd == clientFd)
		{
			_pollFds.erase(it);
			break;
		}
	}
	_clients.erase(clientFd);
}
//Main loop event

void Server::run()
{
	initServer();
	std::cout << "[Server] Server Running at main loop... " << std::endl;
	while(true)
	{
		int pollCount = poll(_pollFds.data(),_pollFds.size(),-1);
		if(pollCount < 0)
		{
			std::cerr << "Error poll()" << std::endl;
			break;
		}
		size_t currentSize = _pollFds.size();
		for(size_t i = 0; i < _pollFds.size(); ++i)
		{
			if(_pollFds[i].revents & POLLIN)
			{
				if(_pollFds[i].fd == _serverFd)
					acceptNewClient();
				else
					handleClientData(_pollFds[i].fd);
			}
		}

	}
}