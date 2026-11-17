#ifndef REPLIES_HPP
#define REPLIES_HPP

enum ReplyCode
{
    // SUCCESS REPLIES
    RPL_WELCOME             = 001,
    RPL_YOURHOST            = 002,
    RPL_CREATED             = 003,
    RPL_MYINFO              = 004,

    RPL_UMODEIS             = 221,
    RPL_CHANNELMODEIS       = 324,
    RPL_CREATIONTIME        = 329,
    RPL_NOTOPIC             = 331,
    RPL_TOPIC               = 332,
    RPL_INVITING            = 341,
    RPL_NAMREPLY            = 353,
    RPL_ENDOFNAMES          = 366,
    RPL_BANLIST             = 367,
    RPL_ENDOFBANLIST        = 368,

    // ERROR REPLIES
    ERR_NOSUCHNICK          = 401,
    ERR_NOSUCHSERVER        = 402,
    ERR_NOSUCHCHANNEL       = 403,
    ERR_CANNOTSENDTOCHAN    = 404,
    ERR_TOOMANYCHANNELS     = 405,
    ERR_WASNOSUCHNICK       = 406,
    ERR_NOORIGIN            = 409,
    ERR_NORECIPIENT         = 411,
    ERR_NOTEXTTOSEND        = 412,
    ERR_UNKNOWNCOMMAND      = 421,
    ERR_NONICKNAMEGIVEN     = 431,
    ERR_ERRONEUSNICKNAME    = 432,
    ERR_NICKNAMEINUSE       = 433,
    ERR_USERNOTINCHANNEL    = 441,
    ERR_NOTONCHANNEL        = 442,
    ERR_USERONCHANNEL       = 443,
    ERR_NOTREGISTERED       = 451,
    ERR_NEEDMOREPARAMS      = 461,
    ERR_ALREADYREGISTERED   = 462,
    ERR_PASSWDMISMATCH      = 464,

    ERR_KEYSET              = 467,
    ERR_CHANNELISFULL       = 471,
    ERR_UNKNOWNMODE         = 472,
    ERR_INVITEONLYCHAN      = 473,
    ERR_BANNEDFROMCHAN      = 474,
    ERR_BADCHANNELKEY       = 475,
    ERR_BADCHANMASK         = 476,
    ERR_NOCHANMODES         = 477,
    ERR_CHANOPRIVSNEEDED    = 482,
    ERR_INVALIDMODEPARAM    = 696
};

#endif