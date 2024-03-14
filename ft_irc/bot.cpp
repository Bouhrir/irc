#include"bot.hpp"
bot::bot()
{
}
bot::bot(string in)
{
    this->input = in;
}
bot::bot(const bot __unused&dup)
{
}
bot &bot::operator=(const bot __unused&dup)
{
    return *this;
}
bot::~bot()
{

}