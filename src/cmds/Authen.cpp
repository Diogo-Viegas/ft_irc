#include "../../inc/Server.hpp"
#include "../../inc/Replies.hpp"
#include "../../inc/Client.hpp"

void Server::pass_cmd(Client &client, const std::vector<std::string> &args)
{
	if(args.empty()) 
	{
		std::cout << "[PASS] Erro: numero incorreto de argumentos" << std::endl;
		return;
	}
	if(args[0] != _password)
	{
		std::cout << "[PASS] Password incorreta!" << std::endl;
		client.setGivenPassword(false);
		return;
	}
	client.setGivenPassword(true);
	std::cout << "[PASS] Client authenticated successfully" << std::endl;
}

static bool isNickChar(char c)
{
	if (c == '[' || c == ']' || c == '\\' || c == '`' || c == '_' || c == '^'
		|| c == '{' || c == '|' || c == '}' || c == '-')
		return (true);
	return (false);
}

static bool is_valid_nick(const std::string &nick)
{
	if(nick.empty() || nick.size() > 9)
		return false;
	if(!std::isalpha(nick[0]) && !isNickChar(nick[0]))
		return false;
	for(size_t i = 1; i < nick.size(); i++)
	{
		if(!std::isalnum(nick[i]) && !isNickChar(nick[i]))
			return false;
	}
	return true;
}

bool Server::nickinUse(const std::string &nickname)
{
	for(size_t i = 0; i < this->_clients.size(); i++)
	{
		if(this->_clients[i].getNickname() == nickname)
			return true;
	}
	return false;
}

int Server::nickCommand(Client &client, const std::vector<std::string> &args)
{
	if(args.empty())
		return (1);
	
	std::string nickname = args[0];
	
	if(!is_valid_nick(nickname) || nickinUse(nickname))
		return (1);
	client.setNickname(nickname);
	std::cout << "[NICK] Nickname changed to: " << nickname << std::endl;
	
	return (0);
}

int Server::userCommand(Client &client,const std::vector<std::string> &args)
{
	if (args.size() < 4)
		return(ERR_NEEDMOREPARAMS);

	if (client.isRegistered())
		return(ERR_ALREADYREGISTERED);

	client.setUsername(args[0]);
	client.setRealname(args[3]);

	std::cout << "[USER] Username: " << args[0] << std::endl;
	std::cout << "[USER] Realname: " << args[3] << std::endl;
	return 0;

}