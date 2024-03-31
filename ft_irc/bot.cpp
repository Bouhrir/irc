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
    std::string msg = "Options :\n"
                      "1.jokes\n"
                      "2.things about ft_irc\n"
                      "3.zodiac\n";
    return msg;
}
int bot::socketsmsg(std::string input)
{
    std::string sockets[9] = {"what are sockets", "the importance of byte order", "converting to and from network", "preparing connection", 
    "preparing sockets", "client side", "server side" , "send and recive data" , "closing socket"};
    for (int i = 0; i < 9; i++)
    {
        if (!input.compare(0, sockets[i].length(), sockets[i]))
            return i;
    }
    return -1;
}
std::string bot::jokes(std::string type)
{
    darija.insert(std::make_pair(1, "Waa tga3ad a regraguie\n"));
    darija.insert(std::make_pair(2, "jbedt lhbiba twila w bdit nbacher\n"));
    darija.insert(std::make_pair(4, "Rissala l bocal\n"));
    darija.insert(std::make_pair(5, "hello my name is moo\n"));
    darija.insert(std::make_pair(6, "BaaaaaaaaaaarrrCAAAaaaaa\n"));
    cooding.insert(std::make_pair(1, "Why did the programmer quit his job?\nBecause he didn't get arrays.\n"));
    cooding.insert(std::make_pair(2, "Why did the programmer go broke?\nBecause he used up all his cache.\n"));
    cooding.insert(std::make_pair(3, "Why did the developer go broke?\nBecause he used up all his inheritance.\n"));
    cooding.insert(std::make_pair(4, "Why did the server go to the doctor?\nBecause it was feeling 'down'!\n"));
    cooding.insert(std::make_pair(5, "Why was the C++ developer always tired?\nBecause he was using his 'class' all the time.\n"));
    cooding.insert(std::make_pair(6, "Why was the JavaScript developer sad?\nBecause he didn't know how to 'null' his feelings.\n"));
    int nokta = (rand() % 5) + 1;
    if (type == "darija")
    {
        std::map<int, std::string>::iterator it = darija.find(nokta);
        if (it != darija.end())
            return it->second;
    }
    if (type == "cooding")
    {
        std::map<int, std::string>::iterator it = cooding.find(nokta);
        if (it != cooding.end())
            return it->second;
    }
    return "unknown type :";
}
std::string bot::Things_Irc()
{
    std::string q = "* project what are sockets\n"
                    "* project the importance of byte order\n"
                    "* project converting to and from network\n"
                    "* project preparing connection\n"
                    "* project preparing sockets\n"
                    "* project client side \n"
                    "* project server side \n"
                    "* project send and recive data\n"
                    "* project closing socket\n";
    return q;
}
std::string bot::What_are_Sockets()
{
    std::string msg = "\nEveryone has probably heard the saying that in Unix systems, \033[1;32m“everything is a file”.\033[0m Sockets\n"
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
std::string bot::Clientside()
{
    std::string msg = "\n[- Connecting to a Server via a Socket -]\n"
                      "In order to develop a client, we only need one socket connected to a remote server. All we\n"
                      "have to do is use the connect() system call from the <sys/socket.h> library:\n"
                      "int connect(int sockfd, const struct sockaddr *serv_addr,socklen_t addrlen);\n"
                      "The function predictably returns 0 in for success and -1 for failure, with errno set to indicate the error.\n"
                      "Once more, all of the data needed for the connection can be found in the structure returned by getaddrinfo()\n"
                      "There we go! Our socket is now ready to send and receive data. But before we get to how to do that, \n"
                      "let’s first take a look at establishing a connection from the server side\n";
    return msg;
}
std::string bot::Serverside()
{
    std::string msg = "\n[-Server Side: Accepting Client Connections via a Socket-]\n"
                      "If we want to develop a server, the connection will need to be done in three steps. First, we\n"
                      "need to bind our socket to a local address and port. Then, we’ll have to listen to the port to\n"
                      "detect incoming connection requests. And finally, we need to accept those client connection requests.\n"
                      "\n[-Binding the Socket to the Local Address-]\n"
                      "The bind() function of <sys/socket.h> allows us to link our socket to a local address and\n"
                      "port. Its prototype est practically identical to its twin function connect(), which we examined for the client side:\n"
                      "\n[-Listening via a Socket to Detect Connection Requests-]\n"
                      "Next, we need to mark the socket as “passive”, meaning it will be used to accept incoming\n"
                      "connection requests on the address and port it is bound to. For this, we will use the listen() function, which is also in <sys/socket.h>.\n"
                      "\n[-Accepting a Client Connection-]\n"
                      "Finally, we must accept the connection requests from a remote client. When a client connect()s on the port of our machine that our socket is listen()ing to\n"
                      "its request is put in the pending queue. When we accept() the request, that function will return a new file descriptor bound to the client’s address\n"
                      "hrough which we will be able to communicate with that client. So we will end up with two file descriptors: \n"
                      "ur initial socket that will continue listening to our port, and a new file descriptor for the client\n"
                      "which we can use to send and receive data.\n";
    return msg;
}
std::string bot::ClosingSocket()
{
    std::string msg = "\n[-Closing a Socket Connection-]\n"
                      "Once we are done sending and receiving data, we will be able to close our socket. \n"
                      "Just as any other file descriptor, a socket can be closed with a simple call to close() from <unistd.h>.\n"
                      "This destroys the file descriptor and prevents any further communication with the socket\n"
                      "the remote side will raise an exception if it attempts to communicate with it by sending or receiving data.\n";
        return msg;
}
std::string bot::Sendingresv()
{
    std::string msg = "\n[-Sending and Receiving Data Through Sockets-]\n"
                      "It’s not worth establishing a connection from a client to a server or vice-versa if we don’t\n"
                      "know how to send and receive data. Clever readers might notice that since sockets are just file descriptors,\n"
                      "we could probably just use the read() and write() system calls. And they’d be totally correct! But other functions exist,\n"
                      "ones that give us more control over the way in which our data is sent and received…\n"
                      "\n[-Sending Data via a Socket-]\n"
                      "The send() function from the <sys/socket.h> library allows us to send data through a stream socket,\n"
                      "which uses a TCP connection.\n"
                      "The send() function returns the number of bytes that were successfully sent. Beware, send()\n"
                      "might not be able to send the entire message in one go! This means that we’ll have to be careful to compare\n"
                      "the value returned here with the length of the message we wish to send,\n"
                      "in order to try sending the rest again if need be. As usual,\n"
                      "this function can also return -1 if there is an error.\n"
                      "\n[-Receiving Data via a Socket-]\n"
                      "Just like the opposite function send(), recv() can be found in <sys/socket.h>. This\n"
                      "function allows us to receive data through a socket.\n"
                      "Just like send(), recv() returns the number of bytes it managed to store in the buffer.\n"
                      "However, if recv() returns 0, it can only mean one thing: the remote computer has closed\n"
                      "the connection. Naturally, the recv() function can also return -1 if it encounters an error,\n"; 
    return msg;
}
std::string bot::PreparingSockets()
{
    std::string msg = "\nAt last, we can create the file descriptor for our socket. With it, we will be able to read and\n"
                      "write in order to respectively receive and sent data. The system call from the\n"
                      "<sys/socket.h> library, simply named socket(), is what we need! This is its prototype:\n"
                      "int socket(int domain, int type, int protocol)\n"
                      "The socket() function returns the file descriptor of the new socket. In case of failure, it\n"
                      "returns -1 and indicates the error it encountered in errno.\n"
                      "But this socket file descriptor is not yet connected to anything at all. Naturally, we will want to\n"
                      "associate it to a socket address (meaning an IP address and port combination). For this, we have two choices:\n"
                      "* Connect the socket to a remote address with connect(). This will allow it to work as a\n"
                      "  client, able to make requests to a remote server\n"
                      "* Connect the socket to a local address with bind(). In this case, it will work as a server,\n"
                      "  able to accept connections from remote clients.\n";
    return msg;
}
std::string bot::PreparingConnection()
{
    std::string msg = "\nWhether our program is a server or a client, the first thing we need to do is prepare\n"
                      "a small data structure. This structure will contain the information that our socket will need:\n"
                      "notably, the IP address and the port to connect to.\n"
                      "[-Structures for the Connection IP Address and Port-]\n"
                      "The basic structures we need to use in order to hold the IP\n"
                      "The basic structures we need to use in order to hold the IP address and port we want to connect to\n"
                      "can be found in the <netinet/in.h> library. \n"
                      "There are two variants of them: one for IPv4 and one for IPv6.\n";
    return msg;
}
std::string bot::Importance_Byte()
{
    std::string msg = "\nWhenever we wish to send and receive data from one computer to another, we must keep in\n"
                      "mind that systems can represent their data in two distinct and opposite ways. Take for\n"
                      "example the hexadecimal integer 2F0A (which is 12042 in decimal). Because of its size, this\n"
                      "integer must be stored over two bytes: 2F and 0A.\n"
                      "It is logical to assume that this integer will always be stored in this order: 2F, followed by 0A.\n"
                      "This is the most common ordering, known as “big endian” since the big end of the number,\n"
                      "the most significant byte, is stored first. But this is not always the case…\n"
                      "In some systems, particularly those with an Intel or Intel-compatible processor,\n"
                      "prefer storing the bytes of our integer in the opposite order, with the least significant,\n"
                      "or small end, first: 0A followed by 2F. We call this ordering “little endian”.\n";

    return msg;
}
std::string bot::Convertingto()
{
    std::string msg = "\nThankfully, we can simply assume that the host system is not storing its bytes in the\n"
                      "right order compared to the network. All we then need to do is to systematically\n"
                      "reorder these bytes when we transfer them between the network and host systems.\n"
                      "For this, we can make use of four handy functions from the <arpa/inet.h> library:\n"
                      "uint32_t htonl(uint32_t hostlong);  |Host to network long|\n"
                      "uint16_t htons(uint16_t hostshort); |Host to network short|\n"
                      "uint32_t ntohl(uint32_t netlong);   |Network to host long|\n"
                      "uint16_t ntohs(uint16_t netshort); |Network to host short|\n"
                      "As we can see, these functions come in two variants: the ones that convert a short\n"
                      "(two bytes, or 16 bits), and those that convert a long (four bytes or 32 bits). They also\n"
                      "work for unsigned integers.\n"
                      "In order to convert a four byte (32 bit) integer from the Host Byte Order to the\n"
                      "Network Byte Order, we’ll want to call the htonl() function (“htonl” stands for “Host\n"
                      "to Network Long”). For the opposite operation, we’d use ntohl() (“Network to Host Long”).\n";
    return msg;
}
std::string bot::Zodiac(int day, int month)
{
        if (month < 1 || month > 12 || day < 1 || day > 31) {
        return "invalid date\n";
    }

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
        return "Scorpio\n";
    } else if ((month == 11 && day >= 22) || (month == 12 && day <= 21)) {
        return "Sagittarius\n";
    } else if ((month == 12 && day >= 22) || (month == 1 && day <= 19)) {
        return "Capricorn\n";
    } else if ((month == 1 && day >= 20) || (month == 2 && day <= 18)) {
        return "Aquarius\n";
    } else if ((month == 2 && day >= 19) || (month == 3 && day <= 20)) {
        return "Pisces\n";
    } else {
        return "invalid date\n";
    }
}
std::string bot::getanswer(std::string input)
{
    input.erase(0,1);
    int msg = socketsmsg(input);
    switch (msg)
    {
    case 0:
        return What_are_Sockets();
    case 1:
        return Importance_Byte();
    case 2:
        return Convertingto();
    case 3:
        return PreparingConnection();
    case 4:
        return PreparingSockets();
    case 5:
        return Clientside();
    case 6:
        return Serverside();
    case 7:
        return Sendingresv();
    case 8:
        return ClosingSocket();
    }
    std::string error = "It seems like you've entered '" + input + "' is there something specific you would like assistance with regarding this input?\n"
                                                                   "if you're confused to use bot check cmd '/Emit help'\n";
    return error;
}
std::string bot::himsg()
{
    std::string hi = "Hi to make the conversation esear\n"
                     "Please select from the following options:\n"
                     "O. To get choices\n"
                     "/Emit help. for help\n";
    return hi;
}
std::string bot::Helpbot()
{
    std::string help = "For jokes enter  1 + the type you want (coding,darija)\n"
                       "For irc question enter the giving when you tap 2\n"
                       "For zodiac enter 'Z day month'\n";
    return help;
}
std::string bot::botstart(std::stringstream &os)
{
    std::string msg;
    os >> msg;
    if (!msg.compare(0, 2, "hi") || !msg.compare(0, 3, "hey") || !msg.compare(0, 5, "hello"))
    {
        return himsg();
    }
    else if (msg == "0")
        return chatoptions();
    else if (msg == "1")
    {
        std::string type;
        os >> type;
        std::cout << type << std::endl;
        return jokes(type);
    }
    else if (msg == "2")
        return Things_Irc();
    else if (msg == "help")
        return Helpbot();
    else if (msg == "Z")
    {
        std::string  day, month;
        os >> day;
        os >> month;
        return Zodiac(std::atoi(day.c_str()), std::atoi(month.c_str()));
    }
    else if (msg == "project") {
        std::streampos position = os.tellg();

        // Seek the read pointer back to the previous position
        os.seekg(position);

        // Get the remaining contents of the stringstream
        std::string remaining;
        std::getline(os, remaining);
        return getanswer(remaining);
    }

    return "Error";
}
bot::~bot()
{
}