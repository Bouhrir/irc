#include "bot.hpp"
bot::bot()
{
}
bot::bot(const bot &__unused dup)
{
}
bot &bot::operator=(const bot &__unused dup)
{
    return *this;
}
std::string bot::chatoptions()
{
    std::string msg = "Options :\n"  "1.jokes\n"  "2.things about ft_irc\n" "3.zodiac\n";
    return msg;
}
int bot::socketsmsg(std::string input)
{
    std::string sockets[3] = {"what are sockets","the importance of byte order", "converting to and from network byte order"};
    for (int i = 0; i < 3; i++)
    {
        if (input == sockets[i])
            return i;
    }
    return -1;
}
std::string bot::jokes(std::string type)
{
    int nokta = rand() % 5;
    std::string darija[5] ={ "Waa tga3ad a regraguie\n","2","3","4","5"};
    if(type == "darija")
        return darija[nokta];
    std::string cooding[5] = {"Why did the programmer go broke?\nBecause he used up all his cache.\n","2","3","4","5"};
    if(type == "cooding")
        return cooding[nokta];
    return "unknown type :";
}
std::string bot::Things_Irc()
{
    std::string quetions = "* what are sockets\n" "* the importance of byte order\n" "* converting to and from network byte order\n";
    return quetions;
}
std::string bot::What_are_Sockets()
{
    std::string msg = "Everyone has probably heard the saying that in Unix systems, \033[1;32m“everything is a file”.\033[0m Sockets\n"
    "are no exception. Indeed, a socket is simply a file descriptor that enables remote \n"
    "communication. There are several different types of sockets, but in this article, we will\n"
    "concentrate on Internet sockets.\n"
    "Of course, there are also many types of Internet sockets which all have different ways to\n"
    "transmit data. Among them, the two main types are:\n"
    "Stream sockets (SOCK_STREAM), which use the TCP in order to communicate. This protocol enforces\n"
    "a reliable and connected transmission of data, at the cost of a slightly reduced performance.\n"
    "Datagram sockets (SOCK_DGRAM), which use UDP. As opposed to TCP, UDP allows connection-less data transmission,\n"
    "which is fast, but without any guarantee\n"
    "We’ve had the chance to explore both of these protocols,\n" 
    "TCP and UDP in the article about the network layers of the Internet.\n";
    return msg;
}
std::string bot::Importance_Byte()
{
    std::string msg = "Whenever we wish to send and receive data from one computer to another, we must keep in\n"
    "mind that systems can represent their data in two distinct and opposite ways. Take for\n"
    "example the hexadecimal integer 2F0A (which is 12042 in decimal). Because of its size, this\n"
    "integer must be stored over two bytes: 2F and 0A.\n"
    "It is logical to assume that this integer will always be stored in this order: 2F, followed by 0A.\n"
    "This is the most common ordering, known as “big endian” since the big end of the number,\n"
    "the most significant byte, is stored first. But this is not always the case…\n";
    return msg;
}
std::string bot::Zodiac(int day,int month)
{
    if ((month == 3 && day >= 21) || (month == 4 && day <= 19)) {
        return "Aries\n";
    } else if ((month == 4 && day >= 20) || (month == 5 && day <= 20)) {
        return "Taurus\n";
    } else if ((month == 5 && day >= 21) || (month == 6 && day <= 20)) {
        return "Gemini\n";
    } else if ((month == 6 && day >= 21) || (month == 7 && day <= 22)) {
        return "Cancer\n";
    } else if ((month == 7 && day >= 23) || (month == 8 && day <= 22)) {
        return "Leo\n";
    } else if ((month == 8 && day >= 23) || (month == 9 && day <= 22)) {
        return "Virgo\n";
    } else if ((month == 9 && day >= 23) || (month == 10 && day <= 22)) {
        return "Libra\n";
    } else if ((month == 10 && day >= 23) || (month == 11 && day <= 21)) {
        return "\033[1;32mScorpio\n\033[0m";
    } else if ((month == 11 && day >= 22) || (month == 12 && day <= 21)) {
        return "Sagittarius\n";
    } else if ((month == 12 && day >= 22) || (month == 1 && day <= 19)) {
        return "Capricorn\n";
    } else if ((month == 1 && day >= 20) || (month == 2 && day <= 18)) {
        return "Aquarius\n";
    } else if ((month == 2 && day >= 19) || (month == 3 && day <= 20)) {
        return "Pisces\n";
    } else {
        return "Invalid date\n";
    }
}
std::string bot::getanswer(std::string input)
{
    int msg = socketsmsg(input);
    switch (msg)
    {
    case 0:
        return What_are_Sockets();
    case 1:
        return Importance_Byte();
        break;
    // default:
    //     return chatoptions();
    //     break;
    }
    std::string error =  "It seems like you've entered '" +  input + "' is there something specific you would like assistance with regarding this input?\n" 
    "if you're confused to use bot check cmd '!help'\n";
    return error;
}
std::string bot::himsg()
{
            std::string hi = "Hi to make the conversation esear\n"
            "Please select from the following options:\n"
            "O. To get choices\n"
            "!help. for help\n";
        return hi;
}
std::string bot::Helpbot()
{
    std::string help = "For jokes enter  1 + the type you want (coding,darija)\n"
    "For irc question enter the giving when you tap 2\n"
    "For zodiac enter 'Z day month'\n";
    return help;
}
std::string bot::botstart(std::string msg)
{
        std::string day;
        std::string month,box;
        if(msg == "hi" || msg == "hey" || msg == "hello")
        {
            return himsg();
        }
        else if(msg == "O")
            return chatoptions();
        else if(msg[0] == '1' && msg[1] == ' ')
        {
            std::stringstream os(msg);
            std::string hna,type;
            os >> hna >> type;
            std::cout << type << std::endl;
            return jokes(type);
        }
        else if(msg == "2")
            return Things_Irc();
        else if(msg == "!help")
            return Helpbot();
        else if(msg[0] == 'Z' && msg[1] == ' ')
        {
            std::stringstream os(msg);
            os >> box;
            os >> day;
            os >> month;
            return Zodiac(std::atoi(day.c_str()),std::atoi(month.c_str()));
        }
        else
            return getanswer(msg);
    std::string error =  "It seems like you've entered '" +  msg + "' is there something specific you would like assistance with regarding this input?\n" 
    "if you're confused to use bot check cmd '!help'\n";
    return error;
}
bot::~bot()
{
}