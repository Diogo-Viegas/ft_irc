#include "../inc/Parser.hpp"
#include <cctype>

std::string Parser::toUpper(const std::string& str) {
    std::string res = str;
    for (size_t i = 0; i < res.length(); ++i) {
        res[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(res[i])));
    }
    return res;
}

IRCmsg Parser::parseLine(const std::string& line) {
    IRCmsg msg;
    size_t pos = 0;
    size_t len = line.length();

    // 1. Ignorar espaços iniciais
    while (pos < len && line[pos] == ' ')
        pos++;

    if (pos >= len)
        return msg;

    // 2. Verificar se existe um prefixo opcional (começa por ':')
    if (line[pos] == ':') {
        size_t spacePos = line.find(' ', pos);
        if (spacePos == std::string::npos)
            return msg; // Linha inválida (só tem prefixo)
        msg.prefix = line.substr(pos + 1, spacePos - (pos + 1));
        pos = spacePos;
        while (pos < len && line[pos] == ' ')
            pos++;
    }

    // 3. Extrair o COMANDO e converter para MAIÚSCULAS
    size_t cmdEnd = line.find(' ', pos);
    if (cmdEnd == std::string::npos) {
        msg.command = toUpper(line.substr(pos));
        return msg;
    }
    msg.command = toUpper(line.substr(pos, cmdEnd - pos));
    pos = cmdEnd;

    // 4. Extrair os ARGUMENTOS
    while (pos < len) {
        while (pos < len && line[pos] == ' ')
            pos++;
        if (pos >= len)
            break;

        // Se encontrar ':', o resto da linha é o último argumento (com espaços)
        if (line[pos] == ':') {
            msg.args.push_back(line.substr(pos + 1));
            break;
        }

        // Caso contrário, extrai até ao próximo espaço
        size_t nextSpace = line.find(' ', pos);
        if (nextSpace == std::string::npos) {
            msg.args.push_back(line.substr(pos));
            break;
        } else {
            msg.args.push_back(line.substr(pos, nextSpace - pos));
            pos = nextSpace;
        }
    }

    return msg;
}