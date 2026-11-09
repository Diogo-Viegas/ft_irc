#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <string>
class Client
{
    private:
        int _fd;
        std::string _hostname;
        std::string _nickname;
        std::string _username;
        std::string _realname;

        bool _hasGivenPassword;
        bool _isRegistered;

        std::string _readBuffer;
        std::string _writeBuffer;
    public:
        Client();
        Client(int fd, const std::string &hostname);
        Client(const Client &src);
        Client& operator=(const Client &src);
        ~Client();
    
        //getters
        int getFd() const;
        const std::string& getHostname() const;
        const std::string& getNickname() const;
        const std::string& getUsername() const;
        const std::string& getRealname() const;
        bool    hasGivenPassword() const;
        bool    isRegistered() const;
        std::string& getReadBuffer();
        std::string& getWriteBuffer();
        //setters
        void setNickname(const std::string &nickname);
        void setUsername(const std::string &username);
        void setRealname(const std::string &realname);
        void setGivenPassword(bool value);
        void setRegistered(bool value);
        //gestao do buffer de leitura (parsing /r /n)
        void appendReadBuffer(const std::string &data);
        bool extractCommand(std::string& command);

        //gestao do buffer de envio
        void appendWriteBuffer(const std::string &data);
};

#endif