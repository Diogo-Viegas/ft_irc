#include "../inc/Client.hpp"

Client::Client():_fd(-1),_hostname(""),_nickname("*"),_username(""),_realname(""),_hasGivenPassword(false),
_isRegistered(false),_readBuffer(""),_writeBuffer("")
{
}
Client::Client(int fd,const std::string &hostname):_fd(fd),_hostname(hostname),_nickname("*"),_username(""),_realname(""),_hasGivenPassword(false),
_isRegistered(false),_readBuffer(""),_writeBuffer("")
{
}
Client::Client(const Client &src)
{
    *this = src;
}
Client& Client::operator=(const Client &src)
{
    if(this != &src)
    {
        this->_fd = src._fd;
        this->_hostname = src._hostname;
        this->_nickname = src._nickname;
        this->_username = src._username;
        this->_realname = src._realname;
        this->_hasGivenPassword = src._hasGivenPassword;
        this->_isRegistered = src._isRegistered;
        this->_readBuffer = src._readBuffer;
        this->_writeBuffer = src._writeBuffer;
    }
    return *this;
}
Client::~Client()
{   
}
//getters
int Client::getFd() const{return this->_fd;}
const std::string& Client::getHostname() const {return this->_hostname;}
const std::string& Client::getNickname() const {return this->_nickname;}
const std::string& Client::getUsername() const {return this->_username;}
const std::string& Client::getRealname() const {return this->_realname;}
bool Client::hasGivenPassword() const {return this->_hasGivenPassword;}
bool Client::isRegistered() const {return this->_isRegistered;}
std::string& Client::getReadBuffer(){return this->_readBuffer;}
std::string& Client::getWriteBuffer(){return this->_writeBuffer;}

//setters
void Client::setNickname(const std::string &nickname){this->_nickname = nickname;}
void Client::setUsername(const std::string &username){this->_username = username;}
void Client::setRealname(const std::string &realname){this->_realname = realname;}
void Client::setGivenPassword(bool value){this->_hasGivenPassword = value;}
void Client::setRegistered(bool value){this->_isRegistered = value;}

//gestao de buffers
void Client::appendReadBuffer(const std::string &data)
{
    this->_readBuffer += data;
}

bool Client::extractCommand(std::string& command)
{
    size_t pos = this->_readBuffer.find("\r\n");
    size_t delimLen = 2;

    if(pos == std::string::npos)
    {
        pos = this->_readBuffer.find("\n");
        delimLen = 1;
    }
    if(pos != std::string::npos)
    {
        command = this->_readBuffer.substr(0,pos);
        this->_readBuffer.erase(0,pos + delimLen);
        return true;
    }
    return false;
}



void Client::appendWriteBuffer(const std::string &data)
{
    this->_writeBuffer += data;
}