#include "../inc/Server.hpp"

bool Server::signal = false;

void Server::clearClients(int fd){ //-> clear the clients
	for(size_t i = 0; i < fds.size(); i++){ //-> remove the client from the pollfd
		if (fds[i].fd == fd)
			{fds.erase(fds.begin() + i); break;}
	}
	for(size_t i = 0; i < clients.size(); i++){ //-> remove the client from the vector of clients
		if (clients[i].getFd() == fd)
			{clients.erase(clients.begin() + i); break;}
	}

}

void Server::signalHandler(int signum)
{
    (void)signum;
    std::cout << "Signal Received!" << std::endl;
    Server::signal = true;
}

void Server::closeFds(){
	for(size_t i = 0; i < clients.size(); i++){ //-> close all the clients
		std::cout << "Client <" << clients[i].getFd() << "> Disconnected" << std::endl;
		close(clients[i].getFd());
	}
	if (serverSocketFD != -1){ //-> close the server socket
		std::cout << "Server <" << serverSocketFD << "> Disconnected"<< std::endl;
		close(serverSocketFD);
	}
}


void Server::serverSocket()
{
	struct sockaddr_in addr;
	struct pollfd NewPoll;
	addr.sin_family = AF_INET; //-> set the address family to ipv4
	addr.sin_port = htons(this->port); //-> convert the port to network byte order (big endian)
	addr.sin_addr.s_addr = INADDR_ANY; //-> set the address to any local machine address

	serverSocketFD = socket(AF_INET, SOCK_STREAM, 0); //-> create the server socket
	if(serverSocketFD == -1) //-> check if the socket is created
		throw(std::runtime_error("failed to create socket"));

	int en = 1;
	if(setsockopt(serverSocketFD, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1) //-> set the socket option (SO_REUSEADDR) to reuse the address
		throw(std::runtime_error("failed to set option (SO_REUSEADDR) on socket"));
	if (fcntl(serverSocketFD, F_SETFL, O_NONBLOCK) == -1) //-> set the socket option (O_NONBLOCK) for non-blocking socket
		throw(std::runtime_error("faield to set option (O_NONBLOCK) on socket"));
	if (bind(serverSocketFD, (struct sockaddr *)&addr, sizeof(addr)) == -1) //-> bind the socket to the address
		throw(std::runtime_error("failed to bind socket"));
	if (listen(serverSocketFD, SOMAXCONN) == -1) //-> listen for incoming connections and making the socket a passive socket
		throw(std::runtime_error("listen() failed"));

	NewPoll.fd = serverSocketFD; //-> add the server socket to the pollfd
	NewPoll.events = POLLIN; //-> set the event to POLLIN for reading data
	NewPoll.revents = 0; //-> set the revents to 0
	fds.push_back(NewPoll); //-> add the server socket to the pollfd
}

void Server::serverInit()
{
	this->port = 4444;
	serverSocket(); //-> create the server socket

	std::cout << "Server <" << serverSocketFD << "> Connected" << std::endl;
	std::cout << "Waiting to accept a connection...\n";

	while(Server::signal == false) //run the server until the signal is received
	{
		if((poll(&fds[0],fds.size(),-1) == -1) && Server::signal == false) //wait for an event
			throw(std::runtime_error("poll() failed"));
		for(size_t i = 0; i < fds.size(); i++)
		{
			if(fds[i].revents & POLLIN)
			{
				if(fds[i].fd  == serverSocketFD)
					acceptNewClient();
				else
					receiveNewData(fds[i].fd);
			}
		}
	}
	closeFds();
}

int main()
{
	Server ser;
	std::cout << "---- SERVER ----" << std::endl;
	try{
		signal(SIGINT, Server::signalHandler); //-> catch the signal (ctrl + c)
		signal(SIGQUIT, Server::signalHandler); //-> catch the signal (ctrl + \)
		ser.serverInit(); //-> initialize the server
	}
	catch(const std::exception& e){
		ser.closeFds(); //-> close the file descriptors
		std::cerr << e.what() << std::endl;
	}
	std::cout << "The Server Closed!" << std::endl;
}
