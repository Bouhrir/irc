#pragma once
#include<iostream>
using std::cout;
using std::string;
using std::endl;
using std::cin;
class bot
{
    private:
        string input;
    public:
        bot();
        bot(string in);
        bot(const bot &dup);
        bot &operator=(const bot &dup);
        ~bot();
};

bot::bot(/* args */)
{
}

bot::~bot()
{
}
