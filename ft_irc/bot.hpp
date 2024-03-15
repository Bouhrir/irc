#pragma once
#include<iostream>
#include<sstream>
class bot
{
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
        std::string botstart(std::string msg);
        std::string himsg();
        std::string Zodiac(int day,int month);
        std::string Helpbot();

};
