#pragma once
#include<iostream>
#include<sstream>
#include<map>
class bot
{
    private:
    std::map<int,std::string> darija;
    std::map<int,std::string> cooding;
    public:
        bot();
        bot(const bot &dup);
        bot &operator=(const bot &dup);
        ~bot();
        std::string getanswer(std::string input);
        int socketsmsg(std::string input);
        std::string chatoptions();
        std::string What_are_Sockets();
        std::string Things_Irc();
        std::string Importance_Byte();  
        std::string jokes(std::string type);
        std::string botstart(std::stringstream &os);
        std::string himsg();
        std::string Zodiac(int day,int month);
        std::string Helpbot();
        std::string Convertingto();
        std::string PreparingConnection();
        std::string PreparingSockets();
        std::string Clientside();
        std::string Serverside();
        std::string Sendingresv();
        std::string ClosingSocket();

};
